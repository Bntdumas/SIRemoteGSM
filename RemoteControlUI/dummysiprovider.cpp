#include "dummysiprovider.h"

#include <qmath.h>

DummySIProvider::DummySIProvider(QObject *parent) :
    SIProvider(parent), m_index(0)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

    m_runners[2] = 42790;
    m_runners[4] = 17019;
    m_runners[8] = 684897;
    m_runners[9] = 890510;
    m_runners[14] = 1337;
    m_runners[15] = 42060;

    m_timer->start(1000);
}

void DummySIProvider::timeout()
{
    if (m_runners.contains(m_index)) {
        emit runnerPunched(m_runners[m_index], QTime::currentTime());
    } else if (20 < m_index) {
        if (qrand() % 5) {
            QTime time = QTime::currentTime();
            time.addSecs(qrand() % 7200);
            emit runnerPunched(qrand() % 9999999, time);
        }
    }

    m_index++;
}
