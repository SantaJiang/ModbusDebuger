#include "JsonConfig.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>



JsonConfig::JsonConfig()
{

}

JsonConfig::~JsonConfig()
{

}

void JsonConfig::readConfig()
{
    QFile file(m_strPath+"//Config.json");
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return;
	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	if (!doc.isObject())
		return;
	QJsonObject objCfg = doc.object();

    //读配置项
    QJsonValue valDB = objCfg.value("ModbusDevice");
    if (valDB.isObject())
    {
        QJsonObject objDB = valDB.toObject();
        m_ModbusDevice.spBaudRate = objDB["Rate"].toInt();
        m_ModbusDevice.spDataBits = objDB["DataBits"].toInt();
        m_ModbusDevice.spStopBits = objDB["StopBits"].toInt();
        m_ModbusDevice.spParity = objDB["Parity"].toInt();
        m_ModbusDevice.strData = objDB["strData"].toString();
        m_ModbusDevice.hexData = objDB["hexData"].toString();
        m_ModbusDevice.isShowHEX = objDB["isShowHEX"].toBool();
        m_ModbusDevice.isCRC16 = objDB["isCRC16"].toBool();
    }
}

void JsonConfig::writeConfig()
{
	QJsonObject objCfg;
    QJsonObject objMD;
    objMD.insert("Rate", QJsonValue(m_ModbusDevice.spBaudRate));
    objMD.insert("DataBits", QJsonValue(m_ModbusDevice.spDataBits));
    objMD.insert("StopBits", QJsonValue(m_ModbusDevice.spStopBits));
    objMD.insert("Parity", QJsonValue(m_ModbusDevice.spParity));
    objMD.insert("strData", QJsonValue(m_ModbusDevice.strData));
    objMD.insert("hexData", QJsonValue(m_ModbusDevice.hexData));
    objMD.insert("isShowHEX", QJsonValue(m_ModbusDevice.isShowHEX));
    objMD.insert("isCRC16", QJsonValue(m_ModbusDevice.isCRC16));

    objCfg.insert("ModbusDevice", QJsonValue(objMD));

	//保存到json文件
	QJsonDocument doc(objCfg);
    QFile file(m_strPath+"//Config.json");
	if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        return;
    file.write(doc.toJson());
}
