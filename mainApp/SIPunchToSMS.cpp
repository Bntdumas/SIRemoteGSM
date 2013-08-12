#include "SIPunchToSMS.h"
#include "GSModule.h"
#include "SportIdentReader.h"

#ifdef RASPBERRYPI
#include "wiringPi.h"
#endif

#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTime>

SIPunchToSMS::SIPunchToSMS(const QString &GSMPort, const QString &SIPort, QObject *parent) :
    QObject(parent),
    m_GSModuleIsReady(false),
    m_GSModule(0)
{

#ifdef RASPBERRYPI
    const int ret = wiringPiSetup();
    if (ret == -1) {
        qFatal("failed to setup wiringPi lib.");
    }
    pinMode(Power, OUTPUT);
    pinMode(Reset, OUTPUT);
#endif

    m_ButtonTimer = new QTimer(this);
    m_ButtonTimer->setSingleShot(true);
    connect(m_ButtonTimer, SIGNAL(timeout()), this, SLOT(releaseButtons()));

    initialiseGSModule(GSMPort);
    initialiseSportIdentReader(SIPort);
}

void SIPunchToSMS::resetGSModule()
{
#ifdef RASPBERRYPI
    digitalWrite(Reset, true) ;
#endif
}

void SIPunchToSMS::powerGSModule()
{
#ifdef RASPBERRYPI
    digitalWrite(Power, true) ;
#endif
    m_ButtonTimer->start(5000);
}

void SIPunchToSMS::releaseButtons()
{
#ifdef RASPBERRYPI
    digitalWrite(Power, false) ;
    digitalWrite(Reset, false) ;
#endif
}

void SIPunchToSMS::GSModuleReady()
{
    m_GSModuleIsReady = true;
}

void SIPunchToSMS::punchToSMS(const int SInumber, const QTime &punchTimeSTamp)
{
    qDebug() << Q_FUNC_INFO;
    if (m_GSModuleIsReady) {
        const QString SMS = QString("%1@%2").arg(SInumber).arg(punchTimeSTamp.toString());
        m_GSModule->sendSMS(SMS, QLatin1String("+46722230141"));
    }
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
    connect(m_GSModule, SIGNAL(readyToTransmit()), this, SLOT(GSModuleReady()));
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

    connect(m_SIReader, SIGNAL(SIPunch(int,QTime)), this, SLOT(punchToSMS(int,QTime)));
}


