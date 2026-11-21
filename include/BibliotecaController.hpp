#ifndef BIBLIOTECA_CONTROLLER_HPP
#define BIBLIOTECA_CONTROLLER_HPP

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QVariantMap>
#include <memory>
#include <pqxx/pqxx>

#include "CompiladorBusquedasFacade.hpp"
#include "Minero.hpp"

class BibliotecaController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString mensajeEstado READ mensajeEstado NOTIFY mensajeEstadoChanged)
    Q_PROPERTY(QStringList tablasDisponibles READ tablasDisponibles CONSTANT)

private:
    std::string connStr;
    CompiladorBusquedasFacade buscador;
    Minero minero;
    std::unique_ptr<pqxx::connection> conn;
    QString m_mensajeEstado = "Listo";

    static std::string obtenerConnStrPorDefecto();
    void setMensajeEstado(const QString& msg);
    int siguienteId(const std::string& entidad, const std::string& pk);
    static QVariantList jsonAVariantList(const json& arr);

public:
    explicit BibliotecaController(QObject* parent = nullptr);
    ~BibliotecaController() override;

    QString mensajeEstado() const { return m_mensajeEstado; }
    QStringList tablasDisponibles() const;

    // Fase 3: Minero
    Q_INVOKABLE QVariantMap importarDirectorio(const QString& rutaUrl);

    // Fase 4: Canciones (Rolas)
    Q_INVOKABLE QVariantList obtenerCanciones();
    Q_INVOKABLE bool editarCancion(int idRola, const QString& title, const QString& track,
                                   int discNumber, int year, const QString& genre,
                                   const QString& comment, int idPerformer);
    Q_INVOKABLE bool eliminarCancion(int idRola);

    // Fase 4: Álbumes (Albums)
    Q_INVOKABLE QVariantList obtenerAlbumes();
    Q_INVOKABLE bool editarAlbum(int idAlbum, const QString& name, int year,
                                 const QString& path, int idPerformer);
    Q_INVOKABLE bool eliminarAlbum(int idAlbum);

    // Fase 4: Personas, Grupos, Integrantes e Intérpretes
    Q_INVOKABLE QVariantList obtenerPersonas();
    Q_INVOKABLE bool guardarPersona(int idPerson, const QString& stageName, const QString& realName,
                                    const QString& birthDate, const QString& deathDate);

    Q_INVOKABLE QVariantList obtenerGrupos();
    Q_INVOKABLE bool guardarGrupo(int idGroup, const QString& name,
                                  const QString& startDate, const QString& endDate);

    Q_INVOKABLE QVariantList obtenerIntegrantesGrupo(int idGroup);
    Q_INVOKABLE bool agregarIntegrante(int idPerson, int idGroup);
    Q_INVOKABLE bool quitarIntegrante(int idPerson, int idGroup);

    Q_INVOKABLE QVariantList obtenerPerformers();
    Q_INVOKABLE QVariantList obtenerTipos();
    Q_INVOKABLE bool registrarPerformer(const QString& name, int idType);

    // Fase 5: Compilador de Búsquedas
    Q_INVOKABLE QVariantList obtenerCamposDeEntidad(const QString& entidad);
    Q_INVOKABLE QVariantMap ejecutarBusquedaAvanzada(const QVariantMap& consulta);

signals:
    void mensajeEstadoChanged();
};

#endif // BIBLIOTECA_CONTROLLER_HPP
