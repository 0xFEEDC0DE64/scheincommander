#include "lightproject.h"

#include <QCoreApplication>
#include <QQmlEngine>

namespace {
void registrierDenShit()
{
    qmlRegisterUncreatableMetaObject(hilfe::staticMetaObject, "lightcontrol", 1, 0, "DeviceTypeRegisterType", "lass es du depp");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)
