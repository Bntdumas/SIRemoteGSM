#include "SIPunchToSMS.h"

#include "wiringPi.h"

#include <QTimer>

SIPunchToSMS::SIPunchToSMS(const QString &GSMPort, const QString &SIPort, QObject *parent) :
    QObject(parent)
{

    int ret = wiringPiSetup();
    if(ret == -1)
        qFatal("failed to setup wiringPi lib.");
    pinMode(Power, OUTPUT) ;
    pinMode(Reset, OUTPUT)  ;

    m_ButtonTimer = new QTimer(this);
    m_ButtonTimer->setSingleShot(true);
    connect(m_ButtonTimer, SIGNAL(timeout()), this, SLOT(releaseButton()));
}

void SIPunchToSMS::resetGSModule()
{
    digitalWrite(Reset, true) ;
}

void SIPunchToSMS::powerGSModule()
{
    digitalWrite(Power, true) ;
    m_ButtonTimer->start(5000);
}

void SIPunchToSMS::releaseButtons()
{
    digitalWrite(Power, false) ;
    digitalWrite(Reset, false) ;
}
