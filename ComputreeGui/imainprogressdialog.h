#ifndef IMAINPROGRESSDIALOG_H
#define IMAINPROGRESSDIALOG_H

#include <QDialog>

class IMainProgressDialog : public QDialog
{
    Q_OBJECT

public:
    IMainProgressDialog(QWidget *parent = NULL, Qt::WindowFlags flags = 0) : QDialog(parent, flags) {}
    virtual ~IMainProgressDialog() {}

public slots:
    virtual void setCanClose(bool enable) = 0;
    virtual void enableCanClose() = 0;
    virtual void disableCanClose() = 0;

    virtual void setLabelText(QString text) = 0;
    virtual void setValue(int progress) = 0;

    virtual void setSecondLabelText(QString text) = 0;
    virtual void setSecondValue(int progress) = 0;

    virtual void enableCancelButton(bool enable) = 0;
    virtual void setCancelButtonText(const QString &text) = 0;

signals:
    void cancel();
};

#endif // IMAINPROGRESSDIALOG_H
