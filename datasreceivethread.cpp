#include "datasreceivethread.h"
#include "abstractmodbusdevice.h"
#include "databuffer.h"
#include "./Global/algorithm.h"
#include "./Global/global.h"

DatasReceiveThread::DatasReceiveThread(QObject *parent) :
    QThread(parent)
{
    n_maxSize = 0;
    m_pModbusDevice = NULL;
    m_pDataBuf = NULL;
    m_nTimeoutMS = 10;
}

DatasReceiveThread::~DatasReceiveThread()
{

}

void DatasReceiveThread::setModbusDevice(AbstractModbusDevice *pDevice)
{
    m_pModbusDevice = pDevice;
}

void DatasReceiveThread::setDataBuffer(DataBuffer *pBuf)
{
    m_pDataBuf = pBuf;
}

void DatasReceiveThread::run()
{
    DataInfo info;
    info.bSend = false;
    //接收数据
    m_bRunning = true;
    char buf[1024] = {0};
    while (m_bRunning)
    {
        int temp = m_pModbusDevice->recvData(buf, 1024);
        if (0 == temp)
        {
            //加判断，跟前一包的数据的时间对比，大于一定间隔直接发送，小于合并发送
            if (info.data.count() > 0
                    && info.time.msecsTo(QDateTime::currentDateTime()) > m_nTimeoutMS)
            {
                m_pDataBuf->appendFrame(info);
                info.data.clear();
            }
            msleep(1);
            continue;
        }

        if (info.data.count() == 0)
        {
            info.time = QDateTime::currentDateTime();
        }
        info.data.append(QByteArray(buf, temp));

    }
    m_bRunning = false;
}

