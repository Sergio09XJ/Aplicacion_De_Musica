#include "GeneradorSQL.hpp"
#include <sstream>
#include <set>
#include <unordered_set>
#include <stdexcept>

GeneradorSQL::GeneradorSQL(const CatalogoEsquema& catalogo) : catalogo(catalogo) {}

ResultadoCompilacion GeneradorSQL::compilar(const ConsultaAST& ast) const {
    if (ast.entidadBase.empty() || !catalogo.existeTabla(ast.entidadBase)) {
        throw std::invalid_argument("Error semántico: Entidad base inválida o no seleccionada ('" +
                                    ast.entidadBase + "').");
    }

    ResultadoCompilacion resultado;
    std::set<std::string> entidadesInvolucradas;
    entidadesInvolucradas.insert(ast.entidadBase);

    // 1. Resolver columnas a proyectar (SELECT)
    if (ast.campos.empty()) {
        // Por defecto proyecta todos los campos no binarios de la entidad base
        for (const auto& col : catalogo.obtenerColumnasDeTabla(ast.entidadBase)) {
            if (col.tipo != TipoDatoSQL::BINARIO) {
                resultado.columnasProyectadas.push_back({ast.entidadBase, col.nombre, col.nombre});
            }
        }
    } else {
        for (const auto& campo : ast.campos) {
            catalogo.obtenerColumna(campo.entidad, campo.columna);
            entidadesInvolucradas.insert(campo.entidad);
            resultado.columnasProyectadas.push_back(campo);
        }
    }

    // 2. Compilar árbol WHERE y recolectar tablas referenciadas en condiciones
    std::string clausulaWhere;
    int contadorParams = 1;
    if (ast.raizWhere) {
        ast.raizWhere->recolectarTablas(entidadesInvolucradas);
        clausulaWhere = ast.raizWhere->generarSQL(catalogo, resultado.parametros, contadorParams);
    }

    // 3. Validar ORDER BY
    for (const auto& ord : ast.ordenamientos) {
        catalogo.obtenerColumna(ord.entidad, ord.columna);
        entidadesInvolucradas.insert(ord.entidad);
    }

    // 4. Resolver JOINs necesarios sin duplicados
    std::vector<std::string> clausulasJoin;
    std::unordered_set<std::string> tablasUnidas;
    tablasUnidas.insert(ast.entidadBase);

    for (const auto& entidadDestino : entidadesInvolucradas) {
        if (entidadDestino == ast.entidadBase) continue;
        auto ruta = catalogo.resolverRutaJoin(ast.entidadBase, entidadDestino);
        for (const auto& paso : ruta) {
            if (tablasUnidas.insert(paso.tablaDestino).second) {
                clausulasJoin.push_back(paso.clausulaJoin);
            }
        }
    }

    // 5. Ensamblar sentencia SQL final
    std::ostringstream sql;
    sql << "SELECT ";
    if (ast.distintos) {
        sql << "DISTINCT ";
    }

    for (size_t i = 0; i < resultado.columnasProyectadas.size(); ++i) {
        if (i > 0) sql << ", ";
        const auto& c = resultado.columnasProyectadas[i];
        sql << c.entidad << "." << c.columna << " AS " << c.obtenerAliasEfectivo();
    }

    sql << " FROM " << ast.entidadBase;

    for (const auto& joinSql : clausulasJoin) {
        sql << " " << joinSql;
    }

    if (!clausulaWhere.empty()) {
        sql << " WHERE " << clausulaWhere;
    }

    if (!ast.ordenamientos.empty()) {
        sql << " ORDER BY ";
        for (size_t i = 0; i < ast.ordenamientos.size(); ++i) {
            if (i > 0) sql << ", ";
            const auto& ord = ast.ordenamientos[i];
            sql << ord.entidad << "." << ord.columna
                << (ord.direccion == DireccionOrden::ASC ? " ASC" : " DESC");
        }
    }

    if (ast.limite.has_value()) {
        sql << " LIMIT $" << contadorParams++;
        resultado.parametros.push_back(*ast.limite);
    }

    sql << ";";
    resultado.sql = sql.str();
    return resultado;
}
