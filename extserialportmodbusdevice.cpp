#include "extserialportmodbusdevice.h"
#include <QDebug>

ExtSerialPortModbusDevice::ExtSerialPortModbusDevice(QObject *parent)
    : AbstractModbusDevice(parent)
{
    initDevice();
}

ExtSerialPortModbusDevice::ExtSerialPortModbusDevice(const QString& spName,
                                                     int spBaudRate, QObject *parent)
    : AbstractModbusDevice(parent)
{
    m_spName = spName;
    m_nBaudrate = spBaudRate;
    initDevice();
}

void ExtSerialPortModbusDevice::setSerialPort(const QString& spName)
{
    m_spName = spName;
}

bool ExtSerialPortModbusDevice::setBaudrate(int nBaudrate)
{
    if (!AbstractModbusDevice::setBaudrate(nBaudrate))
        return false;

    if (isOpen())
    {
        m_serialPort.setBaudRate2(nBaudrate);
    }
    return true;
}

bool ExtSerialPortModbusDevice::setDataBits(ModbusDataBits dataBits)
{
    if (!AbstractModbusDevice::setDataBits(dataBits))
        return false;

    if (isOpen())
    {
        m_serialPort.setDataBits((DataBitsType)dataBits);
    }
    return true;
}

bool ExtSerialPortModbusDevice::setStopBits(ModbusStopBits stopBits)
{
    if (!AbstractModbusDevice::setStopBits(stopBits))
        return false;

    if (isOpen())
    {
        m_serialPort.setStopBits((StopBitsType)stopBits);
    }
    return true;
}

bool ExtSerialPortModbusDevice::setParityBits(ModbusParityBits parityBits)
{
    if (!AbstractModbusDevice::setParityBits(parityBits))
        return false;

    if (isOpen())
    {
        m_serialPort.setParity((ParityType)parityBits);
    }
    return true;
}

bool ExtSerialPortModbusDevice::open()
{
    close();

    m_serialPort.setPortName(m_spName);
    if (!m_serialPort.open(QIODevice::ReadWrite))
    {
        qWarning()<<QString("serialPort %1 open failed")
                    .arg(m_spName);
        return false;
    }

    m_serialPort.setBaudRate2(m_nBaudrate);
    m_serialPort.setDataBits((DataBitsType)m_nDataBits);
    m_serialPort.setStopBits((StopBitsType)m_nStopBits);
    m_serialPort.setParity((ParityType)m_nParityBits);
    m_serialPort.setFlowControl(FLOW_OFF);

    m_isOpened = true;
    return true;
}

void ExtSerialPortModbusDevice::close()
{
    if (m_serialPort.isOpen())
    {
        m_serialPort.close();
    }
    m_isOpened = false;
}

int ExtSerialPortModbusDevice::recvData(char *buf, int maxSize)
{
    //检查指针buf是否为空

    int nBytes = (int)m_serialPort.bytesAvailable();
    if (nBytes > 0)
    {
        if (m_serialPort.lastError() == E_RECEIVE_PARITY_ERROR)
        {
            m_serialPort.read(buf, qMin(nBytes, maxSize));
            qDebug()<<"E_RECEIVE_PARITY_ERROR";
            return 0;
        }
        return m_serialPort.read(buf, qMin(nBytes, maxSize));
    }

    return 0;
}

int ExtSerialPortModbusDevice::sendData(const QByteArray &datas)
{
    return m_serialPort.write(datas);
}

void ExtSerialPortModbusDevice::initDevice()
{
    m_serialPort.setTimeout(0, 500);
}

