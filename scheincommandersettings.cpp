#include "scheincommandersettings.h"

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
