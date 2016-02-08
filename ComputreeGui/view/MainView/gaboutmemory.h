#ifndef GABOUTMEMORY_H
#define GABOUTMEMORY_H

#include <QWidget>

namespace Ui {
class GAboutMemory;
}

class CT_AbstractCloud;
class CT_AbstractCloudRegistered;
class CT_AbstractCloudIndexRegistered;

class GAboutMemory : public QWidget
{
    Q_OBJECT

public:
    explicit GAboutMemory(QWidget *parent = 0);
    ~GAboutMemory();

private:
    Ui::GAboutMemory *ui;

    void updateUI();

    void setRowForCloud(QString type, int row, CT_AbstractCloud *cloud, size_t &totalUsed);
    void setRowForCloudIndexRegistered(QString type, int row, QList<CT_AbstractCloudIndexRegistered*> cloudIndex, size_t &totalUsed);
    void setRowForCloudsRegistered(QString type, int row, QList<CT_AbstractCloudRegistered*> clouds, size_t &totalUsed);

protected:
    void showEvent(QShowEvent *e);
};

#endif // GABOUTMEMORY_H
