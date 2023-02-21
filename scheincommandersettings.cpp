#include "scheincommandersettings.h"

#include <QCoreApplication>
#include <QQmlEngine>

namespace {
const char KEY_lastProjectFile[] = "lastProjectFile";
}

QString ScheinCommanderSettings::lastProjectFile() const
{
    return value(KEY_lastProjectFile).toString();
}

void ScheinCommanderSettings::setLastProjectFile(const QString &lastProjectFile)
{
    setValue(KEY_lastProjectFile, lastProjectFile);
    emit lastProjectFileChanged(lastProjectFile);
}

namespace {
void registrierDenShit()
{
    qmlRegisterType<ScheinCommanderSettings>("scheincommander", 1, 0, "ScheinCommanderSettings");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)

