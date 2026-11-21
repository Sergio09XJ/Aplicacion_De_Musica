#ifndef CONSULTA_AST_HPP
#define CONSULTA_AST_HPP

#include "TiposCompilador.hpp"
#include "NodoCriterio.hpp"
#include <string>
#include <vector>
#include <memory>
#include <optional>

struct ConsultaAST {
    std::string entidadBase;
    std::vector<CampoSeleccionado> campos;
    std::unique_ptr<NodoCriterio> raizWhere;
    std::vector<Ordenamiento> ordenamientos;
    std::optional<int> limite;
    bool distintos = false;
};

#endif // CONSULTA_AST_HPP
