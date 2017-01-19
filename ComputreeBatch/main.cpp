#include <QtGui>
#include <QApplication>
#include <QMessageBox>

#include "batchdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStringList args = QCoreApplication::arguments();

    bool useTrayIcon = false;

    if(!args.contains("--fullbatch")) {

        useTrayIcon = true;
        if (!QSystemTrayIcon::isSystemTrayAvailable()) {
            QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system. Use \"ComputreeBatch.exe --fullbatch\" to not use the system tray."));
            return 1;
        }
        QApplication::setQuitOnLastWindowClosed(false);
    }

    BatchDialog dialog(useTrayIcon);

    if(!useTrayIcon)
        dialog.show();


    try {
        return app.exec();
    }
    catch(...) {
       //throw;
        return 0;
    }
    return 0;
}

