#include <QtGui/QApplication>
#include <time.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qsrand(time(0));

    return a.exec();
}
