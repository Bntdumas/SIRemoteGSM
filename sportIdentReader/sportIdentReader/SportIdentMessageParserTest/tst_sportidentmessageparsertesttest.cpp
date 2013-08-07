#include <QString>
#include <QtTest>

#include "../sportidentmessageparser.h"

class SportIdentMessageParserTest : public QObject
{
    Q_OBJECT
    
public:
    SportIdentMessageParserTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testSI5();
    void testSI6();
    void testSI8();
};

SportIdentMessageParserTest::SportIdentMessageParserTest()
{
}

void SportIdentMessageParserTest::initTestCase()
{
}

void SportIdentMessageParserTest::cleanupTestCase()
{
}

void SportIdentMessageParserTest::testSI5()
{
    const quint8 rawData1[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x02, 0x10, 0x13, 0x4B, 0x10, 0x00, 0x4C, 0xE7, 0x10, 0x06, 0xA7, 0x6B, 0x03
    };
    QByteArray inMsg1( (char*)rawData1, 19 );
    SportIdentMessageParser::SportIdentMessage out;
    SportIdentMessageParser::parse(inMsg1, out);
    QVERIFY2( out.cardNumber == 0, "Card number parsing failed" );
    QVERIFY2( out.cardSeries == 0, "Card series parsing failed");
    QVERIFY2( out.stationID == 0, "Station ID parsing failed");
    QVERIFY2( out.punchTime == QTime(), "Punch time parsing failed");

    const quint8 rawData2[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x02, 0x10, 0x13, 0x4B, 0x10, 0x00, 0x4C, 0xFC, 0x10, 0x00, 0xFD, 0x7F, 0x03
    };
    QByteArray inMsg2( (char*)rawData2, 19 );
    SportIdentMessageParser::parse(inMsg2, out);
    QVERIFY2( out.cardNumber == 0, "Card number parsing failed" );
    QVERIFY2( out.cardSeries == 0, "Card series parsing failed");
    QVERIFY2( out.stationID == 0, "Station ID parsing failed");
    QVERIFY2( out.punchTime == QTime(), "Punch time parsing failed");
}


void SportIdentMessageParserTest::testSI6()
{
    const quint8 rawData1[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x0D, 0x96, 0x8E, 0x10, 0x00, 0x4C, 0x8B, 0x10, 0x05, 0xFB, 0x82, 0x03
    };
    QByteArray inMsg1( (char*)rawData1, 18 );
    SportIdentMessageParser::SportIdentMessage out;
    SportIdentMessageParser::parse(inMsg1, out);
    QVERIFY2( out.cardNumber == 0, "Card number parsing failed" );
    QVERIFY2( out.cardSeries == 0, "Card series parsing failed");
    QVERIFY2( out.stationID == 0, "Station ID parsing failed");
    QVERIFY2( out.punchTime == QTime(), "Punch time parsing failed");

    const quint8 rawData2[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x0D, 0x96, 0x8E, 0x10, 0x00, 0x4C, 0xAF, 0x10, 0x0B, 0xA3, 0xA5, 0x03
    };
    QByteArray inMsg2( (char*)rawData2, 18 );
    SportIdentMessageParser::parse(inMsg2, out);
    QVERIFY2( out.cardNumber == 0, "Card number parsing failed" );
    QVERIFY2( out.cardSeries == 0, "Card series parsing failed");
    QVERIFY2( out.stationID == 0, "Station ID parsing failed");
    QVERIFY2( out.punchTime == QTime(), "Punch time parsing failed");
}


void SportIdentMessageParserTest::testSI8()
{
    const quint8 rawData1[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x1F, 0x67, 0xD6, 0x10, 0x00, 0x4C, 0xD4, 0x10, 0x01, 0x48, 0x10, 0x03, 0x03
    };
    QByteArray inMsg1( (char*)rawData1, 19 );
    SportIdentMessageParser::SportIdentMessage out;
    SportIdentMessageParser::parse(inMsg1, out);
    QVERIFY2( out.cardNumber == 0, "Card number parsing failed" );
    QVERIFY2( out.cardSeries == 0, "Card series parsing failed");
    QVERIFY2( out.stationID == 0, "Station ID parsing failed");
    QVERIFY2( out.punchTime == QTime(), "Punch time parsing failed");

    const quint8 rawData2[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x1F, 0x67, 0xD6, 0x10, 0x00, 0x4D, 0x10, 0x07, 0x10, 0x00, 0xA2, 0x10, 0x1D, 0x03
    };
    QByteArray inMsg2( (char*)rawData2, 19 );
    SportIdentMessageParser::parse(inMsg2, out);
    QVERIFY2( out.cardNumber == 0, "Card number parsing failed" );
    QVERIFY2( out.cardSeries == 0, "Card series parsing failed");
    QVERIFY2( out.stationID == 0, "Station ID parsing failed");
    QVERIFY2( out.punchTime == QTime(), "Punch time parsing failed");
}




QTEST_APPLESS_MAIN(SportIdentMessageParserTest)

#include "tst_sportidentmessageparsertesttest.moc"
