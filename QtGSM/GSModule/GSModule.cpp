#include "GSModule.h"

#include <QSerialPort>
#include <QDebug>

#define SIM_PIN 4302
#define SMS_MAX_LENGTH 160

GSModule::GSModule(QObject *parent, bool debugOutputEnabled)
    : QObject(parent),
    m_serialPort(0),
    m_debugOutputEnabled(debugOutputEnabled),
    m_readyForNextSMS(true)
{
    if (m_debugOutputEnabled) {
        connect(this, SIGNAL(message(QString,GSModule::MessageType)), this, SLOT(messageAsDebug(QString,GSModule::MessageType)));
    }

    m_commandQueueTimer.setInterval(30000);
    m_commandQueueTimer.setSingleShot(false);
    connect(&m_commandQueueTimer, SIGNAL(timeout()), this, SLOT(sendNextCommandInLine()));
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

    m_commandQueueTimer.start();

    QByteArray rawMsg(line.toLatin1());
    rawMsg.append('\r');

    //emit message(tr("Sending %1 characters").arg(rawMsg.length()), DebugInformation);
    m_serialPort->write(rawMsg);
}

void GSModule::sendSMS(const QString &msg, const QString &phoneNumber)
{
    const QString smsText = msg + QChar(0x1a); //message termination character
    m_SMSMessagesList.append(smsText);
    m_SMSPhoneNumberList.append(phoneNumber);     //TODO check phone number compliance with E.123
    sendNextSMS();
}


void GSModule::serialDataAvailable()
{
    m_dataBuffer.append(m_serialPort->readAll());

    if (m_dataBuffer.contains('>')) {
        requestSendCommand(m_nextSMS);
        m_dataBuffer.remove('>');
    }

    // process each line at the time
    while (m_dataBuffer.contains('\n')) {
        const int lineFeedPos = m_dataBuffer.indexOf('\n');
        const QString line = m_dataBuffer.left(lineFeedPos);
        processIncomingMessage(line);
        m_dataBuffer.remove(0, lineFeedPos + 1);
    }

//    // test if the last bits read forms a line, otherwise save the incomplete line for further reading
//    if (m_dataBuffer.at(m_dataBuffer.count()) == m_dataBuffer.lastIndexOf('\n')) { //data ends with a new line
//        // process each recieved line separately
//        Q_FOREACH(const QString &line, m_dataBuffer.split('\n', QString::SkipEmptyParts)) {
//            if (!line.isEmpty()) {
//                processIncomingMessage(line);
//            }
//        }
//        m_dataBuffer.clear();
//    }
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
    } else if (msg.contains(QLatin1String("OK")) || msg.contains(QLatin1String("ERROR"))) {
        sendNextCommandInLine();
    }

    QString withoutNewLine = msg;
    withoutNewLine.remove('\n');
    emit message(withoutNewLine, RawSerial);

    // ##### SIM PIN request #####
    if (msg.contains(QLatin1String("+CPIN: SIM PIN"))) {
        emit message(tr("GSM module requested PIN code, sending %1").arg(SIM_PIN), ATFormatted);
        requestSendCommand(QString("AT+CPIN=\"%1\"").arg(SIM_PIN));
        return;

    // ##### SIM PIN accepted, connection established #####
    } else if (msg.contains(QLatin1String("Call Ready"))) {
        // make sure we sent SMS in text mode
        requestSendCommand(QLatin1String("AT+CMGF=1"));

//    // ##### SMS prompt #####
//    } else if (msg.contains(QLatin1String(">")) && !m_SMSMessageText.isEmpty()) {
//        send(m_SMSMessageText);
//        emit message(tr("SMS sent"), ATFormatted);
//        m_SMSMessageText.clear();

    // ##### Call ready query response #####
    } else if (msg.contains(QLatin1String("+CCALR: 1"))) {
        emit message(tr("GSM module ready to transmit"), ATFormatted);
        sendNextCommandInLine();
    } else if (msg.contains(QLatin1String("+CCALR: 0"))) {
        emit message(tr("GSM module NOT ready to transmit"), ATFormatted);
        sendNextCommandInLine();

    // ##### SMS sent #####
    } else if (msg.contains(QLatin1String("+CMGS:"))) {
        emit message(tr("SMS sent"), ATFormatted);
        m_readyForNextSMS = true;
        sendNextSMS();
    }
}

void GSModule::sendNextCommandInLine()
{
    if (m_commandsQueue.isEmpty()) {
        return;
    }  else {
        // remove previous one
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

void GSModule::sendNextSMS()
{
    if (m_readyForNextSMS && !m_SMSMessagesList.isEmpty()) {
        m_nextSMS = m_SMSMessagesList.takeFirst();

        if (m_nextSMS.length() > SMS_MAX_LENGTH) {
            emit message(tr("The message you are trying to send contains %1 characters, only 160 are allowed.")
                         .arg(m_nextSMS.length()), Error);
            m_SMSPhoneNumberList.removeFirst();
            m_readyForNextSMS = true;
            return;
        }
        emit message(tr("Sending SMS: %1").arg(m_nextSMS), ATFormatted);


        // SMS request, we first make sure the SIM module is ready
        requestSendCommand(QString::fromLatin1("AT+CCALR?"));
        requestSendCommand(QString::fromLatin1("AT+CMGS=\"%1\"").arg(m_SMSPhoneNumberList.takeFirst()));
        m_readyForNextSMS = false;
    }
}

