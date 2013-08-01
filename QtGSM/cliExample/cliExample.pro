#-------------------------------------------------
#
# Project created by QtCreator 2013-07-31T18:00:44
#
#-------------------------------------------------

QT       += core serialport
QT       -= gui

top_srcdir=$$PWD
top_builddir=$$shadowed($$PWD)

TARGET = cliExample
CONFIG   += console
CONFIG   -= app_bundle

linux-rasp-pi-g++: {
    message("setting up CLI Example to run on a Raspberry PI")
    target.path = /home/pi/
    INSTALLS += target
}

TEMPLATE = app

INCLUDEPATH += $$PWD/../GSModule/

LIBS += -L$$top_builddir/../GSModule/ -lGSModule

SOURCES += $$PWD/main.cpp \
           $$PWD/autoSMS.cpp

HEADERS += \
    autoSMS.h
