#include "CatalogoEsquema.hpp"
#include <algorithm>

CatalogoEsquema::CatalogoEsquema() {
    inicializarEsquemaMusica();
}

void CatalogoEsquema::inicializarEsquemaMusica() {
    // 1. Definición exacta de tablas y columnas de musicabasedatos
    tablas["types"] = {
        {"id_type",     {"id_type", TipoDatoSQL::ENTERO, true}},
        {"description", {"description", TipoDatoSQL::TEXTO, false}}
    };

    tablas["performers"] = {
        {"id_performer", {"id_performer", TipoDatoSQL::ENTERO, true}},
        {"id_type",      {"id_type", TipoDatoSQL::ENTERO, false}},
        {"name",         {"name", TipoDatoSQL::TEXTO, false}},
        {"image",        {"image", TipoDatoSQL::BINARIO, false}}
    };

    tablas["persons"] = {
        {"id_person",  {"id_person", TipoDatoSQL::ENTERO, true}},
        {"stage_name", {"stage_name", TipoDatoSQL::TEXTO, false}},
        {"real_name",  {"real_name", TipoDatoSQL::TEXTO, false}},
        {"birth_date", {"birth_date", TipoDatoSQL::TEXTO, false}},
        {"death_date", {"death_date", TipoDatoSQL::TEXTO, false}}
    };

    tablas["groups"] = {
        {"id_group",   {"id_group", TipoDatoSQL::ENTERO, true}},
        {"name",       {"name", TipoDatoSQL::TEXTO, false}},
        {"start_date", {"start_date", TipoDatoSQL::TEXTO, false}},
        {"end_date",   {"end_date", TipoDatoSQL::TEXTO, false}}
    };

    tablas["in_group"] = {
        {"id_person", {"id_person", TipoDatoSQL::ENTERO, true}},
        {"id_group",  {"id_group", TipoDatoSQL::ENTERO, true}}
    };

    tablas["albums"] = {
        {"id_album",     {"id_album", TipoDatoSQL::ENTERO, true}},
        {"path",         {"path", TipoDatoSQL::TEXTO, false}},
        {"name",         {"name", TipoDatoSQL::TEXTO, false}},
        {"year",         {"year", TipoDatoSQL::ENTERO, false}},
        {"id_performer", {"id_performer", TipoDatoSQL::ENTERO, false}},
        {"cover_image",  {"cover_image", TipoDatoSQL::BINARIO, false}}
    };

    tablas["rolas"] = {
        {"id_rola",            {"id_rola", TipoDatoSQL::ENTERO, true}},
        {"id_performer",       {"id_performer", TipoDatoSQL::ENTERO, false}},
        {"id_album",           {"id_album", TipoDatoSQL::ENTERO, false}},
        {"path",               {"path", TipoDatoSQL::TEXTO, false}},
        {"title",              {"title", TipoDatoSQL::TEXTO, false}},
        {"track",              {"track", TipoDatoSQL::TEXTO, false}},
        {"year",               {"year", TipoDatoSQL::ENTERO, false}},
        {"genre",              {"genre", TipoDatoSQL::TEXTO, false}},
        {"id_composer",        {"id_composer", TipoDatoSQL::ENTERO, false}},
        {"id_original_artist", {"id_original_artist", TipoDatoSQL::ENTERO, false}},
        {"disc_number",        {"disc_number", TipoDatoSQL::ENTERO, false}},
        {"comment",            {"comment", TipoDatoSQL::TEXTO, false}}
    };

    // 2. Grafo de relaciones JOIN entre entidades
    relaciones = {
        // Desde rolas
        {"rolas", "albums",     "LEFT JOIN albums ON rolas.id_album = albums.id_album", ""},
        {"rolas", "performers", "LEFT JOIN performers ON rolas.id_performer = performers.id_performer", ""},
        {"rolas", "persons",    "LEFT JOIN persons ON rolas.id_composer = persons.id_person", ""},
        {"rolas", "types",      "LEFT JOIN types ON performers.id_type = types.id_type", "performers"},

        // Desde albums
        {"albums", "performers", "LEFT JOIN performers ON albums.id_performer = performers.id_performer", ""},
        {"albums", "rolas",      "LEFT JOIN rolas ON albums.id_album = rolas.id_album", ""},
        {"albums", "types",      "LEFT JOIN types ON performers.id_type = types.id_type", "performers"},

        // Desde performers
        {"performers", "types",  "LEFT JOIN types ON performers.id_type = types.id_type", ""},
        {"performers", "albums", "LEFT JOIN albums ON performers.id_performer = albums.id_performer", ""},
        {"performers", "rolas",  "LEFT JOIN rolas ON performers.id_performer = rolas.id_performer", ""},

        // Desde persons y groups
        {"persons",  "in_group", "LEFT JOIN in_group ON persons.id_person = in_group.id_person", ""},
        {"persons",  "groups",   "LEFT JOIN groups ON in_group.id_group = groups.id_group", "in_group"},
        {"groups",   "in_group", "LEFT JOIN in_group ON groups.id_group = in_group.id_group", ""},
        {"groups",   "persons",  "LEFT JOIN persons ON in_group.id_person = persons.id_person", "in_group"},
        {"in_group", "persons",  "LEFT JOIN persons ON in_group.id_person = persons.id_person", ""},
        {"in_group", "groups",   "LEFT JOIN groups ON in_group.id_group = groups.id_group", ""}
    };
}

