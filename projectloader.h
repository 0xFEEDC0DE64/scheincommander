#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include "expected.hpp"

#include <QJsonDocument>
#include <QString>

#include "lightproject.h"


namespace std {
    template <typename... T>
    using expected = tl::expected<T...>;
}





namespace ProjectLoader {

std::expected<LightProject, QString> loadProject(const QJsonDocument &json);

std::expected<QJsonDocument, QString> saveProject(const LightProject &val);

} // namespace ProjectLoader


#endif // PROJECTLOADER_H
