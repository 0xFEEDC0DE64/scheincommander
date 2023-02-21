#include <QGuiApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "dmxcontroller.h"
#include "scheincommandersettings.h"

#define STR(x) #x

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

    qputenv("QT_IM_MODULE", QByteArrayLiteral("qtvirtualkeyboard"));
    qputenv("QSG_RHI_BACKEND", QByteArrayLiteral("opengl"));

    QCoreApplication::setOrganizationDomain("com.büro");
    QCoreApplication::setOrganizationName("büro");
    QCoreApplication::setApplicationName("scheincommander");
    QCoreApplication::setApplicationVersion(STR(CMAKE_PROJECT_VERSION));

    QGuiApplication app{argc, argv};

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("project-file", QCoreApplication::translate("main", "Project file to load."));

    QCommandLineOption windowedOption {
        QStringList{"w", "windowed"},
        QCoreApplication::translate("main", "Show in windowed (only dev)")
    };
    parser.addOption(windowedOption);

    if (!parser.parse(app.arguments()))
    {
        qFatal("could not parse arguments: %s", qPrintable(parser.errorText()));
        return -1;
    }

    const auto windowed = parser.isSet(windowedOption);

    ScheinCommanderSettings settings;

    DmxController controller{settings, &app};

    if (const auto &positionalArguments = parser.positionalArguments(); !positionalArguments.isEmpty())
        controller.loadProject(positionalArguments.first());
    else if (const auto &lastProjectFile = settings.lastProjectFile(); !lastProjectFile.isEmpty())
        controller.loadProject(lastProjectFile);

    if (!controller.start() && !windowed)
        return -1;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("__controller", &controller);
    engine.rootContext()->setContextProperty("__windowed", windowed);

    const QUrl url{u"qrc:/scheincommander/main.qml"_qs};
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
