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
    QTest::newRow("IKV 204939-1") << QByteArray("\x02\x53\x10\x07\x2D\x10\x02\x10\x13\x4B\x10\x00\x4C\xE7\x10\x06\xA7\x6B\x03", 19 )
                                  << (quint32)204939 << (quint8)0x02 << (quint8)0x2D << QTime(17,28,07);

    // SI 204939, IKV, type 5, old protocol, punched at 17:28:28
    QTest::newRow("IKV 204939-2") << QByteArray("\x02\x53\x10\x07\x2D\x10\x02\x10\x13\x4B\x10\x00\x4C\xFC\x10\x00\xFD\x7F\x03", 19)
                                  << (quint32)204939 << (quint8)0x02 << (quint8)0x2D << QTime(17,28,28);

    // SI 204948, IKV, type 5, old protocol, punched at 9:36:39
    QTest::newRow("IKV 204948-1") << QByteArray("\x02\x53\x10\x08\x2D\x10\x02\x10\x13\x54\x10\x00\x87\x27\x10\x13\x10\x0D\x8D\x03", 20)
                                  << (quint32)204948 << (quint8)0x02 << (quint8)0x2D << QTime(9,36,39);

    // SI 204948, IKV, type 5, old protocol, punched at 9:37:28
    QTest::newRow("IKV 204948-2") << QByteArray("\x02\x53\x10\x08\x2D\x10\x02\x10\x13\x54\x10\x00\x87\x58\x10\x09\x8F\xD4\x03", 19)
                                  << (quint32)204948 << (quint8)0x02 << (quint8)0x2D << QTime(9,37,28);

    // SI 890510, Benoit Dumas, type 6, old protocol, punched at 17:26:35
    QTest::newRow("Benoit Dumas 890510-1") << QByteArray("\x02\x53\x10\x07\x2D\x10\x0D\x96\x8E\x10\x00\x4C\x8B\x10\x05\xFB\x82\x03", 18)
                                           << (quint32)890510 << (quint8)0x0D << (quint8)0x2D << QTime(17,26,35);

    // SI 890510, Benoit Dumas, type 6, old protocol, punched at 17:27:11
    QTest::newRow("Benoit Dumas 890510-2") << QByteArray("\x02\x53\x10\x07\x2D\x10\x0D\x96\x8E\x10\x00\x4C\xAF\x10\x0B\xA3\xA5\x03", 18)
                                           << (quint32)890510 << (quint8)0x0D << (quint8)0x2D << QTime(17,27,11);

    // SI 540022, Tim Dalheimer, type 6, old protocol, punched at 9:26:09
    QTest::newRow("Tim Dalheimer 540022-1") << QByteArray("\x02\x53\x10\x08\x2D\x10\x08\x3D\x76\x10\x00\x84\xB1\x10\x0A\xAC\x93\x03", 18)
                                            << (quint32)540022 << (quint8)0x08 << (quint8)0x2D << QTime(9,26,9);

    // SI 540022, Tim Dalheimer, type 6, old protocol, punched at 9:26:53
    QTest::newRow("Tim Dalheimer 540022-2") << QByteArray("\x02\x53\x10\x08\x2D\x10\x08\x3D\x76\x10\x00\x84\xDD\x10\x00\x44\xAA\x03", 18)
                                            << (quint32)540022 << (quint8)0x08 << (quint8)0x2D << QTime(9,26,53);

    // SI 700527, Kalle Dalheimer, type 6, old protocol, punched at 9:27:41
    QTest::newRow("Kalle Dalheimer 700527-1") << QByteArray("\x02\x53\x10\x08\x2D\x10\x0A\xB0\x6F\x10\x00\x85\x10\x0D\x10\x0A\xFA\x10\x12\x03", 20)
                      << (quint32)700527 << (quint8)0x0A << (quint8)0x2D << QTime(9,27,41);

    // SI 700527, Kalle Dalheimer, type 6, old protocol, punched at 9:27:54
    QTest::newRow("Kalle Dalheimer 700527-2") << QByteArray("\x02\x53\x10\x08\x2D\x10\x0A\xB0\x6F\x10\x00\x85\x10\x1A\x10\x03\x88\x24\x03", 19)
                      << (quint32)700527 << (quint8)0x0A << (quint8)0x2D << QTime(9,27,54);

    // SI 647656, Tanja Dalheimer, type 6, old protocol, punched at 9:28:54
    QTest::newRow("Tanja Dalheimer 647656-1") << QByteArray("\x02\x53\x10\x08\x2D\x10\x09\xE1\xE8\x10\x00\x85\x56\x10\x0E\x66\x10\x01\x03", 19)
                      << (quint32)647656 << (quint8)0x09 << (quint8)0x2D << QTime(9,28,54);

    // SI 647656, Tanja Dalheimer, type 6, old protocol, punched at 9:29:16
    QTest::newRow("Tanja Dalheimer 647656-2") << QByteArray("\x02\x53\x10\x08\x2D\x10\x09\xE1\xE8\x10\x00\x85\x6C\x10\x13\xFA\x4F\x03", 18)
                      << (quint32)647656 << (quint8)0x09 << (quint8)0x2D << QTime(9,29,16);

    // SI 684897, Jan Dalheimer, type 6, old protocol, punched at 9:30:03
    QTest::newRow("Jan Dalheimer 684897-1") << QByteArray("\x02\x53\x10\x08\x2D\x10\x0A\x73\x61\x10\x00\x85\x9B\x10\x03\x84\x31\x03", 18)
                      << (quint32)684897 << (quint8)0x0A << (quint8)0x2D << QTime(9,30,3);

    // SI 684897, Jan Dalheimer, type 6, old protocol, punched at 9:30:28
    QTest::newRow("Jan Dalheimer 684897-2") << QByteArray("\x02\x53\x10\x08\x2D\x10\x0A\x73\x61\x10\x00\x85\xB4\x10\x01\x66\x3D\x03", 18)
                      << (quint32)684897 << (quint8)0x0A << (quint8)0x2D << QTime(9,30,28);

    // SI 721209, Per-Martin Svärd, type 6, old protocol, punched at 9:31:18
    QTest::newRow("Per-Martin Svärd 721209-1") << QByteArray("\x02\x53\x10\x08\x2D\x10\x0B\x10\x01\x39\x10\x00\x85\xE6\x10\x01\x61\x10\x1C\x03", 20)
                      << (quint32)721209 << (quint8)0x0B << (quint8)0x2D << QTime(9,31,18);

    // SI 721209, Per-Martin Svärd, type 6, old protocol, punched at 9:32:34
    QTest::newRow("Per-Martin Svärd 721209-2") << QByteArray("\x02\x53\x10\x08\x2D\x10\x0B\x10\x01\x39\x10\x00\x86\x32\x10\x06\x99\x3E\x03", 19)
                      << (quint32)721209 << (quint8)0x0B << (quint8)0x2D << QTime(9,32,34);

    // SI 2058198, IKV, type 8, old protocol, punched at 17:27:48
    QTest::newRow("IKV 2058198-1") << QByteArray("\x02\x53\x10\x07\x2D\x10\x1F\x67\xD6\x10\x00\x4C\xD4\x10\x01\x48\x10\x03\x03", 19)
                      << (quint32)2058198 << (quint8)0x1F << (quint8)0x2D << QTime(17,27,48);

    // SI 2058198, IKV, type 8, old protocol, punched at 17:28:39
    QTest::newRow("IKV 2058198-2") << QByteArray("\x02\x53\x10\x07\x2D\x10\x1F\x67\xD6\x10\x00\x4D\x10\x07\x10\x00\xA2\x10\x1D\x03", 20)
                      << (quint32)2058198 << (quint8)0x1F << (quint8)0x2D << QTime(17,28,39);

    // SI 2058126, IKV, type 8, old protocol, punched at 9:38:01
    QTest::newRow("IKV 2058126-1") << QByteArray("\x02\x53\x10\x08\x2D\x10\x1F\x67\x8E\x10\x00\x87\x79\x10\x0E\x10\x13\xD4\x03", 19)
                      << (quint32)2058126 << (quint8)0x1F << (quint8)0x2D << QTime(9,38,1);

    // SI 2058126, IKV, type 8, old protocol, punched at 9:38:36
    QTest::newRow("IKV 2058126-2") << QByteArray("\x02\x53\x10\x08\x2D\x10\x1F\x67\x8E\x10\x00\x87\x9C\x10\x02\x4D\xF0\x03", 18)
                      << (quint32)2058126 << (quint8)0x1F << (quint8)0x2D << QTime(9,38,36);

    // SI 9183776, Sven-Åke Lundberg, type 11, old protocol, punched at 9:35:36
    QTest::newRow("Sven-Åke Lundberg 9183776-1") << QByteArray("\x02\x53\x10\x08\x2D\x8C\x22\x20\x10\x00\x86\xE8\x10\x12\x46\x20\x03", 17)
                      << (quint32)9183776 << (quint8)0x8C << (quint8)0x2D << QTime(9,35,36);

    // SI 9183776, Sven-Åke Lundberg, type 11, old protocol, punched at 9:36:07
    QTest::newRow("Sven-Åke Lundberg 9183776-2") << QByteArray("\x02\x53\x10\x08\x2D\x8C\x22\x20\x10\x00\x87\x10\x07\x10\x0D\x24\x7A\x03", 18)
                      << (quint32)9183776 << (quint8)0x8C << (quint8)0x2D << QTime(9,36,7);




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
