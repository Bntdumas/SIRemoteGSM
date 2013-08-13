QT += core serialport
QT -= gui

top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)

TARGET = cliExample
CONFIG += console
CONFIG -= app_bundle

linux-rasp-pi-g++: {
    message("setting up CLI Example to run on a Raspberry PI")
    target.path = /home/pi/sportIdentGSM/
    INSTALLS += target
    DEFINES += RASPBERRYPI
}

TEMPLATE = app

INCLUDEPATH += $$top_srcdir/../GSModule/

LIBS += -L$$top_builddir/../GSModule/ -lGSModule

linux-rasp-pi-g++: {
    LIBS += -L$$top_srcdir/../../wiringPi/ -lwiringPi
    INCLUDEPATH += $$top_srcdir/../../wiringPi/
}

SOURCES += $$top_srcdir/main.cpp \
           $$top_srcdir/autoSMS.cpp

HEADERS += \
    $$top_srcdir/autoSMS.h
