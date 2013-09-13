#include "util.h"

Util* Util::instance = new Util();

Util::Util(QObject *parent) :
    QObject(parent), m_competitionTime(QDateTime(QDate::currentDate(), QTime(9, 00)))
{

}

int Util::timeToSeconds(const QTime &time)
{
    return time.hour() * 60 * 60 + time.minute() * 60 + time.second();
}

QDateTime Util::timeDifference(const QTime &t1, const QTime &t2)
{
    QTime time = QTime(0, 0, 0, 0);
    time = time.addSecs(t2.secsTo(t1));
    time = time.addMSecs(t1.msec() - t2.msec());
    return QDateTime(QDate::currentDate(), time);
}
