#-------------------------------------------------
#
# Project created by QtCreator 2013-07-28T10:39:36
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

top_srcdir=$$PWD
top_builddir=$$shadowed($$PWD)

TARGET = SportIdentReaderExample
TEMPLATE = app


SOURCES += $$top_srcdir/main.cpp\
        $$top_srcdir/punchVisualiser.cpp

HEADERS  += $$top_srcdir/punchVisualiser.h

FORMS    += $$top_srcdir/punchVisualiser.ui

INCLUDEPATH += $$top_srcdir/../sportIdentReader/
              # /home/benoit/Downloads/libsportident-0.2.1/src/

LIBS += -L$$top_builddir/../sportIdentReader/ -lsportIdentReader
      #  -L/home/benoit/Downloads/libsportident-0.2.1/src/.libs -llibsportident

message($$top_builddir/)
