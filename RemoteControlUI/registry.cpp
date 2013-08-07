#include "registry.h"

Registry::Registry(QObject *parent) :
    QObject(parent), m_model(0)
{
}

void Registry::setModel(IncommingRunnersModel *model)
{
    m_model = model;
}

int Registry::registerProvider(Provider *provider)
{
    connect(provider, SIGNAL(runnerPunched(QString,QString,QTime,QTime,int)),
            m_model, SLOT(addRunner(QString,QString,QTime,QTime,int)));
    int index = m_providers.size();
    m_providers.insert(index, provider);
    return index;
}

void Registry::unregisterProvider(int index)
{
    if (!m_providers.contains(index)) {
        return;
    }

    Provider* provider = m_providers[index];
    disconnect(provider, SIGNAL(runnerPunched(QString,QString,QTime,QTime,int)),
            m_model, SLOT(addRunner(QString,QString,QTime,QTime,int)));
    m_providers.remove(index);
    provider->deleteLater();
}

int Registry::registerMapper(RunnerMapper *mapper, SIProvider *provider)
{
    mapper->setProvider(provider);
    return registerProvider(mapper);
}
