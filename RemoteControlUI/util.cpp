#include "util.h"

Util* Util::instance = new Util();

Util::Util(QObject *parent) :
    QObject(parent), m_competitionTime(QDateTime(QDate::currentDate(), QTime(9, 00)))
{

}

int Util::timeToString(const QTime &time)
{
    return time.hour() * 60 * 60 + time.minute() * 60 + time.second();
}
