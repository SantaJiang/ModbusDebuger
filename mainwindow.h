#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QSerialPortInfo>
#include <QList>
#include <QString>
#include "./Global/global.h"


namespace Ui {
class MainWindow;
}
class AbstractModbusDevice;
class DatasReceiveThread;
class DataBuffer;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void InitSerialPortList();
    void InitSerialPortBaudRateList();
    void setDataBuffer(DataBuffer *pBuf);

protected slots:
    void on_pushButton_open_clicked();
    void update();
    void showFrame(const DataInfo& frame);

private:
    Ui::MainWindow *ui;
    void seTimer(bool isTiming);
    void readState();
    void saveState();

private slots:
    void on_pushButton_refreash_clicked();
    void on_pushButton_send_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_close_clicked();
    void on_checkBox_hexShow_clicked(bool checked);
    void on_checkBox_timing_clicked(bool checked);
    void sendData();
    void on_spinBox_timeout_valueChanged(int arg1);
    void on_checkBox_hexSend_clicked(bool checked);

    void on_pushButton_save_clicked();

private:
    QByteArray m_sendDatas;
    QList<QSerialPortInfo> spInfoList;
    QList<qint32> spBaudRateList;
    AbstractModbusDevice* m_pModbusDevice;
    //定义线程实例指针
    DatasReceiveThread* m_pRecvThread;
    //定义数据缓存区实例指针
    DataBuffer *m_pDataBuf;
    bool m_isTiming;
    QTimer *sendTimer;
    QString m_strData;
    QString m_hexData;

    //字符串显示形式QList
    QList<DataInfo> m_lstFrames;
};

#endif // MAINWINDOW_H
