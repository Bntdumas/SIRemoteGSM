#include "SIPunchToSMS.h"
#include "GSModule.h"
#include "SportIdentReader.h"

#include "wiringPi.h"

#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

SIPunchToSMS::SIPunchToSMS(const QString &GSMPort, const QString &SIPort, QObject *parent) :
    QObject(parent),
    m_GSModuleIsReady(false),
    m_GSModule(0)
{

    const int ret = wiringPiSetup();
    if (ret == -1) {
        qFatal("failed to setup wiringPi lib.");
    }
    pinMode(Power, OUTPUT);
    pinMode(Reset, OUTPUT);

    m_ButtonTimer = new QTimer(this);
    m_ButtonTimer->setSingleShot(true);
    connect(m_ButtonTimer, SIGNAL(timeout()), this, SLOT(releaseButtons()));

    initialiseGSModule(GSMPort);
    initialiseSportIdentReader(SIPort);
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

void SIPunchToSMS::GSModuleReady()
{
    m_GSModuleIsReady = true;
}

void SIPunchToSMS::initialiseGSModule(const QString &port)
{
    qDebug() << "Setting up GSM Module";
    if (m_GSModule) {
        delete m_GSModule;
    }

    m_GSModule = new GSModule(this, true);

    QSerialPort *serialPort = new QSerialPort();
    serialPort->setPort(QSerialPortInfo(port));
    serialPort->setBaudRate(QSerialPort::Baud19200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::OddParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    m_GSModule->initializeSerialConnection(serialPort);
    powerGSModule();
}

void SIPunchToSMS::initialiseSportIdentReader(const QString &port)
{
    qDebug() << "Setting up master SI unit";
    if (m_SIReader) {
        delete m_SIReader;
    }

    m_SIReader = new SportIdentReader(this, true);
    m_SIReader->setserialPort(port);
    m_SIReader->setserialSpeed(38400);
    m_SIReader->initSerialConnection();
}


