/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#include "mytreedelegate.h"
#include "myqstandarditem.h"

#include <QApplication>
#include <QCheckBox>
#include <QPainter>

MyTreeDelegate::MyTreeDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    _imgDebugOn = new QImage(":/Icones/Icones/debug_on.png");
    _imgDebugOff = new QImage(":/Icones/Icones/debug_off.png");
}

MyTreeDelegate::~MyTreeDelegate()
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
    // colonne de debug
    else if(columnType == MyQStandardItem::StepDebug)
    {
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);

        opt.features ^= QStyleOptionViewItemV2::HasCheckIndicator;

        const QWidget *widget = opt.widget;
        QStyle *style = widget ? widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);

        int size = _imgDebugOn->height();

        if(option.rect.height() < size)
        {
            size = option.rect.height();
        }

        //QRect rect(option.rect.left() + (option.rect.width()/2) - (size/2), option.rect.top() + (option.rect.height()/2) - (size/2), size, size);
        QRect rect(option.rect.left(), option.rect.top() + (option.rect.height()/2) - (size/2), size, size);

        if(index.data(Qt::CheckStateRole).toBool())
        {
            painter->drawImage(rect, *_imgDebugOn);
        }
        else
        {
            painter->drawImage(rect, *_imgDebugOff);
        }

        return;
    }

    QStyledItemDelegate::paint(painter, option, index);
}
