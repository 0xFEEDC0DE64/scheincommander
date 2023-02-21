#pragma once

#include <QSettings>

class ScheinCommanderSettings : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(QString lastProjectFile READ lastProjectFile WRITE setLastProjectFile NOTIFY lastProjectFileChanged)

public:
    using QSettings::QSettings;

    QString lastProjectFile() const;
    void setLastProjectFile(const QString &lastProjectFile);

signals:
    void lastProjectFileChanged(const QString &lastProjectFile);
};
