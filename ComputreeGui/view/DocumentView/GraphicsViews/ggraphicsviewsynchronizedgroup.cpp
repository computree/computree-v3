/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
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

#include "ggraphicsviewsynchronizedgroup.h"
#include "ui_ggraphicsviewsynchronizedgroup.h"

#include "dm_graphicsviewsynchronizedgroup.h"
#include "dm_itemmodelviewsynchronizedgroup.h"

#include "gitemmodelviewsyncchoosedialog.h"

GGraphicsViewSynchronizedGroup::GGraphicsViewSynchronizedGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GGraphicsViewSynchronizedGroup)
{
    ui->setupUi(this);

    ui->treeWidgetDocument->setColumnCount(1);
    ui->treeWidgetDocument->header()->hide();
}

GGraphicsViewSynchronizedGroup::~GGraphicsViewSynchronizedGroup()
{
    delete ui;

    QMapIterator<QString, QList<DM_AbstractViewSynchronizedGroup*> > it(_dmSynchroGroupMap);

    while(it.hasNext())
    {
        it.next();
        const QList<DM_AbstractViewSynchronizedGroup*> &list = it.value();

        qDeleteAll(list.begin(), list.end());
    }
}

void GGraphicsViewSynchronizedGroup::addAllGraphicsFromDocumentView(const GDocumentViewForGraphics *view)
{
    if(view != NULL)
    {
        QTreeWidgetItem *pItem = getItemForType(view->getType());

        if(pItem == NULL)
        {
            pItem = createItemForType(ui->treeWidgetDocument->invisibleRootItem(), view->getType());
            ui->treeWidgetDocument->invisibleRootItem()->addChild(pItem);
        }

        QList<DM_AbstractViewSynchronizedGroup*> lGroups = _dmSynchroGroupMap.value(view->getType(), QList<DM_AbstractViewSynchronizedGroup*>() );

        DM_GraphicsViewSynchronizedGroup *syncGroup = NULL;

        if(lGroups.isEmpty())
        {
            syncGroup = new DM_GraphicsViewSynchronizedGroup();
            lGroups.append(syncGroup);
            _dmSynchroGroupMap.insert(view->getType(), lGroups);
        }
        else
        {
            syncGroup = (DM_GraphicsViewSynchronizedGroup*)lGroups.first();
        }

        QTreeWidgetItem *parentItem = createItemForDocument(pItem, *view);

        int i = 1;

        QListIterator<GGraphicsView*> it(view->getGraphicsList());

        while(it.hasNext())
        {
            GGraphicsView *gw = it.next();

            syncGroup->addGraphicsView(gw);

            parentItem->addChild(createItemForGraphicsView(parentItem, *gw, i));

            ++i;
        }

        pItem->addChild(parentItem);

        connect(view, SIGNAL(closed(DM_DocumentView*)), this, SLOT(documentViewClosed()), Qt::DirectConnection);
    }
}

void GGraphicsViewSynchronizedGroup::removeDocumentView(const GDocumentViewForGraphics *view)
{
    if(view != NULL)
    {
        QTreeWidgetItem *pItem = getItemForType(view->getType());

        if(pItem != NULL)
        {
            QString name = view->getSubWindow()->windowTitle();

            int n = pItem->childCount();
            bool continueLoop = true;
            int i = 0;

            while((i<n)
                  && continueLoop)
            {
                QTreeWidgetItem *item = pItem->child(i);

                if(item->text(0) == name)
                {
                    delete item;

                    continueLoop = false;
                }

                ++i;
            }
        }

        QList<DM_AbstractViewSynchronizedGroup*> lGroups = _dmSynchroGroupMap.value(view->getType(), QList<DM_AbstractViewSynchronizedGroup*>() );

        if(!lGroups.isEmpty())
        {
            DM_GraphicsViewSynchronizedGroup *syncGroup = (DM_GraphicsViewSynchronizedGroup*)lGroups.first();

            QListIterator<GGraphicsView*> it(view->getGraphicsList());

            while(it.hasNext())
            {
                GGraphicsView *gw = it.next();

                syncGroup->removeGraphicsView(gw);
            }
        }

        QMapIterator<QString, QList<DM_AbstractViewSynchronizedGroup*> > it(_dmSynchroGroupMap);

        while(it.hasNext())
        {
            it.next();

            QListIterator<DM_AbstractViewSynchronizedGroup*> itG(it.value());

            while(itG.hasNext())
            {
                DM_ItemModelViewSynchronizedGroup *gg = dynamic_cast<DM_ItemModelViewSynchronizedGroup*>(itG.next());

                if(gg != NULL)
                    gg->removeDocumentView(view);
            }
        }
    }
}

