#ifndef REGISTRY_H
#define REGISTRY_H

#include <QObject>

#include "provider.h"
#include "runnermapper.h"
#include "incommingrunnersmodel.h"

class Registry : public QObject
{
    Q_OBJECT
public:
    explicit Registry(QObject *parent = 0);

    void setModel(IncommingRunnersModel* model);

    int registerProvider(Provider* provider);
    void unregisterProvider(int index);

    int registerMapper(RunnerMapper* mapper, SIProvider* provider);

private:
    IncommingRunnersModel* m_model;

    QMap<int, Provider*> m_providers;
};

#endif // REGISTRY_H