bool CatalogoEsquema::existeTabla(const std::string& tabla) const {
    return tablas.find(tabla) != tablas.end();
}

bool CatalogoEsquema::existeColumna(const std::string& tabla, const std::string& columna) const {
    auto itTabla = tablas.find(tabla);
    if (itTabla == tablas.end()) return false;
    return itTabla->second.find(columna) != itTabla->second.end();
}

ColumnaInfo CatalogoEsquema::obtenerColumna(const std::string& tabla, const std::string& columna) const {
    auto itTabla = tablas.find(tabla);
    if (itTabla == tablas.end()) {
        throw std::invalid_argument("Error semántico: La entidad '" + tabla + "' no existe en el esquema.");
    }
    auto itCol = itTabla->second.find(columna);
    if (itCol == itTabla->second.end()) {
        throw std::invalid_argument("Error semántico: El campo '" + columna + "' no pertenece a '" + tabla + "'.");
    }
    return itCol->second;
}

std::vector<std::string> CatalogoEsquema::obtenerTablasDisponibles() const {
    std::vector<std::string> nombres;
    nombres.reserve(tablas.size());
    for (const auto& par : tablas) {
        nombres.push_back(par.first);
    }
    std::sort(nombres.begin(), nombres.end());
    return nombres;
}

std::vector<ColumnaInfo> CatalogoEsquema::obtenerColumnasDeTabla(const std::string& tabla) const {
    auto it = tablas.find(tabla);
    if (it == tablas.end()) {
        throw std::invalid_argument("Error semántico: La entidad '" + tabla + "' no existe.");
    }
    std::vector<ColumnaInfo> cols;
    cols.reserve(it->second.size());
    for (const auto& par : it->second) {
        cols.push_back(par.second);
    }
    return cols;
}

std::vector<RelacionJoin> CatalogoEsquema::resolverRutaJoin(const std::string& origen, const std::string& destino) const {
    if (origen == destino) return {};

    for (const auto& rel : relaciones) {
        if (rel.tablaOrigen == origen && rel.tablaDestino == destino) {
            std::vector<RelacionJoin> ruta;
            if (!rel.tablaIntermedia.empty()) {
                auto subRuta = resolverRutaJoin(origen, rel.tablaIntermedia);
                ruta.insert(ruta.end(), subRuta.begin(), subRuta.end());
            }
            ruta.push_back(rel);
            return ruta;
        }
    }

    throw std::invalid_argument("Error semántico: No existe una relación JOIN definida entre '" +
                                origen + "' y '" + destino + "'.");
}
