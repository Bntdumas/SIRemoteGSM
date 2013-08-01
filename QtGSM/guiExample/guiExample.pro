QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)

TARGET = gsmGUITest
TEMPLATE = app


SOURCES += \
    $$top_srcdir/main.cpp\
    $$top_srcdir/view.cpp

HEADERS  += \
    $$top_srcdir/view.h

FORMS    += \
    $$top_srcdir/view.ui

INCLUDEPATH += $$top_srcdir/../GSModule/

LIBS += -L$$top_builddir/../GSModule/ -lGSModule

OTHER_FILES += \
    README.md
