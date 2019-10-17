#include "qextserialportrecvthread.h"
#include "qextserialport.h"
#include "../highprecisiontimer.h"
#include <Windows.h>
#include <QTime>
#include <QDebug>

QextSerialPortRecvThread::QextSerialPortRecvThread(QObject *parent) :
    QThread(parent)
{
    m_pSerialPort = NULL;
    m_bRunning = false;
}

QextSerialPortRecvThread::QextSerialPortRecvThread(QextSerialPort *pSerialPort,
                                                   QObject *parent) :
    QThread(parent)
{
    m_pSerialPort = pSerialPort;
    m_bRunning = false;
}

void QextSerialPortRecvThread::setSerialPort(QextSerialPort *pSerialPort)
{
    if (!isRunning())
    {
        m_pSerialPort = pSerialPort;
    }
}

void QextSerialPortRecvThread::run()
{
    HighPrecisionTimer timer;
    m_bRunning = true;
    while (m_bRunning)
    {
        if (m_pSerialPort == NULL || !m_pSerialPort->isOpen())
            break;

        qint64 nBytes = m_pSerialPort->bytesAvailable();
        if (nBytes > 0)
        {
            QByteArray datas = m_pSerialPort->readAll();
            if (datas.count() > 0)
                emit recvDatas(QTime::currentTime(), datas);
        }
        timer.msleepEx(1);
    }
}
