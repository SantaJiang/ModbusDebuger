#ifndef SYSCONFIG_H
#define SYSCONFIG_H
#include <QString>
#include "global.h"



class SysConfig
{
protected:
    SysConfig();
public:
    virtual ~SysConfig();
    static SysConfig& getInstance();
    void setPath(const QString& strPath);
    virtual void readConfig() = 0;
    virtual void writeConfig() = 0;
public:
    ModbusDevice m_ModbusDevice;
    DataInfo m_DataInfo;
protected:
    QString m_strPath;
};


#endif // SYSCONFIG_H
