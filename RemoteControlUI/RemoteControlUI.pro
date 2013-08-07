# Add more folders to ship with the application, here
folder_01.source = qml/RemoteControlUI
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

qnx:BUILD_BBSMSPROVIDER = true

#debug:BUILD_DUMMYPROVIDER = true

!isEmpty(BUILD_DBUSPROVIDER) {
    QT += dbus
    HEADERS += dbusprovider.h
    SOURCES += dbusprovider.cpp
    DEFINES += DBUSPROVIDER
    message("Building DBus Provider")
}
!isEmpty(BUILD_DUMMYPROVIDER) {
    HEADERS += dummyprovider.h
    SOURCES += dummyprovider.cpp
    DEFINES += DUMMYPROVIDER
    message("Building Dummy Provider")
}
!isEmpty(BUILD_BBSMSPROVIDER) {
    qnx {
        # sql and xml is needed by bbpim
        QT += sql xml
        LIBS += -lbbpim -lbb
        HEADERS += bbsmsprovider.h
        SOURCES += bbsmsprovider.cpp
        DEFINES += BBSMSPROVIDER
        message("Building BlackBerry SMS Provider")
    }
    !qnx {
        warning("You cannot use the Blackberry SMS Provider on non-QNX devices")
    }
}

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    incommingrunnersmodel.cpp \
    sortfilterproxymodel.cpp \
    provider.cpp \
    util.cpp \
    runnermapper.cpp \
    registry.cpp \
    basicfilemapper.cpp

HEADERS += \
    incommingrunnersmodel.h \
    sortfilterproxymodel.h \
    provider.h \
    util.h \
    runnermapper.h \
    registry.h \
    basicfilemapper.h

OTHER_FILES += icon.png

QT += core

contains(QT_MAJOR_VERSION, 5):QT += qml quick
!contains(QT_MAJOR_VERSION, 5):QT += declarative

MOC_DIR = .tmp/moc
OBJECTS_DIR = .tmp/obj
UI_DIR = .tmp/ui

qnx {
    QMAKE_LFLAGS += '-Wl,-rpath,\'./app/native/lib\''

    PACKAGE_ARGS = -arg -platform -arg qnx \
        $${PWD}/bar-descriptor.xml $$TARGET \
        -e $${PWD}/qml qml \
        -e $${PWD}/icon.png res/icon.png \
        -e $$[QT_INSTALL_PLUGINS]/platforms/libqqnx.so plugins/platforms/libqqnx.so #\
#        -e $${PWD}/splash_2.png res/splash_2.png\
#        -e $$[QT_INSTALL_LIBS]/libQt5Core.so.5 lib/libQt5Core.so.5 \
#        -e $$[QT_INSTALL_LIBS]/libQt5Gui.so.5 lib/libQt5Gui.so.5 \
#        -e $$[QT_INSTALL_LIBS]/libQt5OpenGL.so.5 lib/libQt5OpenGL.so.5 \
#        -e $$[QT_INSTALL_LIBS]/libQt5Quick.so.5 lib/libQt5Quick.so.5 \
#        -e $$[QT_INSTALL_LIBS]/libQt5Qml.so.5 lib/libQt5Qml.so.5 \
#        -e $$[QT_INSTALL_LIBS]/libQt5V8.so.5 lib/libQt5V8.so.5 \
#        -e $$[QT_INSTALL_QML]/ imports/

    package.target = $${TARGET}.bar
    package.depends = $$TARGET
    package.commands = blackberry-nativepackager \
        -package $${TARGET}.bar \
        -devMode -debugToken ~/.rim/debugtoken1.bar \
        $${PACKAGE_ARGS}

    signedpackage.target = $${TARGET}-signed.bar
    signedpackage.depends = $$TARGET
    signedpackage.commands = blackberry-nativepackager \
        -package $${TARGET}-signed.bar \
        -sign -storepass $$(BB_STORE_PASS) \
        $${PACKAGE_ARGS}

    QMAKE_EXTRA_TARGETS += package signedpackage

    OTHER_FILES += bar-descriptor.xml
    DEFINES += CUSTOMDEBUGHANDLER
    LIBS += -lbb -lbbcascades
}

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
contains(QT, qml):include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
!contains(QT, qml):include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
