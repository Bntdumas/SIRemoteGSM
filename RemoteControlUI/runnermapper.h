#ifndef RUNNERMAPPER_H
#define RUNNERMAPPER_H

#include "provider.h"

#include <QDebug>
#include <QMap>

class QAbstractItemModel;

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

    Q_PROPERTY(QAbstractItemModel* runnersModel READ model NOTIFY modelChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessage)
    Q_PROPERTY(bool supportsEdit READ supportsEdit)

public:
    explicit RunnerMapper(QObject *parent = 0);

    void setProvider(SIProvider* provider);

    QAbstractItemModel* model() { return m_model; }

    virtual bool supportsEdit() { return false; }

    QString errorMessage() const { return m_errorMessage; }

public slots:
    virtual void changeSINumber(const int si, const int newSI) { Q_UNUSED(si); Q_UNUSED(newSI); }
    virtual void changeName(const int si, const QString& newName) { Q_UNUSED(si); Q_UNUSED(newName); }
    virtual void changeLap(const int si, const int newLap) { Q_UNUSED(si); Q_UNUSED(newLap); }
    virtual void removeRunner(const int si) { Q_UNUSED(si); }
    virtual void addRunner(const int si, const QString& name, const QString& team, const int lap)
    { Q_UNUSED(si); Q_UNUSED(name); Q_UNUSED(team); Q_UNUSED(lap); }

    virtual int siForIndex(const int index);
    virtual QString nameForIndex(const int index);
    virtual int lapForIndex(const int index) { Q_UNUSED(index);return -1; }

protected:
    virtual bool map(const int si, const QTime& realTime,
                     QString* name, QString* team, QTime* time, int* lap, int* bib) = 0;

    virtual QMap<int, QString> runners() = 0;

    void reloadRunners();

    void error(const QString& message);

private slots:
    void punched(const int si, const QTime& realTime);

private:
    SIProvider* m_provider;
    QAbstractItemModel* m_model;
    QString m_errorMessage;

signals:
    void modelChanged();
    void errorMessage(const QString& message);
};

#endif // RUNNERMAPPER_H
