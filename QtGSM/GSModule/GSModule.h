#ifndef GSMMODULE_H
#define GSMMODULE_H

#include <QObject>
#include <QTimer>
#include <QStringList>

class QSerialPort;

/**
 * @brief A class that enables communication with a serial GSM module.
 */

class GSModule : public QObject
{
    Q_OBJECT
public:
    explicit GSModule(QObject *parent = 0, bool debugOutputEnabled = false);

    /**
     * @brief Open the serial console with the specified serialPort.
     */
    void initializeSerialConnection(QSerialPort *serialPort);

    /**
     * @brief Sends a line on the serial port,
     *      the CR is not needed here, it is automatically added before sending
     * @param line The line to send
     */
    void send(const QString &line);

    /**
     * @brief The different messages types
     */
    enum MessageType {
        DebugInformation = 0,
        Error,
        ATFormatted,
        RawSerial
    };

    /**
     * @brief Send a SMS
     * @param message: the SMS message to send (160 chars limit)
     * @param phoneNumber: A string representing the recipient's phone number.
     * @note the phone number must comply to the E.123 standard:
     *               http://en.wikipedia.org/wiki/E.123
     */
    void sendSMS(const QString &message, const QString &phoneNumber);

Q_SIGNALS:
    /**
     * @brief Sends a message with information on the module
     * @param message
     * @param The type of message (see @MessageType)
     */
    void message(const QString &message, GSModule::MessageType type);

    /**
     * @brief Emitted when the module is ready to transmit data.
     */
    void readyToTransmit();


private Q_SLOTS:
    void serialDataAvailable();
    void messageAsDebug(const QString &message, GSModule::MessageType type);

    /**
     * @brief Send the next command in the command queue
     * @note declared as slot so it can be triggered by the command timer
     */
    void sendNextCommandInLine();

private:
    QSerialPort *m_serialPort;

    /**
     * @brief First check for incoming message, check if it's a AT command or if message is part of a bigger response
     */
    void processIncomingMessage(const QString &msg);

    /**
     * @brief Used to store the currently recieved data to be handled
     */
    QString m_dataBuffer;

    /**
     * @brief store the queue of SMS messages to be sent. each message can only be send after the previous one was sent.
     * Using the m_readyForNextSMS boolean
     */
    QStringList m_SMSMessagesList;
    QStringList m_SMSPhoneNumberList;
    bool m_readyForNextSMS;
    QString m_nextSMS;

    /**
     * @brief timer to delay reception of data. Usefull when sending commands where the SIM is
     *      expected ot take time to answer.
     */
    QTimer m_tempoTimer;

    /**
     * @brief For most AT commands, the SIM will answer "OK" or "ERROR". When a AT command is issued
     * we wait for the OK/ERROR answer before sending more commands
     */
    QStringList m_commandsQueue;
    void requestSendCommand(const QString &command);

    /**
     * @brief Safety timer, if no answer is recieved (OK/ERROR) after 30 seconds, send the next command in line
     */
    QTimer m_commandQueueTimer;


    /**
      * display message in debug output
      */
    bool m_debugOutputEnabled;

    /**
     * @brief sends the next SMS in line
     */
    void sendNextSMS();

};
Q_DECLARE_METATYPE(GSModule::MessageType)
#endif // GSMMODULE_H
