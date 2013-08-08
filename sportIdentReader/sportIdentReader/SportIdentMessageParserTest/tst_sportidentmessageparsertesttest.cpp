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
    void testSI();
    void testSI_data();
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


void SportIdentMessageParserTest::testSI_data()
{
    QTest::addColumn<QByteArray>("frames");
    QTest::addColumn<quint32>("cardNumber");
    QTest::addColumn<quint8>("cardSeries");
    QTest::addColumn<quint8>("stationID");
    QTest::addColumn<QTime>("punchTime");

    // SI 204939, IKV, type 5, old protocol, punched at 17:28:07
    const quint8 rawData1[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x02, 0x10, 0x13, 0x4B, 0x10, 0x00, 0x4C, 0xE7, 0x10, 0x06, 0xA7, 0x6B, 0x03
    };
    QByteArray frames1( (char*)rawData1, 19 );
    QTest::newRow("IKV 204939-1") << frames1 << (quint32)204939 << (quint8)0x02 << (quint8)0x2D << QTime(17,28,07);

    // SI 204939, IKV, type 5, old protocol, punched at 17:28:28
    const quint8 rawData2[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x02, 0x10, 0x13, 0x4B, 0x10, 0x00, 0x4C, 0xFC, 0x10, 0x00, 0xFD, 0x7F, 0x03
    };
    QByteArray frames2( (char*)rawData2, 19 );
    QTest::newRow("IKV 204939-2") << frames2 << (quint32)204939 << (quint8)0x02 << (quint8)0x2D << QTime(17,28,28);

    // SI 890510, Benoit Dumas, type 6, old protocol, punched at 17:26:35
    const quint8 rawData3[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x0D, 0x96, 0x8E, 0x10, 0x00, 0x4C, 0x8B, 0x10, 0x05, 0xFB, 0x82, 0x03
    };
    QByteArray frames3( (char*)rawData3, 18 );
    QTest::newRow("Benoit Dumas 890510") << frames3 << (quint32)890510 << (quint8)0x0D << (quint8)0x2D << QTime(17,26,35);
}


void SportIdentMessageParserTest::testSI()
{
    QFETCH(QByteArray, frames);
    QFETCH(quint32, cardNumber);
    QFETCH(quint8, cardSeries);
    QFETCH(quint8, stationID);
    QFETCH(QTime, punchTime);

    SportIdentMessageParser::SportIdentMessage out;
    SportIdentMessageParser::parse(frames, out);

    QCOMPARE( out.cardNumber, cardNumber );
    QCOMPARE( out.cardSeries, cardSeries );
    QCOMPARE( out.stationID, stationID );
    QCOMPARE( out.punchTime, punchTime );
}



#ifdef K
void SportIdentMessageParserTest::testSI8()
{
    const quint8 rawData1[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x1F, 0x67, 0xD6, 0x10, 0x00, 0x4C, 0xD4, 0x10, 0x01, 0x48, 0x10, 0x03, 0x03
    };
    QByteArray inMsg1( (char*)rawData1, 19 );
    SportIdentMessageParser::SportIdentMessage out;
    SportIdentMessageParser::parse(inMsg1, out);
    QVERIFY2( out.cardNumber == 2058198, "Card number parsing failed" );
    QVERIFY2( out.cardSeries == 0x1F, "Card series parsing failed");
    QVERIFY2( out.stationID == 0x2D, "Station ID parsing failed");
    QVERIFY2( out.punchTime == QTime(), "Punch time parsing failed");

    const quint8 rawData2[] = {
        0x02, 0x53, 0x10, 0x07, 0x2D, 0x10, 0x1F, 0x67, 0xD6, 0x10, 0x00, 0x4D, 0x10, 0x07, 0x10, 0x00, 0xA2, 0x10, 0x1D, 0x03
    };
    QByteArray inMsg2( (char*)rawData2, 20 );
    SportIdentMessageParser::parse(inMsg2, out);
    QVERIFY2( out.cardNumber == 2058198, "Card number parsing failed" );
    QVERIFY2( out.cardSeries == 0x1F, "Card series parsing failed");
    QVERIFY2( out.stationID == 0x2D, "Station ID parsing failed");
    QVERIFY2( out.punchTime == QTime(), "Punch time parsing failed");
}
#endif



QTEST_APPLESS_MAIN(SportIdentMessageParserTest)

#include "tst_sportidentmessageparsertesttest.moc"
