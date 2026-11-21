#ifndef COMPILADOR_BUSQUEDAS_FACADE_HPP
#define COMPILADOR_BUSQUEDAS_FACADE_HPP

#include "CatalogoEsquema.hpp"
#include "GeneradorSQL.hpp"
#include "ConsultaBuilder.hpp"
#include "Rolas.hpp"
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <memory>
#include <vector>
#include <optional>

using json = nlohmann::json;

class CompiladorBusquedasFacade {
private:
    CatalogoEsquema catalogo;
    GeneradorSQL generador;
    std::unique_ptr<pqxx::connection> conn;

public:
    explicit CompiladorBusquedasFacade(
        const std::string& conn_str = "dbname=musicabasedatos user=postgres password=music_dev_password host=postgres port=5432");
    ~CompiladorBusquedasFacade();

    // Compilación pura (útil para pruebas unitarias o vista previa en UI)
    ResultadoCompilacion compilar(const ConsultaAST& ast) const;

    // Ejecución parametrizada contra PostgreSQL -> JSON para la interfaz
    json ejecutarConsulta(const ConsultaAST& ast);
    json buscarSimple(const std::string& entidad,
                      const std::string& campo,
                      OperadorComparacion op,
                      ValorParametro valor);

    const CatalogoEsquema& obtenerCatalogo() const;

    // --- Puntos de integración con el Modelo y el Minero (Fase 3) ---
    std::vector<std::unique_ptr<Rolas>> consultarRolas(const ConsultaAST& ast);
    bool existeRolaPorRuta(const std::string& path);
    std::optional<int> buscarIdPerformer(const std::string& nombre);
    std::optional<int> buscarIdAlbum(const std::string& nombre, int idPerformer);

};

#endif // COMPILADOR_BUSQUEDAS_FACADE_HPP