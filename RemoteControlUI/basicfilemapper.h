#ifndef BASICFILEMAPPER_H
#define BASICFILEMAPPER_H

#include "runnermapper.h"

#include <QMap>

class BasicFileMapper : public RunnerMapper
{
    Q_OBJECT
public:
    explicit BasicFileMapper(QObject *parent = 0);

    virtual bool supportsEdit() { return true; }

private:
    struct MappedRunner
    {
        QString name;
        QString team;
        QString bibnumber;
        int lap;
    };

    QMap<int, MappedRunner> m_runners;

    QString generateCache() const;
    void readCache(const QString &data);
    bool hasCache() const;
    void saveToCache();
    void loadCache();

    void loadPermanentData();
    void loadRunners();
    void loadRunnersFromXML();

protected:
    bool map(const int si, const QTime &realTime, QString *name, QString *team, QTime *time, int *lap);
    QMap<int, QString> runners();

public slots:
    void changeSINumber(const int si, const int newSI);
    void changeName(const int si, const QString &newName);
    void changeLap(const int si, const int newLap);
    void removeRunner(const int si);
    void addRunner(const int si, const QString &name, const QString &team, const int lap);

    int lapForIndex(const int index);

    void invalidateCache();
};

#endif // BASICFILEMAPPER_H
