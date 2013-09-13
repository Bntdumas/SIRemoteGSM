#include "dbusprovider.h"

#include <QtDBus>

#define SERVICE_NAME "jan.dalheimer.remotecontrol.RemoteControlUI"

DBusProvider::DBusProvider(QObject *parent) :
    Provider(parent)
{
    if (!QDBusConnection::sessionBus().isConnected()) {
        qFatal(QDBusConnection::sessionBus().lastError().message().toLocal8Bit().constData());
    }

    if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME)) {
        qFatal(QDBusConnection::sessionBus().lastError().message().toLocal8Bit().constData());
    }

    QDBusConnection::sessionBus().registerObject("/", this, QDBusConnection::ExportAllSlots);
}

void DBusProvider::runnerPunchedAtControl(const QString &name, const QString &team, const QTime &time, const QTime &realTime, const int lap)
{
    emit runnerPunched(name, team, time, realTime, lap, 0);
}
