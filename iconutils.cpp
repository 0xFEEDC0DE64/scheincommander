#include "iconutils.h"

QUrl getIconUrl(QStringView name)
{
    return QUrl{QString{"qrc:/scheincommander/icons/%0.png"}.arg(name)};
}
