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
    explicit SIPunchToSMS(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // SIPUNCHTOSMS_H
