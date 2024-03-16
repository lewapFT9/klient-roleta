#include "roleta.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Roleta w;
    w.show();
    return a.exec();
}
