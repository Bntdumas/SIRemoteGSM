#include <QCoreApplication>
#include "SIPunchToSMS.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SIPunchToSMS SiSms;
    return a.exec();
}
