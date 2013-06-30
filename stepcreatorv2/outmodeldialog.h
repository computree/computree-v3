#ifndef OUTModelDialog_H
#define OUTModelDialog_H

#include <QDialog>

namespace Ui {
    class OUTModelDialog;
}

class OUTModelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OUTModelDialog(QWidget *parent = 0);
    ~OUTModelDialog();

private:
    Ui::OUTModelDialog *ui;
};

#endif // OUTModelDialog_H
