#ifndef PROVIDER_H
#define PROVIDER_H

#include <QObject>
#include <QTime>

class Provider : public QObject
{
    Q_OBJECT
public:
    explicit Provider(QObject *parent = 0);
    
signals:
    void runnerPunched(const QString& name, const QString& team, const QTime& time, const QTime& realTime,
                       const int lap);
};

#endif // PROVIDER_H
