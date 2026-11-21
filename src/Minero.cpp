#include "Minero.hpp"
#include "Types.hpp"
#include "Performers.hpp"
#include "Persons.hpp"
#include "Albums.hpp"
#include "Rolas.hpp"


#include <algorithm>
#include <cctype>
#include <iostream>
#include <stdexcept>

#if __has_include(<taglib/fileref.h>)
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#define TIENE_TAGLIB 1
#else
#define TIENE_TAGLIB 0
#endif

namespace fs = std::filesystem;

// clase LectorEtiquetasMP3 ------------------------------------

void LectorEtiquetasMP3::aplicarValoresPorDefecto(const fs::path& ruta, MetadatosMP3& meta) {
    if (meta.title.empty()) {
        meta.title = ruta.stem().string();
    }
    if (meta.album.empty()) {
        std::string carpeta = ruta.parent_path().filename().string();
        meta.album = carpeta.empty() ? "Álbum Desconocido" : carpeta;
    }
    if (meta.artist.empty()) {
        meta.artist = "Desconocido";
    }
}

MetadatosMP3 LectorEtiquetasMP3::leer(const fs::path& ruta) const {
    MetadatosMP3 meta;
    meta.path = ruta.string();
    meta.discNumber = 1;

#if TIENE_TAGLIB
    TagLib::FileRef archivo(meta.path.c_str());

    if (!archivo.isNull() && archivo.tag()) {
        TagLib::Tag* tag = archivo.tag();

        meta.title   = tag->title().to8Bit(true);
        meta.artist  = tag->artist().to8Bit(true);
        meta.album   = tag->album().to8Bit(true);
        meta.genre   = tag->genre().to8Bit(true);
        meta.comment = tag->comment().to8Bit(true);
        meta.year    = static_cast<int>(tag->year());
        meta.track   = (tag->track() > 0) ? std::to_string(tag->track()) : "";

        if (archivo.file()) {
            TagLib::PropertyMap props = archivo.file()->properties();
            if (props.contains("DISCNUMBER") && !props["DISCNUMBER"].isEmpty()) {
                std::string discoRaw = props["DISCNUMBER"].front().to8Bit(true);
                size_t barra = discoRaw.find('/');
                if (barra != std::string::npos) {
                    discoRaw = discoRaw.substr(0, barra);
                }
                try {
                    int num = std::stoi(discoRaw);
                    if (num > 0) meta.discNumber = num;
                } catch (...) {
                    meta.discNumber = 1;
                }
            }
        }
    }
#endif

    aplicarValoresPorDefecto(ruta, meta);
    return meta;
}


// Clase Minero ----------------------------------------------------

