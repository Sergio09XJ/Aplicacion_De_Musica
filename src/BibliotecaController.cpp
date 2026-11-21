
#include "Rolas.hpp"
#include "Albums.hpp"
#include "Persons.hpp"
#include "Groups.hpp"
#include "In_Group.hpp"
#include "Performers.hpp"
#include <QUrl>

#include "BibliotecaController.hpp"
#include <pqxx/pqxx>


std::string BibliotecaController::obtenerConnStrPorDefecto() {
#ifdef __APPLE__
    return "dbname=musicabasedatos user=postgres password=music_dev_password host=localhost port=5432";
#else
#ifdef __APPLE__
    const std::string conn_str = "dbname=musicabasedatos user=postgres password=music_dev_password host=localhost port=5432";
#else
    const std::string conn_str = "dbname=musicabasedatos user=postgres password=music_dev_password host=postgres port=5432";
#endif
#endif
}

BibliotecaController::BibliotecaController(QObject* parent)
    : QObject(parent),
    connStr(obtenerConnStrPorDefecto()),
    buscador(connStr),
    minero(buscador, connStr) {
    try {
        conn = std::make_unique<pqxx::connection>(connStr);
    } catch (const std::exception& e) {
        m_mensajeEstado = QString("Error BD: %1").arg(e.what());
    }
}

BibliotecaController::~BibliotecaController() {
    conn.reset();
}

void BibliotecaController::setMensajeEstado(const QString& msg) {
    if (m_mensajeEstado != msg) {
        m_mensajeEstado = msg;
        emit mensajeEstadoChanged();
    }
}

QStringList BibliotecaController::tablasDisponibles() const {
    QStringList lista;
    for (const auto& t : buscador.obtenerCatalogo().obtenerTablasDisponibles()) {
        lista << QString::fromStdString(t);
    }
    return lista;
}

int BibliotecaController::siguienteId(const std::string& entidad, const std::string& pk) {
    ConsultaBuilder b;
    ConsultaAST ast = b.seleccionarEntidad(entidad)
                          .agregarCampo(entidad, pk, "max_id")
                          .ordenarPor(entidad, pk, DireccionOrden::DESC)
                          .limitar(1)
                          .construir();
    json res = buscador.ejecutarConsulta(ast);
    if (!res.empty() && !res[0]["max_id"].is_null()) {
        return res[0]["max_id"].get<int>() + 1;
    }
    return 1;
}

QVariantList BibliotecaController::jsonAVariantList(const json& arr) {
    QVariantList lista;
    for (const auto& item : arr) {
        QVariantMap mapa;
        for (auto it = item.begin(); it != item.end(); ++it) {
            const QString k = QString::fromStdString(it.key());
            if (it.value().is_null()) mapa[k] = QVariant();
            else if (it.value().is_number_integer()) mapa[k] = it.value().get<int>();
            else if (it.value().is_number_float()) mapa[k] = it.value().get<double>();
            else if (it.value().is_boolean()) mapa[k] = it.value().get<bool>();
            else mapa[k] = QString::fromStdString(it.value().get<std::string>());
        }
        lista.append(mapa);
    }
    return lista;
}

// --- Fase 3: Minero ---
QVariantMap BibliotecaController::importarDirectorio(const QString& rutaUrl) {
    QVariantMap salida;
    try {
        QString rutaLocal = QUrl(rutaUrl).isLocalFile() ? QUrl(rutaUrl).toLocalFile() : rutaUrl;
        ResumenMinado res = minero.minarDirectorio(rutaLocal.toStdString());
        salida["escaneados"] = res.archivosEscaneados;
        salida["insertadas"] = res.cancionesInsertadas;
        salida["omitidas"] = res.cancionesOmitidas;
        salida["errores"] = res.errores;
        setMensajeEstado(QString("Importación lista: %1 nuevas, %2 omitidas.")
                             .arg(res.cancionesInsertadas)
                             .arg(res.cancionesOmitidas));
    } catch (const std::exception& e) {
        salida["error"] = QString::fromStdString(e.what());
        setMensajeEstado(QString("Error al minar: %1").arg(e.what()));
    }
    return salida;
}

