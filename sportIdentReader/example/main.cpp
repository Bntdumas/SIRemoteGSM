#include "punchVisualiser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    punchVisualiser w;
    w.show();
    
    return a.exec();
}
