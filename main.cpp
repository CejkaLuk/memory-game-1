#include <QtGui>
#include <time.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{    
    QApplication::setApplicationName("memory-game");
    QApplication::setApplicationVersion("1.0");

    QTranslator t;
    t.load(QApplication::applicationName() + "_" + QLocale::system().name(), ":/translations");

    QApplication a(argc, argv);
    QApplication::installTranslator(&t);

    MainWindow w;
    w.show();
    qsrand(time(0));

    return a.exec();
}
