#include <QApplication>
#include "mainwindow.h"
#include "tools.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tools::initItemTypes();
    MainWindow w;
    w.show();

    return a.exec();
}
