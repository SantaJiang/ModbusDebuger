
#ifndef _QEXTSERIALPORT_H_
#define _QEXTSERIALPORT_H_
#include <QMap>
#include "qextserialportrecvthread.h"

/*POSIX CODE*/
#ifdef _TTY_POSIX_
#include "posix_qextserialport.h"
#define QextBaseType Posix_QextSerialPort

/*MS WINDOWS CODE*/
#else
#include "win_qextserialport.h"
#define QextBaseType Win_QextSerialPort
#endif

class QextSerialPort: public QextBaseType {
    Q_OBJECT
public:
    QextSerialPort();
    QextSerialPort(const QString & name);
    QextSerialPort(PortSettings const& s);
    QextSerialPort(const QString & name, PortSettings const& s);
    QextSerialPort(const QextSerialPort& s);
    QextSerialPort& operator=(const QextSerialPort&);
    virtual ~QextSerialPort();

    void setBaudRate2(int nBaudRate);
private:
    void initExtSerialPort();
private:
    QMap<int, int> m_mapBaudRate;
};

class QextSerialPortWithRecvThread : public QextSerialPort
{
    Q_OBJECT
public:
    QextSerialPortWithRecvThread();
    QextSerialPortWithRecvThread(const QString & name);
    QextSerialPortWithRecvThread(PortSettings const& s);
    QextSerialPortWithRecvThread(const QString & name, PortSettings const& s);
    QextSerialPortWithRecvThread(const QextSerialPortWithRecvThread& Other);
    QextSerialPortWithRecvThread& operator=(const QextSerialPortWithRecvThread& Other);
    virtual ~QextSerialPortWithRecvThread();

    virtual bool open(OpenMode mode=0);
    virtual void close();
signals:
    void recvDatas(QTime t, QByteArray datas);
private:
    void initRecvThread();
private:
    QextSerialPortRecvThread m_recvThread;
};

#endif
