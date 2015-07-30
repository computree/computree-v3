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
#include "dm_itemdrawableconfigurationmanagerview.h"
#include "tools/itemdrawable/dm_itemdrawableviewconfigurationbuilder.h"

namespace Ui {
    class GItemDrawableConfigurationManagerView;
}

/**
 * @brief View that show all configurations present in the document ("setDocument" method) and allows to change their values
 */
class GItemDrawableConfigurationManagerView : public QWidget, public DM_ItemDrawableConfigurationManagerView
{
    Q_OBJECT

public:
    explicit GItemDrawableConfigurationManagerView(QWidget *parent = 0);
    ~GItemDrawableConfigurationManagerView();

    /**
     * @brief Returns the document used by this manager
     */
    DM_DocumentView* getDocumentView() const;

    /**
     * @brief Return the collection of itemdrawable that was impacted by the configuration passed in parameter
     */
    QList<CT_AbstractItemDrawable*> itemDrawablesForConfiguration(CT_ItemDrawableConfiguration *config) const;

public slots:

    /**
     * @brief Set the document to use. All configuration of all itemdrawable present in the document will be show
     */
    void setDocument(DM_DocumentView *doc);

private:
    Ui::GItemDrawableConfigurationManagerView   *ui;
    DM_DocumentView                             *_doc;
    DM_ItemDrawableViewConfigurationBuilder     *m_configBuilder;
    bool                                        m_init;
    QMutex                                      m_mutex;

    /**
     * @brief Called to initialize the combobox and the table widget when a new document is set
     */
    void initView();

    /**
     * @brief Called to initialize table's header
     */
    void setTableHeader();

    /**
     * @brief Called to clear the table widget
     */
    void clearTableWidget();

private slots:

    /**
     * @brief Called when we must add a new configuration to the view
     */
    void slotMustAddItem(DM_ItemDrawableViewConfiguration config);

    /**
     * @brief Called when we must remove the configuration from the view
     */
    void slotMustRemoveItem(DM_ItemDrawableViewConfiguration config);

    /**
     * @brief Called when the current item of the combobox that contains configuration name changed (user has changed the configuration to show)
     */
    void on_widgetComboBox_currentItemChanged(const QString &text);

    /**
     * @brief Called when a value changed in the table (user has changed a value)
     */
    void on_tableWidgetConfiguration_itemChanged(QTableWidgetItem *item);

    /**
     * @brief Called when the current index changed in a combox of the table (user has changed a value)
     */
    void tableWidgetComboBoxIndexChanged(QString value);

    /**
     * @brief Called to set to the log that the type was not implemented
     */
    void logDebugTypeNotImplemented(CT_ItemDrawableConfiguration::Type type);

signals:
    /**
     * @brief Emitted when a value changed from a configuration
     */
    void valueOfConfigurationChanged(CT_ItemDrawableConfiguration *config, QVariant lastValue, QVariant newValue, CT_ItemDrawableConfiguration::Type typeOfValue);
};

#endif // GITEMDRAWABLECONFIGURATIONMANAGERVIEW_H
