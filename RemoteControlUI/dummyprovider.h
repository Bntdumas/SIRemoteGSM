#ifndef DUMMYPROVIDER_H
#define DUMMYPROVIDER_H

#include "provider.h"

class DummyProvider : public Provider
{
    Q_OBJECT
public:
    explicit DummyProvider(QObject *parent = 0);

private slots:
    void fill();
};

#endif // DUMMYPROVIDER_H
