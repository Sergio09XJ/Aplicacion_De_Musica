#ifndef GENERADOR_SQL_HPP
#define GENERADOR_SQL_HPP

#include "CatalogoEsquema.hpp"
#include "ConsultaAST.hpp"

class GeneradorSQL {
private:
    const CatalogoEsquema& catalogo;

public:
    explicit GeneradorSQL(const CatalogoEsquema& catalogo);
    ResultadoCompilacion compilar(const ConsultaAST& ast) const;
};

#endif // GENERADOR_SQL_HPP
