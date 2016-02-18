#include "ggradientmanager.h"
#include "ui_ggradientmanager.h"

#include <QWidgetAction>
#include <QMenu>

#include "view/DocumentView/GraphicsViews/ColorGradient/gcolorgradientview.h"

GGradientManager::GGradientManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGradientManager)
{
    ui->setupUi(this);

    ui->toolButton->setPopupMode(QToolButton::InstantPopup);
    ui->toolButton->setMenu(new QMenu(this));

    QColor special;

    // AUTUMN
    QLinearGradient gr = QLinearGradient(0, 0, 1, 0);
    gr.setColorAt(0, Qt::red);
    gr.setColorAt(1, Qt::yellow);

    addGradient(gr);

    // GRAY
    gr = QLinearGradient(0, 0, 1, 0);
    gr.setColorAt(0, Qt::black);
    gr.setColorAt(1, Qt::white);

    addGradient(gr);

    // COOL
    gr = QLinearGradient(0, 0, 1, 0);
    gr.setColorAt(0, Qt::cyan);
    gr.setColorAt(1, Qt::magenta);

    addGradient(gr);

    // HOT
    gr = QLinearGradient(0, 0, 1, 0);
    gr.setColorAt(0, Qt::black);
    gr.setColorAt(0.25, Qt::red);
    gr.setColorAt(0.75, Qt::yellow);
    gr.setColorAt(1, Qt::white);

    addGradient(gr);

    // HSV
    gr = QLinearGradient(0, 0, 1, 0);
    gr.setColorAt(0, Qt::red);
    gr.setColorAt(0.166, Qt::yellow);
    gr.setColorAt(0.333, Qt::green);
    gr.setColorAt(0.5, Qt::cyan);
    gr.setColorAt(0.666, Qt::blue);
    gr.setColorAt(0.833, Qt::magenta);
    gr.setColorAt(1, Qt::red);

    addGradient(gr);

    // JET
    gr = QLinearGradient(0, 0, 1, 0);
    gr.setColorAt(0, Qt::blue);
    gr.setColorAt(0.333, Qt::cyan);
    gr.setColorAt(0.666, Qt::yellow);
    gr.setColorAt(1, Qt::red);

    addGradient(gr);

    // RAINBOW
    gr = QLinearGradient(0, 0, 1, 0);
    gr.setColorAt(0, Qt::red);
    gr.setColorAt(0.4, Qt::yellow);
    gr.setColorAt(0.6, Qt::green);
    gr.setColorAt(0.8, Qt::blue);
    gr.setColorAt(1, Qt::magenta);

    addGradient(gr);

    // SPRING
    gr = QLinearGradient(0, 0, 1, 0);
    gr.setColorAt(0, Qt::magenta);
    gr.setColorAt(1, Qt::yellow);

    addGradient(gr);

    // SUMMER
    gr = QLinearGradient(0, 0, 1, 0);
    special.setRgbF(0, 0.5, 0.4);
    gr.setColorAt(0, special);
    special.setRgbF(1, 1, 0.4);
    gr.setColorAt(1, special);

    addGradient(gr);

    // WINTER
    gr = QLinearGradient(0, 0, 1, 0);
    gr.setColorAt(0, Qt::blue);
    special.setRgbF(0, 1, 0.5);
    gr.setColorAt(1, special);

    addGradient(gr);

    setGradientSelected(0);
}

GGradientManager::~GGradientManager()
{
    delete ui;
}

void GGradientManager::addGradient(const QLinearGradient &gradient)
{
    QWidget *pWid = new QWidget(NULL);

    QVBoxLayout *layout = new QVBoxLayout(pWid);
    layout->setContentsMargins(0, 2, 0, 2);

    GColorGradientView *widget = new GColorGradientView(NULL);
    widget->fromLinearGradient(gradient);
    widget->setArrowsVisible(false);

    widget->installEventFilter(this);
    widget->setMinimumWidth(250);

    layout->addWidget(widget);

    QWidgetAction *action = new QWidgetAction(ui->toolButton->menu());
    action->setDefaultWidget(pWid);

    ui->toolButton->menu()->addAction(action);

    m_gradientViews.append(GradientWidgetInfo(pWid, widget));
}

void GGradientManager::setGradientSelected(int i)
{
    QMutableListIterator<GradientWidgetInfo> it(m_gradientViews);

    int n = 0;

    while(it.hasNext()) {
        GradientWidgetInfo info = it.next();

        info.setSelected(i == n);

        if(info.isSelected())
            emit newGradientSelected(info.m_gradientView->toLinearGradient());

        it.setValue(info);

        ++n;
    }

    updateToolButtonIcon();
}

QLinearGradient GGradientManager::gradientSelected() const
{
    QLinearGradient gr;

    QListIterator<GradientWidgetInfo> it(m_gradientViews);

    while(it.hasNext()) {
        const GradientWidgetInfo &info = it.next();

        if(info.isSelected())
            return info.m_gradientView->toLinearGradient();
    }

    return gr;
}

void GGradientManager::setGradientWidgetSelected(QWidget *w)
{
    QMutableListIterator<GradientWidgetInfo> it(m_gradientViews);

    while(it.hasNext()) {
        GradientWidgetInfo info = it.next();

        info.setSelected(w == info.m_gradientView);

        if(info.isSelected())
            emit newGradientSelected(info.m_gradientView->toLinearGradient());

        it.setValue(info);
    }

    updateToolButtonIcon();
}

void GGradientManager::updateToolButtonIcon()
{
    QListIterator<GradientWidgetInfo> it(m_gradientViews);

    while(it.hasNext()) {
        const GradientWidgetInfo &info = it.next();

        if(info.isSelected()) {
            GColorGradientView w(NULL);
            w.setMinimumSize(50, 50);
            w.setMaximumSize(50, 50);
            w.setArrowsVisible(false);
            w.fromLinearGradient(info.m_gradientView->toLinearGradient());

            QPixmap pixmap(w.rect().size());
            w.render(&pixmap, QPoint(), QRegion(w.rect()));

            ui->toolButton->setIcon(QIcon(pixmap));
        }
    }
}

bool GGradientManager::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress) {
        setGradientWidgetSelected((QWidget*)obj);
        ui->toolButton->menu()->close();
    }

    return QWidget::eventFilter(obj, event);
}


void GGradientManager::GradientWidgetInfo::setSelected(bool s)
{
    QString stylesheet;

    if(s) {
        stylesheet = QString("border-style: outset;"
                             "border-width: 2px;"
                             "border-color: rgb(255, 0, 0);");
    }

    m_parentWidget->setStyleSheet(stylesheet);
    m_selected = s;
}

bool GGradientManager::GradientWidgetInfo::isSelected() const
{
    return m_selected;
}
