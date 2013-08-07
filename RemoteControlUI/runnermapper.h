#ifndef RUNNERMAPPER_H
#define RUNNERMAPPER_H

#include "provider.h"

class SIProvider : public QObject
{
    Q_OBJECT
public:
    SIProvider(QObject* parent = 0);

signals:
    void runnerPunched(const int si, const QTime& realTime);
};

class RunnerMapper : public Provider
{
    Q_OBJECT
public:
    explicit RunnerMapper(QObject *parent = 0);

    void setProvider(SIProvider* provider);
    
protected:
    virtual bool map(const int si, const QTime& realTime,
                     QString* name, QString* team, QTime* time, int* lap) = 0;

private slots:
    void punched(const int si, const QTime& realTime);

private:
    SIProvider* m_provider;
};

#endif // RUNNERMAPPER_H
