#include "abstractmodbusdevice.h"
#include <QTime>
#include <QThread>
#include <QDebug>
#include <QMutexLocker>
#include "./Global/highprecisiontimer.h"


AbstractModbusDevice::AbstractModbusDevice(QObject *parent)
    : QObject(parent)
{
    m_isOpened = false;

    m_nBaudrate = 115200;
    m_nDataBits = DataBits_8;
    m_nStopBits = StopBits_1;
    m_nParityBits = ParityNone;
}

bool AbstractModbusDevice::setBaudrate(int nBaudrate)
{
    m_nBaudrate = nBaudrate;
    return true;
}

bool AbstractModbusDevice::setDataBits(ModbusDataBits dataBits)
{
    if ((m_nStopBits == StopBits_2 && dataBits == DataBits_5)
            || (m_nStopBits == StopBits_1_5 && dataBits != DataBits_5))
    {
        return false;
    }

    m_nDataBits = dataBits;
    return true;
}

bool AbstractModbusDevice::setStopBits(ModbusStopBits stopBits)
{
    if ((m_nDataBits == DataBits_5 && stopBits == StopBits_2)
            || (stopBits == StopBits_1_5 && m_nDataBits != DataBits_5))
    {
        return false;
    }
    return true;
}

bool AbstractModbusDevice::setParityBits(ModbusParityBits parityBits)
{
    m_nParityBits = parityBits;
    return true;
}

