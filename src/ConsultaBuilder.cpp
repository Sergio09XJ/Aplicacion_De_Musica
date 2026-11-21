#include "ConsultaBuilder.hpp"
#include <stdexcept>

ConsultaBuilder::ConsultaBuilder() {
    grupoRaiz = std::make_unique<GrupoCondiciones>(OperadorLogico::AND);
    pilaGrupos.push_back(grupoRaiz.get());
}

ConsultaBuilder& ConsultaBuilder::seleccionarEntidad(const std::string& entidad) {
    ast.entidadBase = entidad;
    return *this;
}

ConsultaBuilder& ConsultaBuilder::agregarCampo(const std::string& entidad,
                                               const std::string& columna,
                                               const std::string& alias) {
    ast.campos.push_back({entidad, columna, alias});
    return *this;
}

ConsultaBuilder& ConsultaBuilder::soloDistintos(bool activar) {
    ast.distintos = activar;
    return *this;
}

ConsultaBuilder& ConsultaBuilder::donde(const std::string& entidad,
                                        const std::string& columna,
                                        OperadorComparacion op,
                                        ValorParametro valor) {
    auto cond = std::make_unique<CondicionSimple>(entidad, columna, op, std::move(valor));
    pilaGrupos.back()->agregar(std::move(cond));
    return *this;
}

ConsultaBuilder& ConsultaBuilder::dondeEntre(const std::string& entidad,
                                             const std::string& columna,
                                             ValorParametro min,
                                             ValorParametro max) {
    auto cond = std::make_unique<CondicionSimple>(
        entidad, columna, OperadorComparacion::ENTRE, std::move(min), std::move(max));
    pilaGrupos.back()->agregar(std::move(cond));
    return *this;
}

ConsultaBuilder& ConsultaBuilder::dondeNulo(const std::string& entidad,
                                            const std::string& columna,
                                            bool esNulo) {
    OperadorComparacion op = esNulo ? OperadorComparacion::ES_NULO : OperadorComparacion::NO_ES_NULO;
    auto cond = std::make_unique<CondicionSimple>(entidad, columna, op);
    pilaGrupos.back()->agregar(std::move(cond));
    return *this;
}

ConsultaBuilder& ConsultaBuilder::abrirGrupo(OperadorLogico op) {
    auto nuevoGrupo = std::make_unique<GrupoCondiciones>(op);
    GrupoCondiciones* ptr = nuevoGrupo.get();
    pilaGrupos.back()->agregar(std::move(nuevoGrupo));
    pilaGrupos.push_back(ptr);
    return *this;
}

ConsultaBuilder& ConsultaBuilder::cerrarGrupo() {
    if (pilaGrupos.size() <= 1) {
        throw std::logic_error("Error de sintaxis en consulta: No hay grupos abiertos para cerrar.");
    }
    pilaGrupos.pop_back();
    return *this;
}

ConsultaBuilder& ConsultaBuilder::ordenarPor(const std::string& entidad,
                                             const std::string& columna,
                                             DireccionOrden dir) {
    ast.ordenamientos.push_back({entidad, columna, dir});
    return *this;
}

ConsultaBuilder& ConsultaBuilder::limitar(int limite) {
    if (limite <= 0) {
        throw std::invalid_argument("Error semántico: El límite debe ser mayor a 0.");
    }
    ast.limite = limite;
    return *this;
}

ConsultaAST ConsultaBuilder::construir() {
    if (pilaGrupos.size() > 1) {
        throw std::logic_error("Error de sintaxis en consulta: Quedaron grupos de condiciones sin cerrar.");
    }
    if (!grupoRaiz->estaVacio()) {
        ast.raizWhere = std::move(grupoRaiz);
    }
    ConsultaAST resultado = std::move(ast);

    // Reinicia el builder para reutilizarlo si se desea
    ast = ConsultaAST{};
    grupoRaiz = std::make_unique<GrupoCondiciones>(OperadorLogico::AND);
    pilaGrupos.clear();
    pilaGrupos.push_back(grupoRaiz.get());

    return resultado;
}
