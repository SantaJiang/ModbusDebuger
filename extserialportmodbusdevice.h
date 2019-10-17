#ifndef EXTSERIALPORTCANDEVICE_H
#define EXTSERIALPORTCANDEVICE_H

#include "abstractmodbusdevice.h"
#include "./Global/QextSerialPort/qextserialport.h"


class ExtSerialPortModbusDevice : public AbstractModbusDevice
{
    Q_OBJECT
public:
    explicit ExtSerialPortModbusDevice(QObject *parent = 0);
    explicit ExtSerialPortModbusDevice(const QString& spName,
                                 int spBaudRate = 38400,
                                 QObject *parent = 0);
    void setSerialPort(const QString& spName);
    bool setBaudrate(int nBaudrate);
    bool setDataBits(ModbusDataBits dataBits);
    bool setStopBits(ModbusStopBits stopBits);
    bool setParityBits(ModbusParityBits parityBits);


    bool open();
    void close();
    int recvData(char* buf, int maxSize);
    int sendData(const QByteArray &datas);
protected:
    void initDevice();

private:
    QextSerialPort  m_serialPort;
    QString         m_spName;       //串口名称
};

#endif // EXTSERIALPORTCANDEVICE_H
