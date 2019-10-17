#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "extserialportmodbusdevice.h"
#include "datasreceivethread.h"
#include "databuffer.h"
#include "./Global/global.h"
#include "./Global/algorithm.h"
#include "./Global/sysconfig.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QHostAddress>
#include <QFile>
#include <QFileDialog>

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_pModbusDevice = NULL;

    ui->setupUi(this);
    ui->pushButton_send->setEnabled(false);
    ui->textEdit_recv->setReadOnly(true);
    ui->checkBox_addCRC->setEnabled(false);
    ui->checkBox_timing->setEnabled(false);
    ui->spinBox_timing->setEnabled(false);
    InitSerialPortList();
    InitSerialPortBaudRateList();

    m_pRecvThread = new DatasReceiveThread(this);
    m_pDataBuf = new DataBuffer(this);
    m_pRecvThread->setDataBuffer(m_pDataBuf);

    readState();

    // 构造定时器，设置超时为 100 毫秒
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);

    // 构造定时器,定时发送数据
    sendTimer = new QTimer(this);
    connect(sendTimer, SIGNAL(timeout()), this, SLOT(sendData()));



}

MainWindow::~MainWindow()
{
    saveState();
    delete ui;
}

void MainWindow::readState()
{
    SysConfig::getInstance().readConfig();
    ui->comboBox_deviceType->setCurrentText(QString(SysConfig::getInstance().m_ModbusDevice.deviceType));
    ui->comboBox_spBaudRate->setCurrentIndex(SysConfig::getInstance().m_ModbusDevice.spBaudRate);
    ui->comboBox_spDataBits->setCurrentIndex(SysConfig::getInstance().m_ModbusDevice.spDataBits);
    ui->comboBox_spStopBits->setCurrentIndex(SysConfig::getInstance().m_ModbusDevice.spStopBits);
    ui->comboBox_spParity->setCurrentText(QString(SysConfig::getInstance().m_ModbusDevice.spParity));
    if(SysConfig::getInstance().m_ModbusDevice.isSendHEX)
    {
        ui->textEdit_send->append(SysConfig::getInstance().m_ModbusDevice.hexData);
    }
    else
    {
        ui->textEdit_send->append(SysConfig::getInstance().m_ModbusDevice.strData);
    }

    ui->checkBox_hexSend->setChecked(SysConfig::getInstance().m_ModbusDevice.isSendHEX);
    ui->checkBox_hexShow->setChecked(SysConfig::getInstance().m_ModbusDevice.isShowHEX);
    ui->checkBox_addCRC->setChecked(SysConfig::getInstance().m_ModbusDevice.isCRC16);
}

void MainWindow::saveState()
{
    SysConfig& cfg = SysConfig::getInstance();
    cfg.m_ModbusDevice.deviceType = ui->comboBox_deviceType->currentText().toInt();
    cfg.m_ModbusDevice.spBaudRate = ui->comboBox_spBaudRate->currentIndex();
    cfg.m_ModbusDevice.spDataBits = ui->comboBox_spDataBits->currentIndex();
    cfg.m_ModbusDevice.spStopBits = ui->comboBox_spStopBits->currentIndex();
    cfg.m_ModbusDevice.spParity   = ui->comboBox_spParity->currentText().toInt();
    if(ui->checkBox_hexSend->isChecked())
    {
        cfg.m_ModbusDevice.hexData = ui->textEdit_send->toPlainText();
    }
    else
    {
        cfg.m_ModbusDevice.strData = ui->textEdit_send->toPlainText();
    }

    cfg.m_ModbusDevice.isSendHEX  = ui->checkBox_hexSend->isChecked();
    cfg.m_ModbusDevice.isShowHEX  = ui->checkBox_hexShow->isChecked();
    cfg.m_ModbusDevice.isCRC16    = ui->checkBox_addCRC->isChecked();
    SysConfig::getInstance().writeConfig();
}

