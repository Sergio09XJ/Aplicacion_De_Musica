#ifndef TIPOS_COMPILADOR_HPP
#define TIPOS_COMPILADOR_HPP

#include <string>
#include <vector>
#include <variant>
#include <optional>

using ValorParametro = std::variant<int, double, std::string, bool>;

enum class OperadorLogico {
    AND,
    OR
};

enum class OperadorComparacion {
    IGUAL,
    DIFERENTE,
    MENOR,
    MAYOR,
    MENOR_IGUAL,
    MAYOR_IGUAL,
    CONTIENE,
    EMPIEZA_CON,
    TERMINA_CON,
    LIKE,
    ENTRE,
    ES_NULO,
    NO_ES_NULO
};

enum class TipoDatoSQL {
    ENTERO,
    FLOTANTE,
    TEXTO,
    BOOLEANO,
    BINARIO
};

enum class DireccionOrden {
    ASC,
    DESC
};

struct ColumnaInfo {
    std::string nombre;
    TipoDatoSQL tipo;
    bool esLlavePrimaria = false;
};

struct RelacionJoin {
    std::string tablaOrigen;
    std::string tablaDestino;
    std::string clausulaJoin;
    std::string tablaIntermedia = "";
};

struct CampoSeleccionado {
    std::string entidad;
    std::string columna;
    std::string alias = "";

    std::string obtenerAliasEfectivo() const {
        return alias.empty() ? (entidad + "_" + columna) : alias;
    }
};

struct Ordenamiento {
    std::string entidad;
    std::string columna;
    DireccionOrden direccion = DireccionOrden::ASC;
};

struct ResultadoCompilacion {
    std::string sql;
    std::vector<ValorParametro> parametros;
    std::vector<CampoSeleccionado> columnasProyectadas;
};

#endif // TIPOS_COMPILADOR_HPP
