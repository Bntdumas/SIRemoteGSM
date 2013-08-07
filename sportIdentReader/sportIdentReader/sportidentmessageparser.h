#ifndef SPORTIDENTMESSAGEPARSER_H
#define SPORTIDENTMESSAGEPARSER_H

#include <QString>
#include <QTime>

class SportIdentMessageParser
{
private:
    SportIdentMessageParser() {}

    static const quint8 STX=0x02;
    static const quint8 ETX=0x03;
    static const quint8 ACK=0x06;
    static const quint8 DLE=0x10;

    static bool checkCRC(const QByteArray&);
    static quint16 calcCRC(const QByteArray& data, quint32 count);


public:
    struct SportIdentMessage {
        quint32 cardNumber;
        quint8 cardSeries;
        quint8 stationID;
        QTime punchTime;
    };

    static bool parse(const QByteArray& inMsg, SportIdentMessage& out);
};

#endif // SPORTIDENTMESSAGEPARSER_H