Minero::Minero(CompiladorBusquedasFacade& buscador, const std::string& conn_str)
    : buscador(buscador) {
    try {
        this->conn = std::make_unique<pqxx::connection>(conn_str);
        if (!conn->is_open()) {
            std::cerr << "Minero: No se pudo abrir la conexión con PostgreSQL." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error crítico en conexión del Minero: " << e.what() << std::endl;
    }
}

Minero::~Minero() {
    conn.reset();
}

bool Minero::esArchivoMP3(const fs::path& ruta) {
    if (!ruta.has_extension()) {
        return false;
    }

    std::string ext = ruta.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    return ext == ".mp3" || ext == ".flac" || ext == ".m4a";
}



int Minero::obtenerMaxId(const std::string& entidad, const std::string& columnaPk) {
    ConsultaBuilder builder;
    ConsultaAST ast = builder
        .seleccionarEntidad(entidad)
        .agregarCampo(entidad, columnaPk, "max_id")
        .ordenarPor(entidad, columnaPk, DireccionOrden::DESC)
        .limitar(1)
        .construir();

    json res = buscador.ejecutarConsulta(ast);
    if (!res.empty() && !res[0]["max_id"].is_null()) {
        return res[0]["max_id"].get<int>();
    }
    return 0;
}

void Minero::inicializarContadores() {
    int maxPerf  = obtenerMaxId("performers", "id_performer");
    int maxAlbum = obtenerMaxId("albums", "id_album");
    int maxRola  = obtenerMaxId("rolas", "id_rola");

    nextIdPerformer = std::max(maxPerf + 1, 2);
    nextIdAlbum     = std::max(maxAlbum + 1, 1);
    nextIdRola      = std::max(maxRola + 1, 1);
}

void Minero::asegurarRegistrosBase() {
    Types tipoBase(ID_TIPO_DEFAULT, "Desconocido");
    if (tipoBase.Leer_Consultar(ID_TIPO_DEFAULT).empty()) {
        tipoBase.Crear_Insertar();
    }

    Persons personaBase(ID_PERSON_DEFAULT, "Desconocido", "Desconocido", "", "");
    if (personaBase.Leer_Consultar(ID_PERSON_DEFAULT).empty()) {
        personaBase.Crear_Insertar();
    }

    Performers perfBase(ID_PERFORMER_DEFAULT, ID_TIPO_DEFAULT, "Desconocido", "");
    if (perfBase.Leer_Consultar(ID_PERFORMER_DEFAULT).empty()) {
        perfBase.Crear_Insertar();
    }
}

void Minero::vincularPerformerAAlbum(int idAlbum, int idPerformer) {
    if (!conn || !conn->is_open()) return;
    pqxx::work txn(*conn);
    txn.exec_params("UPDATE albums SET id_performer = $1 WHERE id_album = $2;", idPerformer, idAlbum);
    txn.commit();
}

int Minero::obtenerOCrearPerformer(const std::string& nombreArtist) {
    if (nombreArtist == "Desconocido") {
        return ID_PERFORMER_DEFAULT;
    }

    auto itCache = cachePerformers.find(nombreArtist);
    if (itCache != cachePerformers.end()) {
        return itCache->second;
    }

    if (auto idExistente = buscador.buscarIdPerformer(nombreArtist)) {
        cachePerformers[nombreArtist] = *idExistente;
        return *idExistente;
    }

    int nuevoId = nextIdPerformer++;
    Performers nuevoPerf(nuevoId, ID_TIPO_DEFAULT, nombreArtist, "");
    nuevoPerf.Crear_Insertar();

    cachePerformers[nombreArtist] = nuevoId;
    return nuevoId;
}

int Minero::obtenerOCrearAlbum(const std::string& nombreAlbum,
                               const std::string& rutaCarpeta,
                               int year,
                               int idPerformer) {
    const std::string claveCache = std::to_string(idPerformer) + "::" + nombreAlbum;
    auto itCache = cacheAlbums.find(claveCache);
    if (itCache != cacheAlbums.end()) {
        return itCache->second;
    }

    if (auto idExistente = buscador.buscarIdAlbum(nombreAlbum, idPerformer)) {
        cacheAlbums[claveCache] = *idExistente;
        return *idExistente;
    }

    int nuevoId = nextIdAlbum++;
    Albums nuevoAlbum(nuevoId, rutaCarpeta, nombreAlbum, year, "");
    nuevoAlbum.Crear_Insertar();
    vincularPerformerAAlbum(nuevoId, idPerformer);

    cacheAlbums[claveCache] = nuevoId;
    return nuevoId;
}

ResumenMinado Minero::minarDirectorio(const std::string& rutaRaiz) {
    fs::path raiz(rutaRaiz);
    if (!fs::exists(raiz) || !fs::is_directory(raiz)) {
        throw std::invalid_argument("La ruta especificada no existe o no es un directorio válido: " + rutaRaiz);
    }

    cachePerformers.clear();
    cacheAlbums.clear();

    asegurarRegistrosBase();
    inicializarContadores();

    ResumenMinado resumen;

    for (const auto& entrada : fs::recursive_directory_iterator(raiz, fs::directory_options::skip_permission_denied)) {
        if (!entrada.is_regular_file() || !esArchivoMP3(entrada.path())) {
            continue;
        }

        resumen.archivosEscaneados++;

        try {
            fs::path rutaAbsoluta = fs::weakly_canonical(entrada.path());
            std::string rutaStr = rutaAbsoluta.string();

            if (buscador.existeRolaPorRuta(rutaStr)) {
                resumen.cancionesOmitidas++;
                continue;
            }

            MetadatosMP3 meta = lector.leer(rutaStr);

            int idPerformer = obtenerOCrearPerformer(meta.artist);
            int idAlbum = obtenerOCrearAlbum(
                meta.album,
                rutaAbsoluta.parent_path().string(),
                meta.year,
                idPerformer
            );

            int idRola = nextIdRola++;
            Rolas nuevaRola(
                idRola,
                idPerformer,
                idAlbum,
                ID_PERSON_DEFAULT,
                ID_PERFORMER_DEFAULT,
                meta.path,
                meta.title,
                meta.track,
                meta.discNumber,
                meta.year,
                meta.genre,
                meta.comment
            );
            nuevaRola.Crear_Insertar();

            resumen.cancionesInsertadas++;
        } catch (const std::exception& e) {
            resumen.errores++;
            std::cerr << "Error procesando '" << entrada.path().string() << "': " << e.what() << std::endl;
        }
    }

    return resumen;
}

