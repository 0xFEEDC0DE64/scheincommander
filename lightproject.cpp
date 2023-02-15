#include "lightproject.h"

#include <QCoreApplication>
#include <QQmlEngine>

namespace {
void registerDenShit()
{
    qmlRegisterUncreatableMetaObject(hilfe::staticMetaObject, "com.büro", 1, 0, "DeviceTypeRegisterType", "lass es du depp");
}
}
Q_COREAPP_STARTUP_FUNCTION(registerDenShit)
