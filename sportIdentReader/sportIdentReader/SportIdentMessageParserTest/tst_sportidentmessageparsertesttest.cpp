#include <QString>
#include <QtTest>

class SportIdentMessageParserTest : public QObject
{
    Q_OBJECT
    
public:
    SportIdentMessageParserTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
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

void SportIdentMessageParserTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SportIdentMessageParserTest)

#include "tst_sportidentmessageparsertesttest.moc"
