#include "NodoCriterio.hpp"
#include <stdexcept>
#include <sstream>

// --- CondicionSimple ---

CondicionSimple::CondicionSimple(std::string entidad,
                                 std::string columna,
                                 OperadorComparacion operador,
                                 std::optional<ValorParametro> valor,
                                 std::optional<ValorParametro> valorSecundario)
    : entidad(std::move(entidad)),
    columna(std::move(columna)),
    operador(operador),
    valor(std::move(valor)),
    valorSecundario(std::move(valorSecundario)) {}

std::string CondicionSimple::escaparLike(const std::string& texto) {
    std::string escapado;
    escapado.reserve(texto.size());
    for (char c : texto) {
        if (c == '%' || c == '_' || c == '\\') {
            escapado += '\\';
        }
        escapado += c;
    }
    return escapado;
}

void CondicionSimple::validarSemantica(const CatalogoEsquema& catalogo) const {
    ColumnaInfo info = catalogo.obtenerColumna(entidad, columna);

    if (operador == OperadorComparacion::ES_NULO || operador == OperadorComparacion::NO_ES_NULO) {
        return;
    }

    if (!valor.has_value()) {
        throw std::invalid_argument("Error semántico: El operador en '" + entidad + "." + columna + "' requiere un valor.");
    }

    if (operador == OperadorComparacion::ENTRE && !valorSecundario.has_value()) {
        throw std::invalid_argument("Error semántico: El operador ENTRE requiere dos valores (mínimo y máximo).");
    }

    bool esOperadorTexto = (operador == OperadorComparacion::CONTIENE ||
                            operador == OperadorComparacion::EMPIEZA_CON ||
                            operador == OperadorComparacion::TERMINA_CON ||
                            operador == OperadorComparacion::LIKE);

    if (esOperadorTexto && info.tipo != TipoDatoSQL::TEXTO) {
        throw std::invalid_argument("Error semántico: Los operadores de patrón de texto solo aplican a columnas TEXT ('" +
                                    entidad + "." + columna + "').");
    }

    if (info.tipo == TipoDatoSQL::ENTERO && !std::holds_alternative<int>(*valor)) {
        throw std::invalid_argument("Error semántico: Se esperaba un entero para '" + entidad + "." + columna + "'.");
    }

    if (info.tipo == TipoDatoSQL::TEXTO && !std::holds_alternative<std::string>(*valor)) {
        throw std::invalid_argument("Error semántico: Se esperaba una cadena de texto para '" + entidad + "." + columna + "'.");
    }
}

std::string CondicionSimple::generarSQL(const CatalogoEsquema& catalogo,
                                        std::vector<ValorParametro>& params,
                                        int& contadorParams) const {
    validarSemantica(catalogo);

    const std::string colCalificada = entidad + "." + columna;

    switch (operador) {
    case OperadorComparacion::ES_NULO:
        return colCalificada + " IS NULL";
    case OperadorComparacion::NO_ES_NULO:
        return colCalificada + " IS NOT NULL";

    case OperadorComparacion::IGUAL:
        params.push_back(*valor);
        return colCalificada + " = $" + std::to_string(contadorParams++);
    case OperadorComparacion::DIFERENTE:
        params.push_back(*valor);
        return colCalificada + " != $" + std::to_string(contadorParams++);
    case OperadorComparacion::MENOR:
        params.push_back(*valor);
        return colCalificada + " < $" + std::to_string(contadorParams++);
    case OperadorComparacion::MAYOR:
        params.push_back(*valor);
        return colCalificada + " > $" + std::to_string(contadorParams++);
    case OperadorComparacion::MENOR_IGUAL:
        params.push_back(*valor);
        return colCalificada + " <= $" + std::to_string(contadorParams++);
    case OperadorComparacion::MAYOR_IGUAL:
        params.push_back(*valor);
        return colCalificada + " >= $" + std::to_string(contadorParams++);

    case OperadorComparacion::CONTIENE: {
        std::string patron = "%" + escaparLike(std::get<std::string>(*valor)) + "%";
        params.push_back(patron);
        return colCalificada + " ILIKE $" + std::to_string(contadorParams++);
    }
    case OperadorComparacion::EMPIEZA_CON: {
        std::string patron = escaparLike(std::get<std::string>(*valor)) + "%";
        params.push_back(patron);
        return colCalificada + " ILIKE $" + std::to_string(contadorParams++);
    }
    case OperadorComparacion::TERMINA_CON: {
        std::string patron = "%" + escaparLike(std::get<std::string>(*valor));
        params.push_back(patron);
        return colCalificada + " ILIKE $" + std::to_string(contadorParams++);
    }
    case OperadorComparacion::LIKE: {
        params.push_back(std::get<std::string>(*valor));
        return colCalificada + " ILIKE $" + std::to_string(contadorParams++);
    }
    case OperadorComparacion::ENTRE: {
        params.push_back(*valor);
        std::string p1 = "$" + std::to_string(contadorParams++);
        params.push_back(*valorSecundario);
        std::string p2 = "$" + std::to_string(contadorParams++);
        return colCalificada + " BETWEEN " + p1 + " AND " + p2;
    }
    }
    throw std::logic_error("Operador de comparación no soportado.");
}

void CondicionSimple::recolectarTablas(std::set<std::string>& tablas) const {
    tablas.insert(entidad);
}

std::unique_ptr<NodoCriterio> CondicionSimple::clonar() const {
    return std::make_unique<CondicionSimple>(entidad, columna, operador, valor, valorSecundario);
}

// --- GrupoCondiciones ---

GrupoCondiciones::GrupoCondiciones(OperadorLogico op) : operadorLogico(op) {}

void GrupoCondiciones::agregar(std::unique_ptr<NodoCriterio> criterio) {
    if (criterio) {
        hijos.push_back(std::move(criterio));
    }
}

bool GrupoCondiciones::estaVacio() const {
    return hijos.empty();
}

std::string GrupoCondiciones::generarSQL(const CatalogoEsquema& catalogo,
                                         std::vector<ValorParametro>& params,
                                         int& contadorParams) const {
    std::vector<std::string> fragmentos;
    for (const auto& hijo : hijos) {
        std::string sqlHijo = hijo->generarSQL(catalogo, params, contadorParams);
        if (!sqlHijo.empty()) {
            fragmentos.push_back(std::move(sqlHijo));
        }
    }

    if (fragmentos.empty()) return "";
    if (fragmentos.size() == 1) return fragmentos[0];

    const std::string conector = (operadorLogico == OperadorLogico::AND) ? " AND " : " OR ";
    std::ostringstream oss;
    oss << "(";
    for (size_t i = 0; i < fragmentos.size(); ++i) {
        if (i > 0) oss << conector;
        oss << fragmentos[i];
    }
    oss << ")";
    return oss.str();
}

void GrupoCondiciones::recolectarTablas(std::set<std::string>& tablas) const {
    for (const auto& hijo : hijos) {
        hijo->recolectarTablas(tablas);
    }
}

std::unique_ptr<NodoCriterio> GrupoCondiciones::clonar() const {
    auto copia = std::make_unique<GrupoCondiciones>(operadorLogico);
    for (const auto& hijo : hijos) {
        copia->agregar(hijo->clonar());
    }
    return copia;
}
