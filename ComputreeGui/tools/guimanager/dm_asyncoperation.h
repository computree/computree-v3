#ifndef DM_ASYNCOPERATION_H
#define DM_ASYNCOPERATION_H

#include "imainprogressdialog.h"

class DM_AsyncOperation : public QObject
{
    Q_OBJECT

public:
    DM_AsyncOperation();

    void setProgressDialog(const IMainProgressDialog *dialog);
    IMainProgressDialog* progressDialog() const;

private:
    IMainProgressDialog *m_progressDialog;

public slots:
    void setProgress(const int &progress);
    void setProgressRange(const int &min, const int &max);

signals:
    void cancel();
};

#endif // DM_ASYNCOPERATION_H
