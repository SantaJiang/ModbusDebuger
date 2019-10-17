#ifndef QEXTSERIALPORTRECVTHREAD_H
#define QEXTSERIALPORTRECVTHREAD_H

#include <QThread>
#include <QTime>
#include <QByteArray>

class QextSerialPort;
class QextSerialPortRecvThread : public QThread
{
    Q_OBJECT
public:
    explicit QextSerialPortRecvThread(QObject *parent = 0);
    explicit QextSerialPortRecvThread(QextSerialPort* pSerialPort,
                                      QObject *parent = 0);
    void setSerialPort(QextSerialPort* pSerialPort);
protected:
    void run() Q_DECL_OVERRIDE;
signals:    
    void readyRead();
    void recvDatas(QTime t, QByteArray datas);
public slots:

private:
    QextSerialPort* m_pSerialPort;
public:
    bool    m_bRunning;
};

#endif // QEXTSERIALPORTRECVTHREAD_H
