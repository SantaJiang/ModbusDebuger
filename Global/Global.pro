#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T19:52:51
#
#-------------------------------------------------

QT       += widgets

#TARGET = Global
TEMPLATE = lib

DEFINES += GLOBAL_LIBRARY


QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

SOURCES += global.cpp \
    runlog.cpp \
    algorithm.cpp \
    checkboxheaderview.cpp \
    checkboxtablewidget.cpp \
    QextSerialPort/qextserialbase.cpp \
    QextSerialPort/qextserialport.cpp \
    QextSerialPort/win_qextserialport.cpp \
    QextSerialPort/qextserialportrecvthread.cpp \
    iniconfig.cpp \
    JsonConfig.cpp \
    sysconfig.cpp \
    highprecisiontimer.cpp \
    nmcomplexbutton.cpp \
    filterwidget.cpp \
    runlogquerydialog.cpp \
    runlogtablemodel.cpp \
    checkboxheaderviewex.cpp \
    checkablecombobox.cpp

HEADERS += global.h\
        global_global.h \
    runlog.h \
    algorithm.h \
    checkboxheaderview.h \
    checkboxtablewidget.h \
    QextSerialPort/qextserialbase.h \
    QextSerialPort/qextserialport.h \
    QextSerialPort/win_qextserialport.h \
    QextSerialPort/qextserialportrecvthread.h \
    iniconfig.h \
    JsonConfig.h \
    sysconfig.h \
    highprecisiontimer.h \
    nmcomplexbutton.h \
    filterwidget.h \
    runlogquerydialog.h \
    runlogtablemodel.h \
    checkboxheaderviewex.h \
    checkablecombobox.h

CONFIG( debug, debug|release ) {
    TARGET = Globald
    DESTDIR = ../debug/
}else{
    TARGET = Global
    DESTDIR = ../release/
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    resource_global.qrc

FORMS += \
    runlogquerydialog.ui
