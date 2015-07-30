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


#include "gitemdrawableconfigurationmanagerview.h"
#include "ui_gitemdrawableconfigurationmanagerview.h"

#include "dm_documentmanager.h"
#include "dm_guimanager.h"

#include <QLineEdit>
#include <QDebug>

GItemDrawableConfigurationManagerView::GItemDrawableConfigurationManagerView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GItemDrawableConfigurationManagerView)
{
    ui->setupUi(this);

    ui->widgetComboBox->setEmptyText(tr("........... Aucun element configurable dans la vue 3D ..........."));
    m_configBuilder = new DM_ItemDrawableViewConfigurationBuilder(true);

    _doc = NULL;

    initView();
}

GItemDrawableConfigurationManagerView::~GItemDrawableConfigurationManagerView()
{
    delete ui;
    delete m_configBuilder;
}

DM_DocumentView* GItemDrawableConfigurationManagerView::getDocumentView() const
{
    return _doc;
}

QList<CT_AbstractItemDrawable *> GItemDrawableConfigurationManagerView::itemDrawablesForConfiguration(CT_ItemDrawableConfiguration *config) const
{
    return m_configBuilder->itemDrawablesForConfiguration(config);
}

void GItemDrawableConfigurationManagerView::setDocument(DM_DocumentView *doc)
{
    if(doc != _doc)
    {
        if(_doc != NULL)
        {
            disconnect(_doc, NULL, m_configBuilder, NULL);
            disconnect(m_configBuilder, NULL, this, NULL);

            m_configBuilder->clear();
        }

        _doc = doc;

        initView();

        if(_doc != NULL)
        {
            connect(_doc, SIGNAL(itemDrawableAdded(CT_AbstractItemDrawable&)), m_configBuilder, SLOT(addItemDrawable(CT_AbstractItemDrawable&)), Qt::DirectConnection);
            connect(_doc, SIGNAL(itemDrawableToBeRemoved(CT_AbstractItemDrawable&)), m_configBuilder, SLOT(removeItemDrawable(CT_AbstractItemDrawable&)), Qt::DirectConnection);

            qRegisterMetaType<DM_ItemDrawableViewConfiguration>("DM_ItemDrawableViewConfiguration");
            connect(m_configBuilder, SIGNAL(added(DM_ItemDrawableViewConfiguration)), this, SLOT(slotMustAddItem(DM_ItemDrawableViewConfiguration)), Qt::QueuedConnection);
            connect(m_configBuilder, SIGNAL(removed(DM_ItemDrawableViewConfiguration)), this, SLOT(slotMustRemoveItem(DM_ItemDrawableViewConfiguration)), Qt::QueuedConnection);
        }
    }
}

void GItemDrawableConfigurationManagerView::initView()
{
    QMutexLocker locker(&m_mutex);

    m_init = true;

    ui->widgetComboBox->clear();
    clearTableWidget();

    if(_doc != NULL)
    {
        setTableHeader();

        QListIterator<CT_AbstractItemDrawable*> it(_doc->getItemDrawable());

        while(it.hasNext())
            m_configBuilder->addItemDrawable(*it.next());
    }

    QList<DM_ItemDrawableViewConfiguration> configs = m_configBuilder->configurations();
    QListIterator<DM_ItemDrawableViewConfiguration> it(configs);

    if(it.hasNext())
    {
        m_init = false;
        ui->widgetComboBox->addItem(it.next().configuration()->name());
        m_init = true;
    }

    while(it.hasNext())
        ui->widgetComboBox->addItem(it.next().configuration()->name());

    m_init = false;
}

void GItemDrawableConfigurationManagerView::setTableHeader()
{
    QStringList header;

    header << tr("Nom") << tr("Valeur");

    ui->tableWidgetConfiguration->setColumnCount(header.size());
    ui->tableWidgetConfiguration->setHorizontalHeaderLabels(header);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    ui->tableWidgetConfiguration->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#else
    ui->tableWidgetConfiguration->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif
}


void GItemDrawableConfigurationManagerView::clearTableWidget()
{
    ui->tableWidgetConfiguration->clear();
    ui->tableWidgetConfiguration->setRowCount(0);
    ui->tableWidgetConfiguration->setColumnCount(0);
}

void GItemDrawableConfigurationManagerView::slotMustAddItem(DM_ItemDrawableViewConfiguration config)
{
    ui->widgetComboBox->addItem(config.configuration()->name());
}

void GItemDrawableConfigurationManagerView::slotMustRemoveItem(DM_ItemDrawableViewConfiguration config)
{
    ui->widgetComboBox->removeItem(config.configuration()->name());
}

