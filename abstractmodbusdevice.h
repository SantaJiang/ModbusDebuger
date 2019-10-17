#ifndef ABSTRACTMODBUSDEVICE_H
#define ABSTRACTMODBUSDEVICE_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include "./Global/global.h"
#include "./Global/highprecisiontimer.h"


enum ModbusDataBits
{
    DataBits_5 = 0,
    DataBits_6,
    DataBits_7,
    DataBits_8
};
enum ModbusStopBits
{
    StopBits_1 = 0,
    StopBits_1_5,
    StopBits_2
};
enum ModbusParityBits
{
    ParityNone = 0,
    ParityOdd,
    ParityEven
};

class AbstractModbusDevice : public QObject
{
    Q_OBJECT
public:
    explicit AbstractModbusDevice(QObject *parent = 0);

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isOpen(){return m_isOpened;}

    virtual bool setBaudrate(int nBaudrate);
    virtual bool setDataBits(ModbusDataBits dataBits);
    virtual bool setStopBits(ModbusStopBits stopBits);
    virtual bool setParityBits(ModbusParityBits parityBits);


    virtual int recvData(char* buf, int maxSize) = 0;
    virtual int sendData(const QByteArray &datas) = 0;

protected:
    bool m_isOpened;
    HighPrecisionTimer m_hpTimer;

    int             m_nBaudrate;
    ModbusDataBits  m_nDataBits;
    ModbusStopBits  m_nStopBits;
    ModbusParityBits m_nParityBits;
};

#endif // ABSTRACTMODBUSDEVICE_H
