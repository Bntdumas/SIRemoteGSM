#ifndef BASICFILEMAPPER_H
#define BASICFILEMAPPER_H

#include "runnermapper.h"

#include <QMap>

class BasicFileMapper : public RunnerMapper
{
    Q_OBJECT
public:
    explicit BasicFileMapper(QObject *parent = 0);

private:
    struct MappedRunner
    {
        QString name;
        QString team;
        QString bibnumber;
        int lap;
    };

    QMap<int, MappedRunner> m_runners;

    void loadRunners();
    void loadRunnersFromXML();

protected:
    bool map(const int si, const QTime &realTime, QString *name, QString *team, QTime *time, int *lap);
};

#endif // BASICFILEMAPPER_H
