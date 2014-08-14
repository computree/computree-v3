#ifndef GREDMINEPARAMETERSDIALOG_H
#define GREDMINEPARAMETERSDIALOG_H

#include <QDialog>

namespace Ui {
class GRedmineParametersDialog;
}

class GRedmineParametersDialog : public QDialog
{
    Q_OBJECT

public:

    enum SystemType {
        Windows,
        Linux
    };

    explicit GRedmineParametersDialog(QWidget *parent = 0);
    ~GRedmineParametersDialog();

    QString getDirectory();
    QString getId();
    QString getPassword();
    bool isProxyActivated();
    QString getProxyAdress();
    QString getProxyId();
    QString getProxyPassword();    
    GRedmineParametersDialog::SystemType getSystemType();


private slots:
    void on_pb_directory_clicked();

private:
    Ui::GRedmineParametersDialog *ui;
};

#endif // GREDMINEPARAMETERSDIALOG_H
