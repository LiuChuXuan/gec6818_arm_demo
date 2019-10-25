#-------------------------------------------------
#
# Project created by QtCreator 2019-10-06T16:08:36
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtProject
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        advertisement.cpp \
        button.cpp \
        keyboard.cpp \
        lcd.cpp \
        loginedit.cpp \
        logininterface.cpp \
        main.cpp \
        maininterface.cpp \
        mainwindow.cpp \
        punctuationsoftkey.cpp \
        softkey.cpp \
        tcpclient.cpp \
        v4l2.cpp \
        weatherdatetime.cpp

HEADERS += \
        advertisement.h \
        button.h \
        jpeg/api_v4l2.h \
        jpeg/jconfig.h \
        jpeg/jerror.h \
        jpeg/jmorecfg.h \
        jpeg/jpeglib.h \
        jpeg/yuyv.h \
        keyboard.h \
        lcd.h \
        loginedit.h \
        logininterface.h \
        maininterface.h \
        mainwindow.h \
        punctuationsoftkey.h \
        softkey.h \
        tcpclient.h \
        v4l2.h \
        weatherdatetime.h

FORMS += \
        advertisement.ui \
        keyboard.ui \
        loginedit.ui \
        logininterface.ui \
        maininterface.ui \
        mainwindow.ui \
        punctuationsoftkey.ui \
        softkey.ui \
        tcpclient.ui \
        v4l2.ui \
        weatherdatetime.ui

unix: LIBS += -L $$PWD/ -ljpeg -lapi_v4l2_arm1 -lpthread

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image/image.qrc

#DISTFILES += \
 #   jpeg/libapi_v4l2_arm1.so \
  #  jpeg/libjpeg.so \
   # jpeg/libjpeg.so.8
