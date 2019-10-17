#include "sysconfig.h"
#include "JsonConfig.h"

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

SysConfig::SysConfig()
{
    m_strPath = QDir::currentPath();
}

SysConfig::~SysConfig()
{

}
SysConfig& SysConfig::getInstance()
{
    static JsonConfig cfg;
    return cfg;
}

void SysConfig::setPath(const QString &strPath)
{
    m_strPath = strPath;
}
