
QT -= gui
QT += serialport

TARGET = sportIdentReader
TEMPLATE = lib

DEFINES += SPORTIDENTREADER_LIBRARY

SOURCES += \
       $$PWD/SportIdentReader.cpp

HEADERS += \
       $$PWD/SportIdentReader.h\
       $$PWD/ sportidentreader_global.h

unix: {
    INSTALLS += target
}
