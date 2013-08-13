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

private:
    QSerialPort *m_serialPort;

    /**
     * @brief First check for incoming message, check if it's a AT command or if message is part of a bigger response
     */
    void processIncomingMessage(const QString &msg);

    /**
     * @brief Used to store the currently recieved data when waiting for more.
     */
    QString m_dataBuffer;

    /**
     * @brief When bytes are available, wait until the SIM module is done transmitting data.
     */
    bool m_waitingForEndOfTransmission;
    QTimer m_transmissionWaitTimer;

    /**
     * @brief We need to wait for the SMS module to answer the SMS request before sending it.
     * We store the message in the meantime.
     */
    QString m_SMSMessageText;

    /**
     * @brief timer to delay reception of data. Usefull when sending commands where the SIM is
     *      expected ot take time to answer.
     */
    QTimer m_tempoTimer;

    /**
     * @brief FOr most AT commands, the SIM will answer "OK" or "ERROR". When a AT command is issued
     * we wait for the OK/ERROR answer before sending more commands
     */
    QStringList m_commandsQueue;
    void sendNextCommandInLine();
    void requestSendCommand(const QString &command);

    /**
      * display message in debug output
      */
    bool m_debugOutputEnabled;
};
Q_DECLARE_METATYPE(GSModule::MessageType)
#endif // GSMMODULE_H
