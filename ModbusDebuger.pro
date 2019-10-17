QT       += core gui
QT       += widgets serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModbusDebuger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp  \
    extserialportmodbusdevice.cpp \
    Global/QextSerialPort/qextserialbase.cpp \
    Global/QextSerialPort/qextserialport.cpp \
    Global/QextSerialPort/qextserialportrecvthread.cpp \
    Global/QextSerialPort/win_qextserialport.cpp \
    Global/algorithm.cpp \
    Global/global.cpp \
    Global/JsonConfig.cpp \
    Global/sysconfig.cpp \
    abstractmodbusdevice.cpp \
    datasreceivethread.cpp \
    databuffer.cpp \
    Global/highprecisiontimer.cpp

HEADERS  += mainwindow.h \
    extserialportmodbusdevice.h \
    mainwindow.cpp.autosave \
    Global/QextSerialPort/qextserialbase.h \
    Global/QextSerialPort/qextserialport.h \
    Global/QextSerialPort/qextserialportrecvthread.h \
    Global/QextSerialPort/win_qextserialport.h \
    Global/algorithm.h \
    Global/global.h \
    Global/JsonConfig.h \
    Global/sysconfig.h \
    abstractmodbusdevice.h \
    modbusmaster_global.h \
    Global/highprecisiontimer.h \
    datasreceivethread.h \
    databuffer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS    += mainwindow.ui

RESOURCES += \
    image.qrc

RC_ICONS = logo.ico
