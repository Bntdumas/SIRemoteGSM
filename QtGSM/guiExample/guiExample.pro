QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

top_srcdir=$$PWD
top_builddir=$$shadowed($$PWD)

TARGET = gsmGUITest
TEMPLATE = app


SOURCES += \
    $$PWD/main.cpp\
    $$PWD/view.cpp

HEADERS  += \
    $$PWD/view.h

FORMS    += \
    $$PWD/view.ui

INCLUDEPATH += $$PWD/../GSModule/

LIBS += -L$$top_builddir/../GSModule/ -lGSModule

OTHER_FILES += \
    README.md
