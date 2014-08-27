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


#ifndef GITEMDRAWABLECONFIGURATIONMANAGERVIEW_H
#define GITEMDRAWABLECONFIGURATIONMANAGERVIEW_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QMutex>

#include "dm_documentview.h"
#include "tools/itemdrawable/dm_itemdrawableviewconfigurationbuilder.h"

namespace Ui {
    class GItemDrawableConfigurationManagerView;
}

class GItemDrawableConfigurationManagerView : public QWidget
{
    Q_OBJECT

public:
    explicit GItemDrawableConfigurationManagerView(QWidget *parent = 0);
    ~GItemDrawableConfigurationManagerView();

    DM_DocumentView* getDocumentView();

public slots:

    void setDocument(DM_DocumentView *doc);

signals:

    void mustAddItem(CT_ItemDrawableConfiguration *config);
    void mustRemoveItem(CT_ItemDrawableConfiguration *config);

private:
    Ui::GItemDrawableConfigurationManagerView   *ui;
    DM_DocumentView                             *_doc;
    DM_ItemDrawableViewConfigurationBuilder     *m_configBuilder;
    bool                                        m_init;
    QMutex                                      m_mutex;

    void initView();
    void setEmptyComboBoxText();
    void setTableHeader();
    void clearTableWidget();

private slots:

    void slotMustAddItem(DM_ItemDrawableViewConfiguration config);
    void slotMustRemoveItem(DM_ItemDrawableViewConfiguration config);

    void on_widgetComboBox_currentItemChanged(const QString &text);

    void on_tableWidgetConfiguration_itemChanged(QTableWidgetItem *item);
    void comboBoxIndexChanged(QString value);
};

#endif // GITEMDRAWABLECONFIGURATIONMANAGERVIEW_H
