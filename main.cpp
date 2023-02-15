#include <QGuiApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "dmxcontroller.h"

int main(int argc, char *argv[])
{
    qSetMessagePattern(QStringLiteral("%{time dd.MM.yyyy HH:mm:ss.zzz} "
                                      "["
                                      "%{if-debug}D%{endif}"
                                      "%{if-info}I%{endif}"
                                      "%{if-warning}W%{endif}"
                                      "%{if-critical}C%{endif}"
                                      "%{if-fatal}F%{endif}"
                                      "] "
                                      "%{function}(): "
                                      "%{message}"));

    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app{argc, argv};

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption windowedOption {
        QStringList{"w", "windowed"},
        QCoreApplication::translate("main", "Show in windowed (only dev)")
    };
    parser.addOption(windowedOption);

    if (!parser.parse(app.arguments()))
    {
        qFatal("could not parse arguments!");
        return -1;
    }

    const auto windowed = parser.isSet(windowedOption);

    DmxController controller{&app};
    if (!controller.start() && !windowed)
        return -1;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("__controller", &controller);

    const QUrl url{windowed ? u"qrc:/lightcontrol/main-dev.qml"_qs : u"qrc:/lightcontrol/main.qml"_qs};
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
