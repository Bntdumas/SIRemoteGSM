#ifndef DBUSPROVIDER_H
#define DBUSPROVIDER_H

#include "provider.h"

class DBusProvider : public Provider
{
    Q_OBJECT
public:
    explicit DBusProvider(QObject *parent = 0);

public slots:
    Q_SCRIPTABLE void runnerPunchedAtControl(const QString& name, const QString& team, const QTime& time,
                                             const QTime& realTime, const int lap);
};

#endif // DBUSPROVIDER_H
