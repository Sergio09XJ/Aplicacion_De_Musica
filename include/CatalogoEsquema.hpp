#ifndef CATALOGO_ESQUEMA_HPP
#define CATALOGO_ESQUEMA_HPP

#include "TiposCompilador.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

class CatalogoEsquema {

private:
    std::unordered_map<std::string, std::unordered_map<std::string, ColumnaInfo>> tablas;
    std::vector<RelacionJoin> relaciones;

    void inicializarEsquemaMusica();

public:
    CatalogoEsquema();

    bool existeTabla(const std::string& tabla) const;
    bool existeColumna(const std::string& tabla, const std::string& columna) const;
    ColumnaInfo obtenerColumna(const std::string& tabla, const std::string& columna) const;

    std::vector<std::string> obtenerTablasDisponibles() const;
    std::vector<ColumnaInfo> obtenerColumnasDeTabla(const std::string& tabla) const;
    std::vector<RelacionJoin> resolverRutaJoin(const std::string& origen, const std::string& destino) const;
};

#endif // CATALOGO_ESQUEMA_HPP
