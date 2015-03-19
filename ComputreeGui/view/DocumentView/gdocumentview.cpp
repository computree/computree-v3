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


#include "gdocumentview.h"

#include "dm_guimanager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

GDocumentView::GDocumentView(GDocumentManagerView &manager, QString title) : DM_DocumentView(manager, title)
{
    _subWindow = new MyQMdiSubWindow(*this, &manager);
    _subWindow->setMinimumSize(QSize(400, 100));

    connect(_subWindow, SIGNAL(toBeClosed(QCloseEvent*)), this, SLOT(closeEvent(QCloseEvent*)), Qt::DirectConnection);

    // un widget qui contient....
    _centralWidget = new QWidget(_subWindow);

    QVBoxLayout *layoutCentralWidget = new QVBoxLayout();
    layoutCentralWidget->setContentsMargins(0, 0, 0, 0);
    layoutCentralWidget->setSpacing(3);

    delete getCentralWidget()->layout();

    getCentralWidget()->setLayout(layoutCentralWidget);

    _subWindow->setWidget(getCentralWidget());
    _subWindow->setWindowTitle(title);
}

GDocumentView::~GDocumentView()
{
}

void GDocumentView::init()
{
    // le widget qui contiendra les widgets créé par les CT_AbstractItemDrawable....
    createAndAddItemDrawableWidgetContainer(getCentralWidget());
}

QPoint GDocumentView::pos() const
{
    return getSubWindow()->pos();
}

QSize GDocumentView::size() const
{
    return getSubWindow()->size();
}

bool GDocumentView::isMaximized() const
{
    return getSubWindow()->isMaximized();
}

void GDocumentView::move(const QPoint &pos)
{
    getSubWindow()->move(pos);
}

void GDocumentView::resize(const QSize &size)
{
    getSubWindow()->resize(size);
}

void GDocumentView::setMaximized(bool val)
{
    if(val)
        getSubWindow()->showMaximized();
    else
        getSubWindow()->showNormal();
}

MyQMdiSubWindow* GDocumentView::getSubWindow() const
{
    return _subWindow;
}

QByteArray GDocumentView::saveGeometry() const
{
    return getSubWindow()->saveGeometry();
}

bool GDocumentView::restoreGeometry(const QByteArray &geometry)
{
    return getSubWindow()->restoreGeometry(geometry);
}

void GDocumentView::addActionOptions(ActionOptionsInterface *options)
{
    options->setParent(_subWindow);
    _layoutItemDrawableWidget->addWidget(options);
}

void GDocumentView::removeActionOptions(ActionOptionsInterface *options)
{
    delete options;
}

bool GDocumentView::isVisible() const
{
    return getSubWindow()->isVisible();
}

bool GDocumentView::canClose() const
{
    return ((closeFilter() == NULL) || closeFilter()->canClose(this));
}

///////////////// PROTECTED //////////////

QWidget* GDocumentView::getCentralWidget() const
{
    return _centralWidget;
}

void GDocumentView::createAndAddItemDrawableWidgetContainer(QWidget *parent)
{
    QWidget *itemDrawableWidgetContainer = new QWidget(parent);

    _layoutItemDrawableWidget = new QVBoxLayout();
    _layoutItemDrawableWidget->setContentsMargins(0, 0, 0, 0);

    delete itemDrawableWidgetContainer->layout();
    itemDrawableWidgetContainer->setLayout(_layoutItemDrawableWidget);

    parent->layout()->addWidget(itemDrawableWidgetContainer);
}

void GDocumentView::closeEvent(QCloseEvent *closeEvent)
{
    if(canClose())
    {
        removeAllItemDrawable();

        emit closed(this);
    }
    else
    {
        closeEvent->ignore();
    }
}
