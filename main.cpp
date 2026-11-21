#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "BibliotecaController.hpp"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    BibliotecaController controller;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("biblioteca", &controller);

    const QUrl url(QStringLiteral("qrc:/AplicacionMusica/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}
