#ifndef CONSULTA_BUILDER_HPP
#define CONSULTA_BUILDER_HPP

#include "ConsultaAST.hpp"
#include <vector>
#include <memory>

class ConsultaBuilder {
private:
    ConsultaAST ast;
    std::unique_ptr<GrupoCondiciones> grupoRaiz;
    std::vector<GrupoCondiciones*> pilaGrupos;

public:
    ConsultaBuilder();

    ConsultaBuilder& seleccionarEntidad(const std::string& entidad);
    ConsultaBuilder& agregarCampo(const std::string& entidad, const std::string& columna, const std::string& alias = "");
    ConsultaBuilder& soloDistintos(bool activar = true);

    ConsultaBuilder& donde(const std::string& entidad,
                           const std::string& columna,
                           OperadorComparacion op,
                           ValorParametro valor);

    ConsultaBuilder& dondeEntre(const std::string& entidad,
                                const std::string& columna,
                                ValorParametro min,
                                ValorParametro max);

    ConsultaBuilder& dondeNulo(const std::string& entidad,
                               const std::string& columna,
                               bool esNulo = true);

    ConsultaBuilder& abrirGrupo(OperadorLogico op);
    ConsultaBuilder& cerrarGrupo();

    ConsultaBuilder& ordenarPor(const std::string& entidad,
                                const std::string& columna,
                                DireccionOrden dir = DireccionOrden::ASC);

    ConsultaBuilder& limitar(int limite);

    ConsultaAST construir();
};

#endif // CONSULTA_BUILDER_HPP
