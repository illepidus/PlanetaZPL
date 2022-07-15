#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Planeta Dezavid");
    QCoreApplication::setApplicationName("PlanetaZPL");

    MainWindow w;
    w.show();

    QObject::connect(&w, SIGNAL(quit()), &a, SLOT(quit()));

    return a.exec();
}
