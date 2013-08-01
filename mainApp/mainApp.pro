
top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)

QT += core
QT -= gui

TARGET = SIRemoteGSM
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

linux-rasp-pi-g++: {
    message("setting up Sport ident GSM to run on a Raspberry PI")
    target.path = /home/pi/sportIdentGSM/
    INSTALLS += target
}

SOURCES += $$top_srcdir/main.cpp \
           $$top_srcdir/SIPunchToSMS.cpp

HEADERS += $$top_srcdir/SIPunchToSMS.h \

INCLUDEPATH += $$top_srcdir/../GSModule/ \
               $$top_srcdir/../sportIdentReader/ \
               $$top_srcdir/../wiringPi/

LIBS += -L$$top_builddir/../sportIdentReader/sportIdentReader/ -lsportIdentReader \
        -L$$top_builddir/../QtGSM/GSModule/ -lGSModule \
        -L$$top_builddir/../wiringPi/ -lwiringPi

message($$top_srcdir/../wiringPi/)
