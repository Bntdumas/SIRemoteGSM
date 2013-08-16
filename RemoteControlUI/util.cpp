#include "util.h"

#include <qmath.h>

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
    // TODO optimize
    int mseconds1 = t1.hour() * 60 * 60 * 1000 + t1.minute() * 60 * 1000 + t1.second() * 1000 + t1.msec();
    int mseconds2 = t2.hour() * 60 * 60 * 1000 + t2.minute() * 60 * 1000 + t2.second() * 1000 + t2.msec();
    int difference = mseconds1 - mseconds2;
    if (difference < 0) {
        return QDateTime(QDate::currentDate(), QTime());
    }
    int hours = qFloor(difference / 1000 / 60 / 60);
    difference -= hours * 60 * 60 * 1000;
    int minutes = qFloor(difference / 1000 / 60);
    difference -= minutes * 60 * 1000;
    int seconds = qFloor(difference / 1000);
    difference -= seconds * 1000;
    int mseconds = difference;
    return QDateTime(QDate::currentDate(), QTime(hours, minutes, seconds, mseconds));
}
