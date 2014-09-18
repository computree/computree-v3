#include "mytreedelegate.h"
#include "myqstandarditem.h"

#include <QApplication>
#include <QCheckBox>
#include <QPainter>

MyTreeDelegate::MyTreeDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void MyTreeDelegate::paint(QPainter *painter,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    int columnType = index.data(MyQStandardItem::ColumnTypeRole).toInt();

    // colonne de progression
    if((columnType == MyQStandardItem::StepProgress)
        || (columnType == MyQStandardItem::ResultProgress))
    {
        // Cration d'un QStyleOptionProgressBar qui permettra de dessiner notre barre de progression :
        QStyleOptionProgressBar progressBar;

        progressBar.state = QStyle::State_Enabled;
        progressBar.direction = QApplication::layoutDirection();
        progressBar.fontMetrics = QApplication::fontMetrics();
        progressBar.rect = option.rect;
        progressBar.maximum = 100;
        progressBar.minimum = 0;

        // Gestion de la progression :
        progressBar.progress = index.data().toInt();

        // Gestion du texte :
        progressBar.text = QString::number(progressBar.progress) + "%";
        progressBar.textAlignment = Qt::AlignCenter;
        progressBar.textVisible = true;

        // Affichage de la barre de progression  l'aide du style de l'application :
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBar, painter);
        return;
    }
    // colonne de temps
    else if(columnType == MyQStandardItem::StepElapsedTime)
    {
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);

        int elaps = (int)index.data().toDouble();
        int hour = elaps/3600000;
        elaps -= hour*3600000;
        int min = elaps/60000;
        elaps -= min*60000;
        int sec = elaps/1000;
        elaps -= sec*1000;
        int ms = elaps;

        opt.text = QString("%1h:%2m:%3s:%4ms").arg(hour).arg(min).arg(sec).arg(ms);

        const QWidget *widget = opt.widget;
        QStyle *style = widget ? widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

        return;
    }

    QStyledItemDelegate::paint(painter, option, index);
}
