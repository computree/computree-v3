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


#ifndef GMAINWINDOW_H
#define GMAINWINDOW_H

#include <QMainWindow>

#include "dm_mainwindow.h"
#include "dm_context.h"

#include "gdocumentmanagerview.h"
#include "gstepmanager.h"
#include "gmultipleitemdrawablemodelmanager.h"
#include "gitemdrawableconfigurationmanagerview.h"
#include "GraphicsViews/ggraphicsviewsynchronizedgroup.h"
#include "view/ItemDrawableConfigurationView/dm_itemdrawableconfigurationandgraphicsviewsynchronizer.h"
#include "view/MainView/gstepchooserdialog.h"

namespace Ui {
    class GMainWindow;
}

class GStepViewDefault;

class GMainWindow : public QMainWindow, public DM_MainWindow, public CDM_ScriptManagerAbstract::IScriptLoadCallBack
{
    Q_OBJECT

public:
    explicit GMainWindow(QWidget *parent = 0);
    ~GMainWindow();

    void init();

protected:

    void closeEvent(QCloseEvent *ev);

public:

    DocumentManagerInterface* documentManager() const;

    DM_DocumentManagerView* getDocumentManagerView() const;
    DM_MultipleItemDrawableModelManager* getItemDrawableModelManager() const;
    GItemDrawableConfigurationManagerView* getItemDrawableConfigurationManager() const;

    GMainProgressDialog* createWaitProgressDialog();

    void loadConfiguration();
    void writeConfiguration();

    /**
     * @brief Inherited from CDM_ScriptManagerAbstract::IScriptLoadCallBack
     */
    virtual void loadScriptError(CDM_ScriptProblem &problem);

public slots:

    void newDocument();
    void new2DDocument();
    void newItemModelDocument();
    void openFile();
    void openFile(QString &loadDirectory);
    void saveScript();
    void saveScript(QString &saveDirectory);
    void showAboutDialog();
    void showAboutPluginsDialog();
    void cleanItemDrawableOfAllDocuments();
    void showINeedHelpDialog();

    void arrangeDocksInTabs();
    void arrangeDocksInColumnWithLogAtBottom();
    void arrangeDocksInColumn();
    void actionHovered();

    void showStepChooser();
    void hideStepChooser();

private:
    Ui::GMainWindow                         *ui;
    GDocumentManagerView                    *_docManagerView;
    GStepManager                            *_stepManagerView;
    GMultipleItemDrawableModelManager       *_itemDrawableModelView;
    GItemDrawableConfigurationManagerView   *_itemDrawableConfigurationView;
    DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer m_itemDrawableConfigurationSyncWithGraphicsViewManager;
    GGraphicsViewSynchronizedGroup          *_graphicsViewSyncGroupView;

    QString                                 _defaultOpenDirPath;
    QString                                 _defaultSaveDirPath;

    QAction                                 *actionSaveScript;
    QAction                                 *actionAProposPlugin;

    QLabel                                  *_permanentLabelForMessage;
    GStepChooserDialog                      *m_stepChooserDialog;

    bool                                    m_inLoadConfiguration;

    void initUI();
    void loadPlugins(bool showMessageIfNoPluginsFounded = true);
    void showMessageIfScriptBackupIsAvailable();

    QString createFileExtensionAvailable() const;
    void getFileExtensionAvailableInStepsOfLevelRecursively(CT_MenuLevel *level, QHash<QString, QStringList> &hash) const;
    QString createScriptManagerExtension(QString preString, QString postString) const;

    void computeStepChooserDialogDefaults(QPoint &defaultPos, QSize &defaultSize);
private slots:

    /**
     * @brief Called when a QAction of a language is triggered
     */
    void changeLanguage();

    void documentToBeClosed(DM_DocumentView *view);

    void stepAdded(CT_VirtualAbstractStep *step);
    void stepToBeRemoved(CT_VirtualAbstractStep *step);
    void replaceStepChooserDialogToDefaults();
};

#endif // GMAINWINDOW_H
