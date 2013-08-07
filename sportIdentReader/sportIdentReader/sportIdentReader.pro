top_srcdir=$$PWD

QT -= gui
QT += serialport

TARGET = sportIdentReader
TEMPLATE = lib

DEFINES += SPORTIDENTREADER_LIBRARY

SOURCES += \
       $$top_srcdir/SportIdentReader.cpp \
    sportidentmessageparser.cpp

HEADERS += \
       $$top_srcdir/SportIdentReader.h\
       $$top_srcdir/Sportidentreader_global.h \
    sportidentmessageparser.h

linux-rasp-pi-g++: {
    message("setting up Sport ident reader to run on a Raspberry PI")
    target.path = /home/pi/sportIdentGSM/
    INSTALLS += target
}
