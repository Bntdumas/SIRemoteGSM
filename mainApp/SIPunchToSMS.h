#ifndef SIPUNCHTOSMS_H
#define SIPUNCHTOSMS_H

#include <QObject>

/**
 * @brief Connects to a Sport ident master unit (serial) and a GSM module (serial too) to emit SMS when somebody punch on the SI unit.
 */

class QTimer;
class GSModule;
class SportIdentReader;

class SIPunchToSMS : public QObject
{
    Q_OBJECT

public:
    explicit SIPunchToSMS(const QString &GSMPort = QLatin1String("ttyAMA0"), const QString &SIPort = QLatin1String("ttyUSB0"), QObject *parent = 0);

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
        Reset = 1
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

    /**
     * @brief Connect the punch to sending a SMS
     * @param SInumber the SI card number
     * @param punchTimeSTamp the punch timestamp
     */
    void punchToSMS(const int SInumber, const QTime &punchTimeSTamp);

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
    SportIdentReader *m_SIReader;

    /**
     * @brief Connect to the GSM/SI serial.
     */
    void initialiseGSModule(const QString &port);
    void initialiseSportIdentReader(const QString &port);

};

#endif // SIPUNCHTOSMS_H
