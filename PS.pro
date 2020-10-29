#-------------------------------------------------
#
# Project created by QtCreator 2020-10-21T16:37:04
#
#-------------------------------------------------

QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    amds.cpp \
    wheeltext.cpp \
    newbutton.cpp \
    mybutton.cpp \
    login.cpp \
    keyboradl.cpp \
    mypushbutton.cpp \
    choose.cpp \
    reg.cpp \
    mysqlquerymodel.cpp \
    myuart.cpp \
    mycamera.cpp \
    mythread.cpp \
    carinfo.cpp \
    pay.cpp

HEADERS  += mainwindow.h \
    videodev2.h \
    amds.h \
    wheeltext.h \
    newbutton.h \
    mybutton.h \
    login.h \
    keyboradl.h \
    mypushbutton.h \
    choose.h \
    reg.h \
    mysqlquerymodel.h \
    myuart.h \
    mycamera.h \
    myhead.h \
    mythread.h \
    carinfo.h \
    pay.h \
    jpeglib.h \
    jconfig.h \
    jerror.h \
    jmorecfg.h

FORMS    += mainwindow.ui \
    amds.ui \
    wheeltext.ui \
    mybutton.ui \
    login.ui \
    keyboradl.ui \
    choose.ui \
    reg.ui \
    carinfo.ui \
    pay.ui

RESOURCES += \
    pic.qrc
