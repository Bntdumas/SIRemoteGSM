#include "runnermapper.h"

#include <QDebug>

SIProvider::SIProvider(QObject *parent) :
    QObject(parent)
{

}

RunnerMapper::RunnerMapper(QObject *parent) :
    Provider(parent), m_provider(0)
{
}

void RunnerMapper::setProvider(SIProvider *provider)
{
    if (m_provider) {
        disconnect(m_provider, SIGNAL(runnerPunched(int,QTime)), this, SLOT(punched(int,QTime)));
    }
    m_provider = provider;
    if (m_provider) {
        connect(m_provider, SIGNAL(runnerPunched(int,QTime)), this, SLOT(punched(int,QTime)));
    }
}

void RunnerMapper::punched(const int si, const QTime &realTime)
{
    QString name;
    QString team;
    QTime time;
    int lap;
    qDebug() << "NOTE: Requesting mapping";
    if (map(si, realTime, &name, &team, &time, &lap)) {
        qDebug() << "NOTE: Emitting runnerPunched";
        emit runnerPunched(name, team , time, realTime, lap);
    }
}
