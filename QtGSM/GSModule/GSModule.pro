
QT -= gui
QT += serialport

TARGET = GSModule
TEMPLATE = lib

DEFINES += GSMODULE_LIBRARY

SOURCES += \
       $$PWD/GSModule.cpp

HEADERS += \
       $$PWD/GSModule.h\
       $$PWD/GSModule_global.h

linux-rasp-pi-g++: {
    message("setting up GSM Library to run on a Raspberry PI")
    target.path = /home/pi/
    INSTALLS += target
}
