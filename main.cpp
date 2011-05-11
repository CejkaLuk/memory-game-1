#include <QtGui>
#include "mainwindow.h"

int main(int argc, char *argv[])
{    
    QApplication::setApplicationName("memory-game");
    QApplication::setApplicationVersion("1.0");

    QTranslator tQt, tApp;
    tQt.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    tApp.load(QApplication::applicationName() + "_" + QLocale::system().name(), ":/translations");

    QApplication a(argc, argv);
    QApplication::installTranslator(&tQt);
    QApplication::installTranslator(&tApp);

    MainWindow w;
    w.show();

    return a.exec();
}