void MainWindow::update()
{
    Q_ASSERT(m_pDataBuf != NULL);

    QList<DataInfo> lstFrames;
    m_pDataBuf->popAllFrames(lstFrames);
    for( int i=0; i<lstFrames.size(); ++i)
    {
        showFrame(lstFrames[i]);
    }
}

void MainWindow::showFrame(const DataInfo &frame)
{
    QString strText;
    if (ui->checkBox_hexShow->isChecked())
        strText = Algorithm::ByteArray2String(frame.data);
    else
        strText = frame.data.data();
    QString strShowData = QString("[%1] %2-> %3")
            .arg(frame.time.toString("HH:mm:ss.zzz"))
            .arg(frame.bSend?QString("Send"):QString("Receive"))
            .arg(strText);
    ui->textEdit_recv->append(strShowData);
    //m_lstFrames.append(frame);
}

void MainWindow::InitSerialPortList()
{
    spInfoList = QSerialPortInfo::availablePorts();
    ui->comboBox_serialport->clear();
    foreach (QSerialPortInfo spInfo, spInfoList)
    {
        ui->comboBox_serialport->addItem(spInfo.portName());
    }
}

void MainWindow::InitSerialPortBaudRateList()
{
    int curIndex = ui->comboBox_spBaudRate->currentIndex();
    spBaudRateList.clear();
    spBaudRateList<<300<<600<<1200<<2400<<4800<<9600<<12500<<25000<<19200<<38400
                 <<57600<<115200<<256000;

    ui->comboBox_spBaudRate->clear();
    foreach (qint32 baudRate, spBaudRateList)
    {
        ui->comboBox_spBaudRate->addItem(tr("%1bps").arg(baudRate, 6));
    }
    ui->comboBox_spBaudRate->setCurrentIndex(curIndex);
}


void MainWindow::on_pushButton_open_clicked()
{
        int devIndex = ui->comboBox_serialport->currentIndex();
        int btIndex = ui->comboBox_spBaudRate->currentIndex();
        qDebug()<<devIndex;

        if (devIndex < 0)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("提示"));
            msgBox.setText(tr("请你选择串口"));
            msgBox.exec();
            return;
        }
        if (btIndex < 0)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("提示"));
            msgBox.setText(tr("请你选择波特率"));
            msgBox.exec();
            return;
        }
        m_pModbusDevice = new ExtSerialPortModbusDevice(spInfoList[devIndex].portName(),
                                                        spBaudRateList[btIndex]);
        int dataBits = ui->comboBox_spDataBits->currentIndex();
        m_pModbusDevice->setDataBits((ModbusDataBits)dataBits);
        int dtopBits = ui->comboBox_spStopBits->currentIndex();
        m_pModbusDevice->setStopBits((ModbusStopBits)dtopBits);
        int darityBits = ui->comboBox_spParity->currentIndex();
        m_pModbusDevice->setParityBits((ModbusParityBits)darityBits);

    if(m_pModbusDevice->open())
    {
        ui->spinBox_timing->setEnabled(false);
        ui->pushButton_send->setEnabled(true);
        ui->label_indicatorLight->setPixmap(QPixmap(":/png/Image/point_green.png"));
        ui->label_indicatorLight->setScaledContents(true);
        ui->checkBox_addCRC->setEnabled(true);
        ui->checkBox_timing->setEnabled(true);
        ui->pushButton_refreash->setEnabled(false);
        ui->spinBox_timing->setEnabled(true);
        ui->groupBox->setEnabled(false);
        ui->pushButton_open->setEnabled(false);
        //启动接收线程
        m_pRecvThread->setModbusDevice(m_pModbusDevice);
        m_pRecvThread->start();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("提示"));
        msgBox.setText(tr("设备打开失败，请检查串口是否被占用"));
        msgBox.exec();
        return;
    }
}

void MainWindow::on_pushButton_refreash_clicked()
{
    InitSerialPortList();
    InitSerialPortBaudRateList();
}

void MainWindow::seTimer(bool isTiming)
{
    int time = ui->spinBox_timing->value();
    if(isTiming)
    {
        sendTimer->start(time);
    }
    else
    {
        sendTimer->stop();
    }
}

