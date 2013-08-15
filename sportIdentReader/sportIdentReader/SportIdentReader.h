#ifndef SPORTIDENTREADER_H
#define SPORTIDENTREADER_H

#include "Sportidentreader_global.h"
#include <QSerialPort>
#include <QTimer>

/**
 * @brief Recieve data from a Sport Ident Control (USB/Serial)
 * and emits a signal with the Sport Ident number and the punch time.
 */

class SPORTIDENTREADERSHARED_EXPORT SportIdentReader: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString serialPort READ serialPort WRITE setserialPort)
    Q_PROPERTY(int serialSpeed READ serialSpeed WRITE setserialSpeed)

public:
    explicit SportIdentReader(QObject *parent = 0, bool debugOutputEnabled = false);

    /**
     * @brief Opens the serial connection to the Sport Ident module
     * @param port (optionnal if m_port is already set) the serial port for the connection
     * @param port (optionnal if m_serialSpeed is already set) the serial speed (38400 or 4800 bauds/s)
     */
    void initSerialConnection(const QString & port = QString(), const int serialSpeed = 0);

    void setserialPort(const QString & port);
    inline QString serialPort() { return m_port; }

    void setserialSpeed(const int serialSpeed);
    inline int serialSpeed() { return m_serialSpeed; }


    /**
     * @brief The different messages types
     */
    enum MessageType {
        DebugInformation = 0,
        Error,
        Warning,
        RawSerial
    };

private Q_SLOTS:
    /**
     * @brief reach when data is available on the open port
     */
    void serialDataAvailable();

    void messageAsDebug(const QString &message, SportIdentReader::MessageType type);


Q_SIGNALS:
    /**
     * @brief Emitted when the class wants to communicate with the exterior.
     * @param str: a message
     * @param type: the message type (see @ref MessageType)
     */
    void logText(const QString &str, SportIdentReader::MessageType type);

    /**
     * @brief Emitted when a punch is detected on the SI unit
     * @param The SI card number who punched
     * @param Timestamp of the punch, according to the SI Unit time
     */
    void SIPunch(const int SICardNUmber, const QTime &punchDateStamp);

    /**
     * @brief Emitted when a punch is detected, no parsing is done on this data
     * @param The raw data as received from the unit
     */
    void rawData(const QByteArray& data);

private:

    QSerialPort *m_serialPort;
    bool m_connected;

    /**
     * @brief m_port, the serial port to connect to.
     */
    QString m_port;

    /**
     * @brief m_serialSpeed
     */

    int m_serialSpeed;
    /**
     * @brief Used to store the currently recieved data when waiting for more.
     */
    QByteArray m_dataBuffer;

    /**
     * @brief When bytes are available, wait until the SIM module is done transmitting data.
     */
    bool m_waitingForEndOfTransmission;
    QTimer m_transmissionWaitTimer;

    /**
     * @brief process the raw data recieved, checks that the packet is valid and process the data
     * @param the recieved byteArray
     */
    void processIncomingMessage(const QByteArray &msg);

    /**
     * @brief Provides a nice way of visualising recieved datagrams
     * @param The raw array
     * @return a formatted string
     */
    QString byteArrayToHexaString(const QByteArray &array) const;


    /**
     * @brief remove the 0x10 characters in the array
     * @param The raw array
     * @return a formatted string
     */
    QByteArray removeSeparationCharacters(const QByteArray &array) const;

    /**
      * display message in debug output
      */
    bool m_debugOutputEnabled;

};
Q_DECLARE_METATYPE(SportIdentReader::MessageType)

#endif // SPORTIDENTREADER_H
