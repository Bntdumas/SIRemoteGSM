#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QTime>
#include <QDateTime>

class Util : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime competitionTime READ competitionTime WRITE setCompetitionTime NOTIFY competitionTimeChanged)

public:
    static Util* instance;

    Q_INVOKABLE int timeToString(const QTime& time);
    Q_INVOKABLE QDateTime timeDifference(const QTime& t1, const QTime& t2);

    QDateTime competitionTime() const { return m_competitionTime; }

public slots:
    void setCompetitionTime(QDateTime arg)
    {
        if (m_competitionTime != arg) {
            m_competitionTime = arg;
            emit competitionTimeChanged(arg);
        }
    }

signals:
    void competitionTimeChanged(QDateTime arg);

private:
    Util(QObject* parent = 0);

    QDateTime m_competitionTime;
};

#endif // UTIL_H
