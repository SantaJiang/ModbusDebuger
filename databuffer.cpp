#include "databuffer.h"
#include "./Global/global.h"
#include <QDateTime>
#include <QMutex>

DataBuffer::DataBuffer(QObject* parent): QObject(parent)
{

}

DataBuffer::~DataBuffer()
{

}

void DataBuffer::appendFrame(const DataInfo& frame)
{
    m_frameBufLock.lock();
    m_lstFrameBuf.append(frame);
    m_frameBufLock.unlock();
}

void DataBuffer::popAllFrames(QList<DataInfo>& lstFrames)
{
    m_frameBufLock.lock();
    lstFrames = m_lstFrameBuf;
    m_lstFrameBuf.clear();
    m_frameBufLock.unlock();
}
