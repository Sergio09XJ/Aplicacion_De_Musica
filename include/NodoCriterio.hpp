#ifndef NODO_CRITERIO_HPP
#define NODO_CRITERIO_HPP

#include "TiposCompilador.hpp"
#include "CatalogoEsquema.hpp"
#include <memory>
#include <vector>
#include <set>
#include <string>
#include <optional>

class NodoCriterio {
public:
    virtual ~NodoCriterio() = default;

    virtual std::string generarSQL(const CatalogoEsquema& catalogo,
                                   std::vector<ValorParametro>& params,
                                   int& contadorParams) const = 0;

    virtual void recolectarTablas(std::set<std::string>& tablas) const = 0;
    virtual std::unique_ptr<NodoCriterio> clonar() const = 0;
};

class CondicionSimple : public NodoCriterio {
private:
    std::string entidad;
    std::string columna;
    OperadorComparacion operador;
    std::optional<ValorParametro> valor;
    std::optional<ValorParametro> valorSecundario;

    void validarSemantica(const CatalogoEsquema& catalogo) const;
    static std::string escaparLike(const std::string& texto);

public:
    CondicionSimple(std::string entidad,
                    std::string columna,
                    OperadorComparacion operador,
                    std::optional<ValorParametro> valor = std::nullopt,
                    std::optional<ValorParametro> valorSecundario = std::nullopt);

    std::string generarSQL(const CatalogoEsquema& catalogo,
                           std::vector<ValorParametro>& params,
                           int& contadorParams) const override;

    void recolectarTablas(std::set<std::string>& tablas) const override;
    std::unique_ptr<NodoCriterio> clonar() const override;
};

class GrupoCondiciones : public NodoCriterio {
private:
    OperadorLogico operadorLogico;
    std::vector<std::unique_ptr<NodoCriterio>> hijos;

public:
    explicit GrupoCondiciones(OperadorLogico op = OperadorLogico::AND);

    void agregar(std::unique_ptr<NodoCriterio> criterio);
    bool estaVacio() const;

    std::string generarSQL(const CatalogoEsquema& catalogo,
                           std::vector<ValorParametro>& params,
                           int& contadorParams) const override;

    void recolectarTablas(std::set<std::string>& tablas) const override;
    std::unique_ptr<NodoCriterio> clonar() const override;
};

#endif // NODO_CRITERIO_HPP