void GItemDrawableConfigurationManagerView::on_widgetComboBox_currentItemChanged(const QString &text)
{
    if(!m_init)
    {
        CT_ItemDrawableConfiguration *config = m_configBuilder->getConfigurationWithName(text).configuration();

        clearTableWidget();

        if(config != NULL)
        {
            setTableHeader();

            int size = config->size();

            ui->tableWidgetConfiguration->setRowCount(size);

            for(int i=0; i<size; ++i)
            {
                QString name = config->getVariableName(i);
                QVariant value = config->getVariableValue(i);
                CT_ItemDrawableConfiguration::Type type = config->getVariableType(i);

                QTableWidgetItem *item = new QTableWidgetItem();
                item->setFlags(Qt::ItemIsEnabled);
                item->setText(name);

                ui->tableWidgetConfiguration->setItem(i, 0, item);

                if(type == CT_ItemDrawableConfiguration::Bool)
                {
                    item = new QTableWidgetItem();
                    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
                    item->setText(tr("Afficher"));

                    if(value.toBool())
                        item->setCheckState(Qt::Checked);
                    else
                        item->setCheckState(Qt::Unchecked);

                    ui->tableWidgetConfiguration->setItem(i, 1, item);
                }
                else if(type == CT_ItemDrawableConfiguration::StringList)
                {
                    QComboBox *cb = new QComboBox(ui->tableWidgetConfiguration);

                    QStringList listV = value.toStringList();

                    while(!listV.isEmpty())
                        cb->addItem(listV.takeFirst());

                    ui->tableWidgetConfiguration->setCellWidget(i, 1, cb);

                    connect(cb, SIGNAL(currentIndexChanged(QString)), this, SLOT(tableWidgetComboBoxIndexChanged(QString)));
                }
                else
                {
                    item = new QTableWidgetItem();
                    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
                    item->setText(value.toString());

                    ui->tableWidgetConfiguration->setItem(i, 1, item);
                }
            }
        }
    }
}

void GItemDrawableConfigurationManagerView::on_tableWidgetConfiguration_itemChanged(QTableWidgetItem *item)
{
    bool firstColumn = false;
    int r = 0;
    int rowCount = ui->tableWidgetConfiguration->rowCount();
    while ((r < rowCount) && !firstColumn)
    {
        if (ui->tableWidgetConfiguration->item(r,1) == item)
        {
            firstColumn = true;
        }
        r++;
    }

    if((_doc != NULL) && firstColumn)
    {
        CT_ItemDrawableConfiguration *config = m_configBuilder->getConfigurationWithName(ui->widgetComboBox->currentItemText()).configuration();

        if(config != NULL)
        {
            int i = item->row();

            QVariant value = config->getVariableValue(i);
            CT_ItemDrawableConfiguration::Type type = config->getVariableType(i);

            if(type == CT_ItemDrawableConfiguration::Bool)
            {
                bool enable = (item->checkState() == Qt::Checked);

                if(enable != value.toBool())
                {
                    config->setVariableValue(i, enable);

                    emit valueOfConfigurationChanged(config, QVariant(!enable), QVariant(enable), type);

                    _doc->getManager()->redrawAllDocument();
                }
            }
            else if (type == CT_ItemDrawableConfiguration::Double)
            {
                bool succeeded = false;
                double newValue = item->text().toDouble(&succeeded);
                if (succeeded && newValue != value.toDouble())
                {
                    config->setVariableValue(i, newValue);

                    emit valueOfConfigurationChanged(config, value, QVariant(newValue), type);

                    _doc->getManager()->redrawAllDocument();
                }
            }
            else if (type == CT_ItemDrawableConfiguration::Int)
            {
                bool succeeded = false;
                int newValue = item->text().toInt(&succeeded);
                if (succeeded && newValue != value.toInt())
                {
                    config->setVariableValue(i, newValue);

                    emit valueOfConfigurationChanged(config, value, QVariant(newValue), type);

                    _doc->getManager()->redrawAllDocument();
                }
            }
            else
            {
                logDebugTypeNotImplemented(type);
            }
        }
    }
}

void GItemDrawableConfigurationManagerView::tableWidgetComboBoxIndexChanged(QString value)
{
    QComboBox *cb = dynamic_cast<QComboBox*>(sender());

    bool firstColumn = false;
    int row = 0;
    int rowCount = ui->tableWidgetConfiguration->rowCount();
    while ((row < rowCount) && !firstColumn)
    {
        if (ui->tableWidgetConfiguration->cellWidget(row,1) == cb)
        {
            firstColumn = true;
        }
        else
            ++row;
    }

    if((_doc != NULL) && firstColumn)
    {
        CT_ItemDrawableConfiguration *config = m_configBuilder->getConfigurationWithName(ui->widgetComboBox->currentItemText()).configuration();

        if(config != NULL)
        {
            int i = row;

            QVariant varValue = config->getVariableValue(i);
            CT_ItemDrawableConfiguration::Type type = config->getVariableType(i);

            if(type == CT_ItemDrawableConfiguration::StringList)
            {
                QStringList sList = varValue.toStringList();

                int idV = sList.indexOf(value);

                if(idV >= 0)
                {
                    sList.removeAt(idV);
                    sList.insert(0, value);

                    config->setVariableValue(i, QVariant(sList));

                    emit valueOfConfigurationChanged(config, varValue, QVariant(sList), type);

                    _doc->getManager()->redrawAllDocument();
                }
            }
            else
            {
                logDebugTypeNotImplemented(type);
            }
        }
    }
}

void GItemDrawableConfigurationManagerView::logDebugTypeNotImplemented(CT_ItemDrawableConfiguration::Type type)
{
    GUI_LOG->addDebugMessage(LogInterface::gui, tr("[GItemDrawableConfigurationManagerView] Modification du type \"%1\" non implementé.").arg(type));
}