void GGraphicsViewSynchronizedGroup::syncItemModelWith(const GItemModelView *view)
{
    if(view != NULL)
    {
        GDocumentView *documentView = view->documentView();
        QString type = documentView->getType();

        QTreeWidgetItem *pItem = getItemForType(type);

        if(pItem == NULL)
        {
            pItem = createItemForType(ui->treeWidgetDocument->invisibleRootItem(), type);
            ui->treeWidgetDocument->invisibleRootItem()->addChild(pItem);
        }

        QList<DM_AbstractViewSynchronizedGroup*> lGroups = _dmSynchroGroupMap.value(type, QList<DM_AbstractViewSynchronizedGroup*>() );
        QListIterator<DM_AbstractViewSynchronizedGroup*> it(lGroups);

        DM_ItemModelViewSynchronizedGroup *syncGroup = NULL;

        while(it.hasNext()
              && (syncGroup == NULL))
        {
            DM_ItemModelViewSynchronizedGroup *g = (DM_ItemModelViewSynchronizedGroup*)it.next();

            if(g->itemModelView() == documentView)
                syncGroup = g;
        }

        if(syncGroup == NULL)
        {
            syncGroup = new DM_ItemModelViewSynchronizedGroup();
            syncGroup->setItemModelView(documentView);
            ((GItemModelView*)view)->setSyncGroup(syncGroup);

            lGroups.append(syncGroup);

            _dmSynchroGroupMap.insert(type, lGroups);
        }

        GItemModelViewSyncChooseDialog dialog;

        if(dialog.setSyncGroup(syncGroup))
        {
            dialog.adjustSize();

            if(dialog.exec())
            {
                int index = lGroups.indexOf(syncGroup);

                if(index < pItem->childCount())
                    delete pItem->child(index);


                QListIterator<DM_DocumentView*> it(syncGroup->documentViews());

                if(it.hasNext())
                {
                    QTreeWidgetItem *parentItem = createItemForDocument(pItem, *documentView);

                    while(it.hasNext())
                    {
                        GDocumentView *docView = (GDocumentView*)it.next();
                        connect(docView, SIGNAL(closed(DM_DocumentView*)), this, SLOT(documentViewClosed()), Qt::DirectConnection);

                        QTreeWidgetItem *newItem = createItemForDocument(parentItem, *docView);
                        parentItem->addChild(newItem);
                    }

                    pItem->addChild(parentItem);

                    connect(documentView, SIGNAL(closed(DM_DocumentView*)), this, SLOT(itemModelViewClosed(DM_DocumentView*)), Qt::DirectConnection);
                }

                ((GItemModelView*)view)->refreshAll();
            }
        }
    }
}

void GGraphicsViewSynchronizedGroup::documentViewClosed()
{
    removeDocumentView(dynamic_cast<GDocumentViewForGraphics*>(sender()));
}

void GGraphicsViewSynchronizedGroup::itemModelViewClosed(DM_DocumentView *view)
{
    QString type = view->getType();
    QTreeWidgetItem *pItem = getItemForType(type);

    if(pItem != NULL)
    {
        QList<DM_AbstractViewSynchronizedGroup*> lGroups = _dmSynchroGroupMap.value(type, QList<DM_AbstractViewSynchronizedGroup*>() );
        QMutableListIterator<DM_AbstractViewSynchronizedGroup*> it(lGroups);

        int index = 0;

        while(it.hasNext())
        {
            DM_ItemModelViewSynchronizedGroup *sGroup = (DM_ItemModelViewSynchronizedGroup*)it.next();

            if(sGroup->itemModelView() == view)
            {
                delete pItem->child(index);
                it.remove();

                delete sGroup;

                _dmSynchroGroupMap.insert(type, lGroups);

                return;
            }

            ++index;
        }
    }
}

QTreeWidgetItem* GGraphicsViewSynchronizedGroup::getItemForType(QString type) const
{
    QTreeWidgetItem *item = NULL;

    int n = ui->treeWidgetDocument->invisibleRootItem()->childCount();
    int i = 0;

    while((i<n)
          && (item == NULL))
    {
        item = ui->treeWidgetDocument->invisibleRootItem()->child(i);

        if(item->text(0) != type)
        {
            item = NULL;
        }

        ++i;
    }

    return item;
}

QTreeWidgetItem* GGraphicsViewSynchronizedGroup::createItemForType(QTreeWidgetItem *parent, QString type) const
{
    return new QTreeWidgetItem(parent, QStringList() << type);
}

QTreeWidgetItem* GGraphicsViewSynchronizedGroup::createItemForDocument(QTreeWidgetItem *parent, const GDocumentView &view) const
{
    return new QTreeWidgetItem(parent, QStringList() << view.getSubWindow()->windowTitle());
}

QTreeWidgetItem* GGraphicsViewSynchronizedGroup::createItemForGraphicsView(QTreeWidgetItem *parent, const GGraphicsView &gr, int index) const
{
    return new QTreeWidgetItem(parent, QStringList() << QString("Vue %1").arg(index));
}
