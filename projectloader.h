#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include <expected>
#include <vector>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "lightproject.h"

#include "projectloader.h"

namespace ProjectLoader {

std::expected<LightProject, QString> loadProject(const QJsonDocument &json);

std::expected<QJsonDocument, QString> saveProject(const LightProject &val);

} // namespace ProjectLoader


#endif // PROJECTLOADER_H
