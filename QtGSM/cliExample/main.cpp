#include <QCoreApplication>
#include "autoSMS.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    autoSMS autosms;

    return a.exec();
}

