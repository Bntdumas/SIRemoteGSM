
top_srcdir=$$PWD

QT -= gui
QT += serialport

TARGET = GSModule
TEMPLATE = lib

DEFINES += GSMODULE_LIBRARY

SOURCES += \
       $$top_srcdir/GSModule.cpp

HEADERS += \
       $$top_srcdir/GSModule.h\
       $$top_srcdir/GSModule_global.h

linux-rasp-pi-g++: {
    message("setting up GSM Library to run on a Raspberry PI")
    target.path = /home/pi/sportIdentGSM/
    INSTALLS += target
}
