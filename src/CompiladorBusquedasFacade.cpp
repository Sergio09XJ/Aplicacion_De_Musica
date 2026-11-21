

#include "CompiladorBusquedasFacade.hpp"
#include <iostream>

CompiladorBusquedasFacade::CompiladorBusquedasFacade(const std::string& conn_str)
    : catalogo(), generador(catalogo) {
    try {
        this->conn = std::make_unique<pqxx::connection>(conn_str);
        if (!conn->is_open()) {
            std::cerr << "CompiladorBusquedas: No se pudo abrir la conexión a PostgreSQL." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error crítico en conexión del compilador: " << e.what() << std::endl;
    }
}

CompiladorBusquedasFacade::~CompiladorBusquedasFacade() {
    conn.reset();
}

ResultadoCompilacion CompiladorBusquedasFacade::compilar(const ConsultaAST& ast) const {
    return generador.compilar(ast);
}

json CompiladorBusquedasFacade::ejecutarConsulta(const ConsultaAST& ast) {
    if (!conn || !conn->is_open()) {
        throw std::runtime_error("No hay conexión activa con la base de datos.");
    }

    ResultadoCompilacion compilado = generador.compilar(ast);

    pqxx::params paramsPqxx;
    for (const auto& param : compilado.parametros) {
        std::visit([&paramsPqxx](const auto& val) {
            paramsPqxx.append(val);
        }, param);
    }

    pqxx::work txn(*conn);
    pqxx::result res = txn.exec_params(compilado.sql, paramsPqxx);
    txn.commit();

    json filasJson = json::array();
    for (const auto& row : res) {
        json filaObj = json::object();
        for (size_t i = 0; i < compilado.columnasProyectadas.size(); ++i) {
            const auto& campo = compilado.columnasProyectadas[i];
            const std::string clave = campo.obtenerAliasEfectivo();

            if (row[static_cast<pqxx::row::size_type>(i)].is_null()) {
                filaObj[clave] = nullptr;
                continue;
            }

            ColumnaInfo info = catalogo.obtenerColumna(campo.entidad, campo.columna);
            switch (info.tipo) {
            case TipoDatoSQL::ENTERO:
                filaObj[clave] = row[static_cast<pqxx::row::size_type>(i)].as<int>(0);
                break;
            case TipoDatoSQL::FLOTANTE:
                filaObj[clave] = row[static_cast<pqxx::row::size_type>(i)].as<double>(0.0);
                break;
            case TipoDatoSQL::BOOLEANO:
                filaObj[clave] = row[static_cast<pqxx::row::size_type>(i)].as<bool>(false);
                break;
            case TipoDatoSQL::TEXTO:
            case TipoDatoSQL::BINARIO:
                filaObj[clave] = row[static_cast<pqxx::row::size_type>(i)].as<std::string>("");
                break;
            }
        }
        filasJson.push_back(std::move(filaObj));
    }

    return filasJson;
}

json CompiladorBusquedasFacade::buscarSimple(const std::string& entidad,
                                             const std::string& campo,
                                             OperadorComparacion op,
                                             ValorParametro valor) {
    ConsultaBuilder builder;
    ConsultaAST ast = builder
                          .seleccionarEntidad(entidad)
                          .donde(entidad, campo, op, std::move(valor))
                          .construir();

    return ejecutarConsulta(ast);
}

const CatalogoEsquema& CompiladorBusquedasFacade::obtenerCatalogo() const {
    return catalogo;
}

// --- Integración con Modelo Rolas ---
std::vector<std::unique_ptr<Rolas>> CompiladorBusquedasFacade::consultarRolas(const ConsultaAST& ast) {
    if (ast.entidadBase != "rolas") {
        throw std::invalid_argument("consultarRolas requiere que la entidad base sea 'rolas'.");
    }

    json resultados = ejecutarConsulta(ast);
    std::vector<std::unique_ptr<Rolas>> lista;
    lista.reserve(resultados.size());

    for (const auto& item : resultados) {
        lista.push_back(std::make_unique<Rolas>(
            item.value("id_rola", 0),
            item.value("id_performer", 0),
            item.value("id_album", 0),
            item.value("id_composer", 0),
            item.value("id_original_artist", 0),
            item.value("path", std::string("")),
            item.value("title", std::string("")),
            item.value("track", std::string("")),
            item.value("disc_number", 0),
            item.value("year", 0),
            item.value("genre", std::string("")),
            item.value("comment", std::string(""))
            ));
    }
    return lista;
}


// --- Helpers listos para el Minero (Fase 3) ---
bool CompiladorBusquedasFacade::existeRolaPorRuta(const std::string& path) {
    json res = buscarSimple("rolas", "path", OperadorComparacion::IGUAL, path);
    return !res.empty();
}

std::optional<int> CompiladorBusquedasFacade::buscarIdPerformer(const std::string& nombre) {
    ConsultaBuilder builder;
    ConsultaAST ast = builder
                          .seleccionarEntidad("performers")
                          .agregarCampo("performers", "id_performer", "id_performer")
                          .donde("performers", "name", OperadorComparacion::IGUAL, nombre)
                          .limitar(1)
                          .construir();

    json res = ejecutarConsulta(ast);
    if (!res.empty() && !res[0]["id_performer"].is_null()) {
        return res[0]["id_performer"].get<int>();
    }
    return std::nullopt;
}

std::optional<int> CompiladorBusquedasFacade::buscarIdAlbum(const std::string& nombre, int idPerformer) {
    ConsultaBuilder builder;
    ConsultaAST ast = builder
                          .seleccionarEntidad("albums")
                          .agregarCampo("albums", "id_album", "id_album")
                          .donde("albums", "name", OperadorComparacion::IGUAL, nombre)
                          .donde("albums", "id_performer", OperadorComparacion::IGUAL, idPerformer)
                          .limitar(1)
                          .construir();

    json res = ejecutarConsulta(ast);
    if (!res.empty() && !res[0]["id_album"].is_null()) {
        return res[0]["id_album"].get<int>();
    }
    return std::nullopt;
}

