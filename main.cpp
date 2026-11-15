#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "InterFacadeTablas.hpp"
#include <QUrl>
#include <QStringLiteral>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    #if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
      engine.loadFromModule("AplicacionMusica", "Main");
    #else
      engine.load(QUrl(QStringLiteral("qrc:/AplicacionMusica/Main.qml")));
    #endif

    return QGuiApplication::exec();
}
