#ifndef SIPUNCHTOSMS_H
#define SIPUNCHTOSMS_H

#include <QObject>

/**
 * @brief Connects to a Sport ident master unit (serial) and a GSM module (serial too) to emit SMS when somebody punch on the SI unit.
 */

class SIPunchToSMS : public QObject
{
    Q_OBJECT

public:
    explicit SIPunchToSMS(const QString &GSMPort = QLatin1String("/dev/ttyAMA0"), const QString &SIPort = QLatin1String("/dev/ttyUSB0"), QObject *parent = 0);
    
Q_SIGNALS:
    
public Q_SLOTS:
    
};

#endif // SIPUNCHTOSMS_H
