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

namespace Ui {
    class GMainWindow;
}

class GMainWindow : public QMainWindow, public DM_MainWindow
{
    Q_OBJECT

public:
    explicit GMainWindow(QWidget *parent = 0);
    ~GMainWindow();

protected:

    void closeEvent(QCloseEvent *ev);

public:

    DocumentManagerInterface* documentManager() const;

    DM_DocumentManagerView* getDocumentManagerView() const;
    DM_MultipleItemDrawableModelManager* getItemDrawableModelManager() const;

    GMainProgressDialog* createWaitProgressDialog();

    void loadConfiguration();
    void writeConfiguration();

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
    void configureCurrentCoordinateSystem();
    void cleanItemDrawableOfAllDocuments();
    void showINeedHelpDialog();

    void arrangeDocksInTabs();
    void arrangeDocksInColumnWithLogAtBottom();
    void arrangeDocksInColumn();

private:
    Ui::GMainWindow                         *ui;
    GDocumentManagerView                    *_docManagerView;
    GStepManager                            *_stepManagerView;
    GMultipleItemDrawableModelManager       *_itemDrawableModelView;
    GItemDrawableConfigurationManagerView   *_itemDrawableConfigurationView;
    GGraphicsViewSynchronizedGroup          *_graphicsViewSyncGroupView;

    QString                                 _defaultOpenDirPath;
    QString                                 _defaultSaveDirPath;

    QAction                                 *actionSaveScript;
    QAction                                 *actionAProposPlugin;
    QMenu                                   *menuNewStepCanBeAddedFirst;

    QLabel                                  *_permanentLabelForMessage;

    void initUI();
    void loadPlugins(bool showMessageIfNoPluginsFounded = true);

    QString createFileExtensionAvailable();
    QString createScriptManagerExtension(QString fileExtension);

private slots:

    /**
     * @brief Called when a QAction of a language is triggered
     */
    void changeLanguage();

    void documentToBeClosed(DM_DocumentView *view);

    void stepAdded(CT_VirtualAbstractStep *step);
    void stepToBeRemoved(CT_VirtualAbstractStep *step);
    void menuNewStepCanBeAddedFirstAboutToShow();

    void addCanBeAddedFirstStepFromMyQAction();
};

#endif // GMAINWINDOW_H