// --- Fase 4: Canciones ---
QVariantList BibliotecaController::obtenerCanciones() {
    ConsultaBuilder b;
    ConsultaAST ast = b.seleccionarEntidad("rolas")
                          .agregarCampo("rolas", "id_rola", "id_rola")
                          .agregarCampo("rolas", "title", "title")
                          .agregarCampo("rolas", "track", "track")
                          .agregarCampo("rolas", "disc_number", "disc_number")
                          .agregarCampo("rolas", "year", "year")
                          .agregarCampo("rolas", "genre", "genre")
                          .agregarCampo("rolas", "comment", "comment")
                          .agregarCampo("rolas", "path", "path")
                          .agregarCampo("rolas", "id_performer", "id_performer")
                          .agregarCampo("performers", "name", "artista")
                          .agregarCampo("albums", "name", "album")
                          .ordenarPor("rolas", "id_rola", DireccionOrden::ASC)
                          .construir();
    return jsonAVariantList(buscador.ejecutarConsulta(ast));
}

bool BibliotecaController::editarCancion(int idRola, const QString& title, const QString& track,
                                         int discNumber, int year, const QString& genre,
                                         const QString& comment, int idPerformer) {
    try {
        Rolas r(idRola, idPerformer, 0, 0, 0, "", title.toStdString(),
                track.toStdString(), discNumber, year, genre.toStdString(), comment.toStdString());
        r.Actualizar_Modificar(1);  // id_performer
        r.Actualizar_Modificar(6);  // title
        r.Actualizar_Modificar(7);  // track
        r.Actualizar_Modificar(8);  // disc_number
        r.Actualizar_Modificar(9);  // year
        r.Actualizar_Modificar(10); // genre
        r.Actualizar_Modificar(11); // comment
        setMensajeEstado("Canción actualizada.");
        return true;
    } catch (const std::exception& e) {
        setMensajeEstado(QString("Error editando canción: %1").arg(e.what()));
        return false;
    }
}

bool BibliotecaController::eliminarCancion(int idRola) {
    try {
        Rolas r(idRola);
        r.Eliminar_Borrar();
        setMensajeEstado("Canción eliminada.");
        return true;
    } catch (const std::exception& e) {
        setMensajeEstado(QString("Error eliminando canción: %1").arg(e.what()));
        return false;
    }
}

// --- Fase 4: Álbumes ---
QVariantList BibliotecaController::obtenerAlbumes() {
    ConsultaBuilder b;
    ConsultaAST ast = b.seleccionarEntidad("albums")
                          .agregarCampo("albums", "id_album", "id_album")
                          .agregarCampo("albums", "name", "name")
                          .agregarCampo("albums", "year", "year")
                          .agregarCampo("albums", "path", "path")
                          .agregarCampo("albums", "id_performer", "id_performer")
                          .agregarCampo("performers", "name", "artista")
                          .ordenarPor("albums", "id_album", DireccionOrden::ASC)
                          .construir();
    return jsonAVariantList(buscador.ejecutarConsulta(ast));
}

bool BibliotecaController::editarAlbum(int idAlbum, const QString& name, int year,
                                       const QString& path, int idPerformer) {
    try {
        pqxx::work txn(*conn);
        txn.exec_params("UPDATE albums SET name = $1, year = $2, path = $3, id_performer = $4 WHERE id_album = $5;",
                        name.toStdString(), year, path.toStdString(), idPerformer, idAlbum);
        txn.commit();
        setMensajeEstado("Álbum actualizado.");
        return true;
    } catch (const std::exception& e) {
        setMensajeEstado(QString("Error editando álbum: %1").arg(e.what()));
        return false;
    }
}

bool BibliotecaController::eliminarAlbum(int idAlbum) {
    try {
        Albums a(idAlbum, "", "", 0, "");
        a.Eliminar_Borrar();
        setMensajeEstado("Álbum eliminado.");
        return true;
    } catch (const std::exception& e) {
        setMensajeEstado(QString("Error eliminando álbum: %1").arg(e.what()));
        return false;
    }
}

// --- Fase 4: Personas, Grupos, Integrantes e Intérpretes ---
QVariantList BibliotecaController::obtenerPersonas() {
    ConsultaBuilder b;
    ConsultaAST ast = b.seleccionarEntidad("persons")
                          .ordenarPor("persons", "id_person", DireccionOrden::ASC)
                          .construir();
    return jsonAVariantList(buscador.ejecutarConsulta(ast));
}

