#pragma once

#include <vector>

#include <QString>
#include <QVector3D>

struct LightTypeConfig
{
    int id;
    QString name;
};

struct LightConfig
{
    int id;
    QString name;
    int lightTypeId;
    int address;
    QVector3D position;
};

struct LightProject
{
    std::vector<LightTypeConfig> lightTypes;
    std::vector<LightConfig> lights;
};
