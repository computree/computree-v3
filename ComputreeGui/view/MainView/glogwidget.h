#ifndef GLOGWIDGET_H
#define GLOGWIDGET_H

#include "interfaces.h"
#include <QWidget>
#include <QReadWriteLock>

namespace Ui {
class GLogWidget;
}

class GLogWidget : public QWidget, public ILogListener
{
    Q_OBJECT
    Q_INTERFACES(ILogListener)

public:
    explicit GLogWidget(QWidget *parent = 0);
    ~GLogWidget();

public slots:
    void addMessage(const int &severity, const int &type, const QString &s, const QString &filter = "");
    void on_pushButtonClear_clicked();

private:
    Ui::GLogWidget              *ui;
    QList<QPair<int, QString> > m_messages;
    QReadWriteLock              m_mutex;
    QHash<int, QColor>          m_colors;

    const static QString        BEGIN_HMTL;
    const static QString        END_HMTL;

private slots:
    void slotNewMessageReceived();

signals:
    void newMessageReceived();
};

#endif // GLOGWIDGET_H
