#include "SportIdentReader.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QtEndian>
#include <QDebug>
#include <QTime>

SportIdentReader::SportIdentReader(QObject *parent, bool debugOutputEnabled) :
    QObject(parent),
    m_serialPort(0),
    m_connected(false),
    m_debugOutputEnabled(debugOutputEnabled)
{
    m_transmissionWaitTimer.setInterval(100);
    connect(&m_transmissionWaitTimer, SIGNAL(timeout()), this, SLOT(serialDataAvailable()));

    if (m_debugOutputEnabled) {
        connect(this, SIGNAL(logText(QString,SportIdentReader::MessageType)), this, SLOT(messageAsDebug(QString,SportIdentReader::MessageType)));
    }
}

void SportIdentReader::initSerialConnection(const QString & port, const int serialSpeed)
{
    if (!port.isEmpty()) {
        setserialPort(port);
    }
    if (serialSpeed) {
        setserialSpeed(serialSpeed);
    }

    if (m_serialPort) {
        m_serialPort->close();
        delete m_serialPort;
    }

    m_serialPort = new QSerialPort(this);
    m_serialPort->clearError();

    m_serialPort->setPort(QSerialPortInfo(m_port));

   connect (m_serialPort, SIGNAL(readyRead()), this, SLOT(serialDataAvailable()));

    emit logText(tr("Connecting to serial port '%1': %2 (%3)").arg(m_serialPort->portName())
                 .arg(m_serialPort->open(QIODevice::ReadWrite) ? "success":"failure")
                 .arg(m_serialPort->errorString()), DebugInformation);

    if (!m_serialPort->setBaudRate(m_serialSpeed)) {
        emit logText(tr("Failed setting up Baud Rate: %1, current baud rate: %2")
                     .arg(m_serialPort->error())
                     .arg(m_serialPort->baudRate()), Error);
    } else if (! m_serialPort->setDataBits(QSerialPort::Data8)) {
        emit logText(tr("Failed setting up Data bits: %1").arg(m_serialPort->error()), Error);
    } else if (!m_serialPort->setParity(QSerialPort::NoParity)) {
        emit logText(tr("Failed setting up Parity: %1").arg(m_serialPort->error()), Error);
    } else if (!m_serialPort->setStopBits(QSerialPort::OneStop)) {
        emit logText(tr("Failed setting up Stop Bits: %1").arg(m_serialPort->error()), Error);
    }
    m_serialPort->clearError();
}

void SportIdentReader::setserialPort(const QString & port)
{
    m_port = port;
}

void SportIdentReader::setserialSpeed(const int serialSpeed)
{
    m_serialSpeed = serialSpeed;
    if (m_serialSpeed != 4800 && m_serialSpeed != 38400) {
        emit logText(tr("The serial baud rate you entered (%1) is probably wrong, "
                        "the SportIdent modules use either 4800 or 38400 bauds/s.")
                     , Warning);
    }

}

void SportIdentReader::serialDataAvailable()
{
    if (m_serialPort->bytesAvailable()) {
        if (m_waitingForEndOfTransmission) {
            // more data coming. Store the current data and wait for more
            m_dataBuffer.append(m_serialPort->readAll());
            m_transmissionWaitTimer.start();
        } else{
            // start waiting for more data
            m_waitingForEndOfTransmission = true;
            m_transmissionWaitTimer.start();
        }
    } else if (m_waitingForEndOfTransmission ) {
            // end of transmission, we can use the buffered data now
            processIncomingMessage(m_dataBuffer);
            m_dataBuffer.clear();
            m_waitingForEndOfTransmission = false;
    }
}

void SportIdentReader::messageAsDebug(const QString &message, SportIdentReader::MessageType type)
{
    if (!m_debugOutputEnabled) {
        return;
    }

    switch (type) {
    case DebugInformation:
        qDebug() << "SI - error" << message;
    case Error:
        qWarning(message.toLocal8Bit().constData());
        break;
    case Warning:
        qDebug() << "SI - Warning : " << message;
        break;
    case RawSerial:
        qDebug() << "SI - RawSerial : " << message;
        break;
    }
}

void SportIdentReader::processIncomingMessage(const QByteArray &msg)
{
    if (msg.isEmpty()) {
        return;
    }

    emit rawData(msg);

    const QByteArray trimmedMessage = removeSeparationCharacters(msg);
    //emit logText(byteArrayToHexaString(msg), RawSerial);
    emit logText(byteArrayToHexaString(trimmedMessage), RawSerial);

    // retrieve SI card number
    QByteArray SINumberHex;
    for (int i = 0; i < 3; ++i) {
        SINumberHex.append(trimmedMessage.at(SICardNumber + i));
    }
    // TODO handle SI cards better, this method does not work for SI 5
    const int SINumber = SINumberHex.toHex().toInt(0, 16);


    emit logText(tr("SI Card number: %1").arg(SINumber), DebugInformation);
    emit SIPunch(SINumber, QTime::currentTime());

   // emit logText(byteArrayToHexaString(removeSeparationCharacters(msg)), RawSerial);

}

QString SportIdentReader::byteArrayToHexaString(const QByteArray &array) const
{
    QString hex = QString(array.toHex());
    int byteCount = 0;
    for (int i = 0; i < hex.size() + 1; i++) { //add a space to make groups of bytes
        if ((i+1) % 3) {
            hex.insert(i++, ' ');
            byteCount++;
        }
    }

    // big hack to break lines, ugly but it works
    QStringList byteList = hex.split(' ');
    for (double i = 1; i < (byteCount / 16.0) + 1; i++) {
        byteList.insert(i * 32, "<br>");
    }

    // make it nicer, still super hacky but it does the trick
    hex = byteList.join(" ");
    hex.replace(QString("<br> "), QString("<br>"));
    return hex;
}

QByteArray SportIdentReader::removeSeparationCharacters(const QByteArray &array) const
{
    QByteArray newArray = array;
    int currentIndex = 0;
    char characterToRemove = 0x10;
    do {
        currentIndex = newArray.indexOf(characterToRemove);
        newArray.remove(currentIndex, 1);
    } while (currentIndex != -1);

    return newArray;
}
