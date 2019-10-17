#ifndef DATABUFFER_H
#define DATABUFFER_H
#include "./Global/global.h"
#include <QDateTime>

class DataBuffer: public QObject
{
    Q_OBJECT
public:
    explicit DataBuffer(QObject* parent  = 0);
    ~DataBuffer();

    //参数使用指针或引用传递，加const
    void appendFrame(const DataInfo& frame);
    void popAllFrames(QList<DataInfo>& lstFrames);

private:
    QList<DataInfo> m_lstFrameBuf;
    QMutex m_frameBufLock;
};

#endif // DATABUFFER_H
