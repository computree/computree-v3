#ifndef GLOGWIDGET_H
#define GLOGWIDGET_H

#include "ct_log/abstract/ct_abstractloglistener.h"

#include <QWidget>
#include <QReadWriteLock>

namespace Ui {
class GLogWidget;
}

class GLogWidget : public QWidget, public CT_AbstractLogListener
{
    Q_OBJECT

public:
    explicit GLogWidget(QWidget *parent = 0);
    ~GLogWidget();

public slots:
    void addMessage(const int &severity, const int &type, const QString &s, const QString &filter = "");

private:
    Ui::GLogWidget              *ui;
    QList<QPair<int, QString> > m_messages;
    QReadWriteLock              m_mutex;
    QHash<int, QColor>          m_colors;

    const static QString        BEGIN_HMTL;
    const static QString        END_HMTL;

private slots:
    void slotNewMessageReceived();
    void on_pushButtonClear_clicked();

signals:
    void newMessageReceived();
};

#endif // GLOGWIDGET_H