bool BibliotecaController::guardarPersona(int idPerson, const QString& stageName, const QString& realName,
                                          const QString& birthDate, const QString& deathDate) {
    try {
        if (idPerson <= 0) {
            int nuevoId = siguienteId("persons", "id_person");
            Persons p(nuevoId, stageName.toStdString(), realName.toStdString(),
                      birthDate.toStdString(), deathDate.toStdString());
            p.Crear_Insertar();
            setMensajeEstado("Persona registrada.");
        } else {
            pqxx::work txn(*conn);
            txn.exec_params("UPDATE persons SET stage_name=$1, real_name=$2, birth_date=$3, death_date=$4 WHERE id_person=$5;",
                            stageName.toStdString(), realName.toStdString(),
                            birthDate.toStdString(), deathDate.toStdString(), idPerson);
            txn.commit();
            setMensajeEstado("Persona actualizada.");
        }
        return true;
    } catch (const std::exception& e) {
        setMensajeEstado(QString("Error guardando persona: %1").arg(e.what()));
        return false;
    }
}

QVariantList BibliotecaController::obtenerGrupos() {
    ConsultaBuilder b;
    ConsultaAST ast = b.seleccionarEntidad("groups")
                          .ordenarPor("groups", "id_group", DireccionOrden::ASC)
                          .construir();
    return jsonAVariantList(buscador.ejecutarConsulta(ast));
}

bool BibliotecaController::guardarGrupo(int idGroup, const QString& name,
                                        const QString& startDate, const QString& endDate) {
    try {
        if (idGroup <= 0) {
            int nuevoId = siguienteId("groups", "id_group");
            Groups g(nuevoId, name.toStdString(), startDate.toStdString(), endDate.toStdString());
            g.Crear_Insertar();
            setMensajeEstado("Grupo registrado.");
        } else {
            pqxx::work txn(*conn);
            txn.exec_params("UPDATE groups SET name=$1, start_date=$2, end_date=$3 WHERE id_group=$4;",
                            name.toStdString(), startDate.toStdString(), endDate.toStdString(), idGroup);
            txn.commit();
            setMensajeEstado("Grupo actualizado.");
        }
        return true;
    } catch (const std::exception& e) {
        setMensajeEstado(QString("Error guardando grupo: %1").arg(e.what()));
        return false;
    }
}

QVariantList BibliotecaController::obtenerIntegrantesGrupo(int idGroup) {
    ConsultaBuilder b;
    ConsultaAST ast = b.seleccionarEntidad("in_group")
                          .agregarCampo("persons", "id_person", "id_person")
                          .agregarCampo("persons", "stage_name", "stage_name")
                          .agregarCampo("persons", "real_name", "real_name")
                          .donde("in_group", "id_group", OperadorComparacion::IGUAL, idGroup)
                          .construir();
    return jsonAVariantList(buscador.ejecutarConsulta(ast));
}

bool BibliotecaController::agregarIntegrante(int idPerson, int idGroup) {
    try {
        In_Group ig(idPerson, idGroup);
        ig.Crear_Insertar();
        setMensajeEstado("Integrante agregado al grupo.");
        return true;
    } catch (const std::exception& e) {
        setMensajeEstado(QString("Error agregando integrante: %1").arg(e.what()));
        return false;
    }
}

bool BibliotecaController::quitarIntegrante(int idPerson, int idGroup) {
    try {
        In_Group ig(idPerson, idGroup);
        ig.Eliminar_Borrar();
        setMensajeEstado("Integrante eliminado del grupo.");
        return true;
    } catch (const std::exception& e) {
        setMensajeEstado(QString("Error quitando integrante: %1").arg(e.what()));
        return false;
    }
}

QVariantList BibliotecaController::obtenerPerformers() {
    ConsultaBuilder b;
    ConsultaAST ast = b.seleccionarEntidad("performers")
                          .agregarCampo("performers", "id_performer", "id_performer")
                          .agregarCampo("performers", "name", "name")
                          .agregarCampo("performers", "id_type", "id_type")
                          .agregarCampo("types", "description", "tipo")
                          .ordenarPor("performers", "id_performer", DireccionOrden::ASC)
                          .construir();
    return jsonAVariantList(buscador.ejecutarConsulta(ast));
}

