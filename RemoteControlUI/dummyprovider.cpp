#include "dummyprovider.h"

#include <QTimer>

DummyProvider::DummyProvider(QObject *parent) :
    Provider(parent)
{
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(fill()));
    timer->start(1000);
}

int i = 0;
void DummyProvider::fill()
{
    switch(i) {
    case 0:
        emit runnerPunched("Hugo Bergenfur", "Hagfors 2", QTime(0, 5, 5), QTime(13, 18), 1, 1); break;
    case 1:
        emit runnerPunched("Erik Andersson", "Hagfors 1", QTime(0, 6, 45), QTime(13, 19, 40), 1, 2); break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        break;
    case 7:
        emit runnerPunched("Elias Bergenfur", "Hagfors 1", QTime(0, 12, 35), QTime(13, 29), 2, 3); break;
    case 8:
        emit runnerPunched("Tim Dalheimer", "Hagfors 2", QTime(0, 12, 34), QTime(13, 30), 2, 4); break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        break;
    case 14:
        emit runnerPunched("Julia Andersson", "Hagfors 1", QTime(0, 8, 58), QTime(13, 38), 3, 5);
        emit runnerPunched("Elsa Fischer", "Hagfors 2", QTime(0, 10, 10), QTime(13, 40, 10), 3, 6); break;
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        break;
    case 20:
        emit runnerPunched("Some more girl", "Hagfors 1", QTime(0, 9, 39), QTime(13, 47), 4, 7); break;
    case 21:
        emit runnerPunched("Hanna Fischer", "Hagfors 2", QTime(0, 15, 10), QTime(13, 50), 4, 8); break;
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
        break;
    case 27:
        emit runnerPunched("Jan Dalheimer", "Hagfors 1", QTime(0, 13, 36), QTime(14, 00), 5, 9); break;
    case 28:
        emit runnerPunched("Benoit Dumas", "Hagfors 2", QTime(0, 13, 37), QTime(14, 05), 5, 10); break;
    }
    i++;
}