void MainWindow::on_pushButton_send_clicked()
{
    sendData();
}

void MainWindow::sendData()
{
    //1、从控件读取输入
    QString strText = ui->textEdit_send->toPlainText();
    //2、根据是否16进制发送转换数据
    if(ui->checkBox_hexSend->isChecked())
    {
        m_sendDatas = Algorithm::String2ByteArray(strText);
    }
    else
    {
        QStringList Mylist = strText.split(" ");
        QStringList Newlist;
        for(int i = 0; i<Mylist.size();++i)
        {
            int a = Mylist.at(i).toInt(Q_NULLPTR,16);

            QChar c = QChar(a);
            Newlist.append(c);
        }
        QString show = Newlist.join("");
        m_sendDatas = show.toLatin1();
    }
    //3、添加CRC
    if (ui->checkBox_addCRC->isChecked())
        Algorithm::AppendCRC16(m_sendDatas);
    //4、发送
    m_pModbusDevice->sendData(m_sendDatas);

    //5、显示和存储
    DataInfo info;
    info.bSend = true;
    info.time = QDateTime::currentDateTime();
    info.data = m_sendDatas;
    showFrame(info);
    m_lstFrames.append(info);
}

void MainWindow::on_pushButton_clear_clicked()
{
    m_lstFrames.clear();
    ui->textEdit_recv->clear();
}

void MainWindow::on_pushButton_close_clicked()
{
    if(ui->checkBox_timing->isChecked())
    {
        ui->checkBox_timing->setChecked(false);
        m_isTiming = false;
        seTimer(m_isTiming);
    }
    //关闭线程
    m_pRecvThread->m_bRunning = false;
    m_pRecvThread->wait();
    //关闭并且释放通信设备
    if (m_pModbusDevice != NULL)
    {
        m_pModbusDevice->close();
        delete m_pModbusDevice;
        m_pModbusDevice = NULL;
    }
    ui->pushButton_send->setEnabled(false);
    ui->label_indicatorLight->setPixmap(QPixmap(":/png/Image/point_red.png"));
    ui->label_indicatorLight->setScaledContents(true);
    ui->checkBox_addCRC->setEnabled(false);
    ui->checkBox_timing->setEnabled(false);
    ui->pushButton_refreash->setEnabled(true);
    ui->spinBox_timing->setEnabled(false);
    ui->groupBox->setEnabled(true);
    ui->pushButton_open->setEnabled(true);
}

/**
 * @brief 16进制数据转ASCII字符数据
 * @param checked
 */
void MainWindow::on_checkBox_hexShow_clicked(bool checked)
{
    ui->textEdit_recv->clear();
}

void MainWindow::on_checkBox_hexSend_clicked(bool checked)
{
    if(checked)
    {
        m_strData =  ui->textEdit_send->toPlainText();
        ui->textEdit_send->clear();
        ui->textEdit_send->append(m_hexData);
    }
    else
    {
        m_hexData =  ui->textEdit_send->toPlainText();
        ui->textEdit_send->clear();
        ui->textEdit_send->append(m_strData);
    }
}


void MainWindow::on_checkBox_timing_clicked(bool checked)
{
    if(!checked)
    {
        m_isTiming = false;
        seTimer(m_isTiming);
        ui->spinBox_timing->setEnabled(true);
    }
    else
    {
        m_isTiming = true;
        seTimer(m_isTiming);
        ui->spinBox_timing->setEnabled(false);
    }
}

void MainWindow::on_spinBox_timeout_valueChanged(int arg1)
{
    Q_ASSERT(m_pRecvThread != NULL);
    m_pRecvThread->m_nTimeoutMS = arg1;
}

void MainWindow::on_pushButton_save_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Data"), "",
                                                    tr("Data Files (*.txt)"));
    if (filePath.isNull())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::information(this, QString::fromStdWString(L"提示"),
                                 QString::fromStdWString(L"无法创建文件"));
        return;
    }
    QString data = ui->textEdit_recv->toPlainText();
    file.write(data.toLatin1());

}
