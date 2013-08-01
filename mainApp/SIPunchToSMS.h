#ifndef SIPUNCHTOSMS_H
#define SIPUNCHTOSMS_H

#include <QObject>

/**
 * @brief Connects to a Sport ident master unit (serial) and a GSM module (serial too) to emit SMS when somebody punch on the SI unit.
 */

class QTimer;
class GSModule;

class SIPunchToSMS : public QObject
{
    Q_OBJECT

public:
    explicit SIPunchToSMS(const QString &GSMPort = QLatin1String("/dev/ttyAMA0"), const QString &SIPort = QLatin1String("/dev/ttyUSB0"), QObject *parent = 0);

    /**
     * @brief Send a signal on the RST pin of the GSM module
     */
    void resetGSModule();

    /**
     * @brief Powers ON/OFF the module
     */
    void powerGSModule();

    /**
     * @brief Mapping between buttons
     */
    enum GSMModuleButtons {
        Power = 0,
        Reset = 0
    };

Q_SIGNALS:
    
public Q_SLOTS:

private Q_SLOTS:
    /**
     * @brief press/release "physical" buttons on the GSM module
     */
    void releaseButtons();

    /**
     * @brief press/release "physical" buttons on the GSM module
     */
    void GSModuleReady();

private:
    /**
     * @brief Holds wether the GSM Module is ready to transmit SMS or not
     */
    bool m_GSModuleIsReady;

    /**
     * @brief Timer for buttons pushing
     */
    QTimer *m_ButtonTimer;

    /**
     * @brief Serial connections
     */
    GSModule *m_GSModule;

    /**
     * @brief Connect to the GSM module serial.
     */
    void initialiseGSModule(const QString &port);
};

#endif // SIPUNCHTOSMS_H
