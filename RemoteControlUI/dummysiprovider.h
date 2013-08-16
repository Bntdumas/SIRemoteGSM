#ifndef DUMMYSIPROVIDER_H
#define DUMMYSIPROVIDER_H

#include "runnermapper.h"

#include <QTimer>

class DummySIProvider : public SIProvider
{
    Q_OBJECT
public:
    explicit DummySIProvider(QObject *parent = 0);
    
private slots:
    void timeout();

private:
    QTimer* m_timer;
    QMap<int, int> m_runners; // time (index) -> si mapping
    int m_index;
};

#endif // DUMMYSIPROVIDER_H
