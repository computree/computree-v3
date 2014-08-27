#include "glogwidget.h"
#include "ui_glogwidget.h"

#include "interfaces.h"

const QString GLogWidget::BEGIN_HMTL = "<html>";
const QString GLogWidget::END_HMTL = "<br/></html>";

GLogWidget::GLogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GLogWidget)
{
    ui->setupUi(this);

    m_colors.insert(LogInterface::trace, Qt::black);
    m_colors.insert(LogInterface::debug, Qt::darkGray);
    m_colors.insert(LogInterface::info, QColor(0, 162, 232));
    m_colors.insert(LogInterface::warning, QColor(228, 160, 3));
    m_colors.insert(LogInterface::error, Qt::red);

    ui->textEdit->setReadOnly(true);

    connect(this, SIGNAL(newMessageReceived()), this, SLOT(slotNewMessageReceived()), Qt::QueuedConnection);
}

GLogWidget::~GLogWidget()
{
    delete ui;
}

void GLogWidget::addMessage(const int &severity, const int &type, const QString &s, const QString &filter)
{
    Q_UNUSED(type)
    Q_UNUSED(filter)

    m_mutex.lockForWrite();
    m_messages.append(qMakePair(severity, s));
    m_mutex.unlock();

    if(isVisible())
        emit newMessageReceived();
}

void GLogWidget::on_pushButtonClear_clicked()
{
    ui->textEdit->clear();
}

void GLogWidget::slotNewMessageReceived()
{
    m_mutex.lockForRead();
    QPair<int, QString> pair = m_messages.takeFirst();
    QColor col = m_colors.value(pair.first, Qt::black);
    ui->textEdit->insertHtml(BEGIN_HMTL + "<font color=\"" + col.name() + "\">" + pair.second + "</font>" + END_HMTL);
    m_mutex.unlock();
}
