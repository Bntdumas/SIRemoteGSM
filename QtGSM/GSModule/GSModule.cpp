#include "GSModule.h"

#include <QSerialPort>
#include <QDebug>

#define SIM_PIN 4302
#define SMS_MAX_LENGTH 160

GSModule::GSModule(QObject *parent, bool debugOutputEnabled)
    : QObject(parent),
    m_serialPort(0),
    m_debugOutputEnabled(debugOutputEnabled)
{
    m_transmissionWaitTimer.setInterval(200);
    connect(&m_transmissionWaitTimer, SIGNAL(timeout()), this, SLOT(serialDataAvailable()));

    if (m_debugOutputEnabled) {
        connect(this, SIGNAL(message(QString,GSModule::MessageType)), this, SLOT(messageAsDebug(QString,GSModule::MessageType)));
    }
}

void GSModule::initializeSerialConnection( QSerialPort *serialPort)
{
    if (m_serialPort) {
        m_serialPort->close();
        delete m_serialPort;
    }

    m_serialPort = serialPort;
    connect (m_serialPort, SIGNAL(readyRead()), this, SLOT(serialDataAvailable()));

   emit message(tr("Connecting to serial port '%1': %2 (%3)").arg(m_serialPort->portName())
                .arg(m_serialPort->open(QIODevice::ReadWrite) ? "success":"failure")
                .arg(m_serialPort->errorString()), DebugInformation);

    m_serialPort->setBaudRate(QSerialPort::Baud19200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);

    send("");
}

void GSModule::send(const QString &line)
{
    if (!m_serialPort->isOpen()) {
        emit message(tr("Data send failed: the serial port is not open."), Error);
        return;
    }

    QByteArray rawMsg(line.toLatin1());
    rawMsg.append('\r');

    emit message(tr("Sending %1 characters").arg(rawMsg.length()), DebugInformation);
    m_serialPort->write(rawMsg);
}

void GSModule::sendSMS(const QString &msg, const QString &phoneNumber)
{
    emit message(tr("Sending SMS..."), ATFormatted);
    if (msg.length() > SMS_MAX_LENGTH) {
        emit message(tr("The message you are trying to send contains %1 characters, only 160 are allowed.")
                     .arg(msg.length()), Error);
    }
    m_SMSMessageText = msg;
    m_SMSMessageText.append(QChar(0x1a)); //message termination character

    //TODO check phone number compliance with E.123

    // make sure we sent SMS in text mode
    requestSendCommand(QLatin1String("AT+CMGF=1"));

    // SMS request
    requestSendCommand(QString::fromLatin1(" AT+CMGS=\"%1\"").arg(phoneNumber));
}


void GSModule::serialDataAvailable()
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
            emit message(m_dataBuffer, RawSerial);
            m_dataBuffer.clear();
            m_waitingForEndOfTransmission = false;
    }
}

void GSModule::messageAsDebug(const QString &message, GSModule::MessageType type)
{
    if (!m_debugOutputEnabled) {
        return;
    }

    switch (type) {
    case DebugInformation:
        qDebug() << "DEBUG : " << message;
        break;
    case Error:
        qWarning("%s", message.toLocal8Bit().constData());
        break;
    case ATFormatted:
        qDebug() << "AT : " << message;
        break;
    case RawSerial:
        qDebug() << "RawSerial : " << message;
        break;
    }
}

void GSModule::processIncomingMessage(const QString &msg)
{
    if (msg.isEmpty()) {
        return;
    }else if (msg.contains(QLatin1String("OK")) || msg.contains(QLatin1String("ERROR"))) {
        sendNextCommandInLine();
    }
    if (msg.contains(QLatin1String("+CPIN: SIM PIN"))) {
        emit message(tr("GSM module requested PIN code, sending %1").arg(SIM_PIN), ATFormatted);
        requestSendCommand(QString("AT+CPIN=\"%1\"").arg(SIM_PIN));
        return;
    } else if (msg.contains(QLatin1String("Call Ready"))) {
        emit readyToTransmit();
    } else if (msg.contains(QLatin1String(">")) && !m_SMSMessageText.isEmpty()) {
        send(m_SMSMessageText);
        emit message(tr("SMS sent"), ATFormatted);
        m_SMSMessageText.clear();
    }
}

void GSModule::sendNextCommandInLine()
{
    if (m_commandsQueue.isEmpty()) {
        return;
    }  else {
        m_commandsQueue.removeFirst();
        if (!m_commandsQueue.isEmpty()) {
            send(m_commandsQueue.first());
        }
    }
}

void GSModule::requestSendCommand(const QString &command)
{
    if (m_commandsQueue.isEmpty()) { //if we're not waiting for ACK on a previous command, send it right away
        m_commandsQueue.append(command);
        send(command);
    } else {
        m_commandsQueue.append(command);
    }
}