QVariantList BibliotecaController::obtenerTipos() {
    ConsultaBuilder b;
    ConsultaAST ast = b.seleccionarEntidad("types").construir();
    return jsonAVariantList(buscador.ejecutarConsulta(ast));
}

bool BibliotecaController::registrarPerformer(const QString& name, int idType) {
    try {
        int nuevoId = siguienteId("performers", "id_performer");
        Performers p(nuevoId, idType, name.toStdString(), "");
        p.Crear_Insertar();
        setMensajeEstado("Intérprete registrado.");
        return true;
    } catch (const std::exception& e) {
        setMensajeEstado(QString("Error registrando intérprete: %1").arg(e.what()));
        return false;
    }
}

// --- Fase 5: Compilador de Búsquedas ---
QVariantList BibliotecaController::obtenerCamposDeEntidad(const QString& entidad) {
    QVariantList lista;
    try {
        for (const auto& col : buscador.obtenerCatalogo().obtenerColumnasDeTabla(entidad.toStdString())) {
            if (col.tipo == TipoDatoSQL::BINARIO) continue;
            QVariantMap m;
            m["nombre"] = QString::fromStdString(col.nombre);
            m["tipo"] = (col.tipo == TipoDatoSQL::ENTERO) ? "ENTERO" : "TEXTO";
            lista.append(m);
        }
    } catch (...) {}
    return lista;
}

QVariantMap BibliotecaController::ejecutarBusquedaAvanzada(const QVariantMap& consulta) {
    QVariantMap respuesta;
    try {
        std::string entidadBase = consulta.value("entidadBase").toString().toStdString();
        QVariantList campos = consulta.value("campos").toList();
        QVariantList criterios = consulta.value("criterios").toList();
        QString conector = consulta.value("conector", "AND").toString();

        ConsultaBuilder builder;
        builder.seleccionarEntidad(entidadBase);

        for (const auto& cVar : campos) {
            QVariantMap cm = cVar.toMap();
            builder.agregarCampo(cm["entidad"].toString().toStdString(),
                                 cm["columna"].toString().toStdString());
        }

        if (!criterios.isEmpty()) {
            builder.abrirGrupo(conector == "OR" ? OperadorLogico::OR : OperadorLogico::AND);

            for (const auto& critVar : criterios) {
                QVariantMap crit = critVar.toMap();
                std::string ent = crit["entidad"].toString().toStdString();
                std::string col = crit["columna"].toString().toStdString();
                QString opStr = crit["operador"].toString();
                QString valStr = crit["valor"].toString();

                OperadorComparacion op = OperadorComparacion::IGUAL;
                if (opStr == "!=") op = OperadorComparacion::DIFERENTE;
                else if (opStr == "<") op = OperadorComparacion::MENOR;
                else if (opStr == ">") op = OperadorComparacion::MAYOR;
                else if (opStr == "<=") op = OperadorComparacion::MENOR_IGUAL;
                else if (opStr == ">=") op = OperadorComparacion::MAYOR_IGUAL;
                else if (opStr == "CONTIENE") op = OperadorComparacion::CONTIENE;
                else if (opStr == "EMPIEZA_CON") op = OperadorComparacion::EMPIEZA_CON;
                else if (opStr == "TERMINA_CON") op = OperadorComparacion::TERMINA_CON;

                ColumnaInfo info = buscador.obtenerCatalogo().obtenerColumna(ent, col);
                if (info.tipo == TipoDatoSQL::ENTERO) {
                    builder.donde(ent, col, op, std::stoi(valStr.toStdString()));
                } else {
                    builder.donde(ent, col, op, valStr.toStdString());
                }
            }
            builder.cerrarGrupo();
        }

        ConsultaAST ast = builder.construir();
        ResultadoCompilacion comp = buscador.compilar(ast);
        json filas = buscador.ejecutarConsulta(ast);

        respuesta["sql"] = QString::fromStdString(comp.sql);
        respuesta["filas"] = jsonAVariantList(filas);
        setMensajeEstado(QString("Búsqueda completada: %1 resultados.").arg(filas.size()));
    } catch (const std::exception& e) {
        respuesta["error"] = QString::fromStdString(e.what());
        setMensajeEstado(QString("Error en búsqueda: %1").arg(e.what()));
    }
    return respuesta;
}
