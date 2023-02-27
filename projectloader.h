#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include <expected>

#include <QJsonDocument>
#include <QString>

#include "lightproject.h"

namespace ProjectLoader {

std::expected<LightProject, QString> loadProject(const QJsonDocument &json);

std::expected<QJsonDocument, QString> saveProject(const LightProject &val);

} // namespace ProjectLoader


#endif // PROJECTLOADER_H
