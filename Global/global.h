#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QDateTime>

#define FromWStr(wstr) QString::fromStdWString(wstr)

struct DataInfo
{
    QDateTime time;
    QByteArray data;
    bool bSend;
};

struct ModbusDevice
{
    int     deviceType;
    int     deviceIndex;
    int     spBaudRate;
    int     spDataBits;
    int     spStopBits;
    int     spParity;
    bool    isShowHEX;
    bool    isSendHEX;
    bool    isCRC16;
    QString strData;
    QString hexData;

};

#endif // GLOBAL_H
