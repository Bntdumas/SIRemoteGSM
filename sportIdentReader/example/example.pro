QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

top_srcdir=$$PWD
top_builddir=$$shadowed($$top_srcdir)
message($$top_builddir)

TARGET = SportIdentReaderExample
TEMPLATE = app


SOURCES += $$top_srcdir/main.cpp\
        $$top_srcdir/punchVisualiser.cpp

HEADERS  += $$top_srcdir/punchVisualiser.h

FORMS    += $$top_srcdir/punchVisualiser.ui

INCLUDEPATH += $$top_srcdir/../sportIdentReader/
LIBS += -L$$top_builddir/../sportIdentReader/ -lsportIdentReader
