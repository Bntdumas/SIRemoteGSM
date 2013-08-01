#ifndef SIPUNCHTOSMS_H
#define SIPUNCHTOSMS_H

#include <QObject>

/**
 * @brief Connects to a Sport ident master unit (serial) and a GSM module (serial too) to emit SMS when somebody punch on the SI unit.
 */

class QTimer;

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

private:
    QTimer *m_ButtonTimer;
};

#endif // SIPUNCHTOSMS_H
