#ifndef GINEEDHELPDIALOG_H
#define GINEEDHELPDIALOG_H

#include <QDialog>

namespace Ui {
class GINeedHelpDialog;
}

class GINeedHelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GINeedHelpDialog(QWidget *parent = 0);
    ~GINeedHelpDialog();

private:
    Ui::GINeedHelpDialog *ui;
};

#endif // GINEEDHELPDIALOG_H
