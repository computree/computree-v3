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


#ifndef DM_GUIMANAGER_H
#define DM_GUIMANAGER_H

#include <QFutureWatcher>
#include <QQueue>

#include "dm_context.h"
#include "dm_mainwindow.h"
#include "dm_documentmanagerview.h"
#include "dm_multipleitemdrawablemodelmanager.h"
#include "dm_actionsmanager.h"
#include "dm_itemdrawableconfigurationmanagerview.h"

#include "cdm_stepmanager.h"
#include "cdm_internationalization.h"

#include "tools/guimanager/dm_guimanagertoken.h"
#include "tools/guimanager/dm_asyncoperation.h"
#include "tools/guimanager/dm_abstractasyncoperationoptions.h"

#define GUI_MANAGER DM_GuiManager::getUniqueInstance()
#define GUI_LOG DM_GuiManager::getUniqueInstance()->getPluginManager()->log()

class CT_AbstractExporter;

/**
  * Classe qui reprsente le gestionnaire de toutes les vues. La
  * vue principale doit hrit de DM_GuiManager et dans son constructeur
  * renseign l'attribut _uniqueInstance comme suit : _uniqueInstance = this;
  *
  * Ce qui veut dire qu'il ne peut y avoir qu'une intance de la vue principale !
  *
  * Le modle peut ainsi avoir accs aux vues grce  la variable GUI_MANAGER
  */
class DM_GuiManager : public QObject, public GuiManagerInterface
{
    Q_OBJECT
    Q_INTERFACES(GuiManagerInterface)

private:

    class ActionItemDrawable
    {
    public:
        ActionItemDrawable(DM_AsyncOperation *aop,
                           CT_AbstractResult *res,
                           QList<CT_AbstractItemDrawable*> *itemList,
                           QString filePath,
                           DM_DocumentManagerView *view,
                           DM_AsynchroneProgress *progress,
                           DM_AsynchroneProgress *secondProgress = NULL)
        {
            _aop = aop;
            _res = res;
            _itemModel = NULL;
            _exporter = NULL;

            if(itemList != NULL)
            {
                _itemList = *itemList;
            }

            _filePath = filePath;
            _view = view;
            _progress = progress;
            _secondProgress = secondProgress;
        }

        ActionItemDrawable(const ActionItemDrawable &action)
        {
            _aop = action._aop;
            _res = action._res;
            _itemModel = action._itemModel;
            _exporter = action._exporter;
            _itemList = action._itemList;
            _filePath = action._filePath;
            _view = action._view;
            m_docView = action.m_docView;
            _progress = action._progress;
            _secondProgress = action._secondProgress;
        }

        void setItemModel(CT_OutAbstractItemModel *model) { _itemModel = model; }
        void setDocumentView(DM_DocumentView *view) { m_docView = view; }
        void setIExporter(CT_AbstractExporter *exporter) { _exporter = exporter; }

        DM_AsyncOperation                   *_aop;
        CT_AbstractResult                   *_res;
        CT_OutAbstractItemModel             *_itemModel;
        DM_DocumentView                     *m_docView;
        CT_AbstractExporter                 *_exporter;
        QList<CT_AbstractItemDrawable*>     _itemList;
        QString                             _filePath;
        DM_DocumentManagerView              *_view;
        DM_AsynchroneProgress               *_progress;
        DM_AsynchroneProgress               *_secondProgress;
    };

    class ActionStep
    {
    public:
        ActionStep(DM_AsyncOperation *aop,
                   CT_VirtualAbstractStep *step,
                     CDM_StepManager *stepManager,
                     DM_DocumentManagerView *docManagerView,
                     DM_AsynchroneProgress *progress,
                     DM_AsynchroneProgress *secondProgress,
                     int totalProgressSize,
                     int nProgress)
        {
            _aop = aop;
            _step = step;
            _stepManager = stepManager;
            _docManagerView = docManagerView;
            _progress = progress;
            _secondProgress = secondProgress;

            _totalProgressSize = totalProgressSize;
            _nProgress = nProgress;
        }

        ActionStep(const ActionStep &action)
        {
            _aop = action._aop;
            _step = action._step;
            _stepManager = action._stepManager;
            _docManagerView = action._docManagerView;
            _progress = action._progress;
            _secondProgress = action._secondProgress;
            _totalProgressSize = action._totalProgressSize;
            _nProgress = action._nProgress;
        }

        DM_AsyncOperation       *_aop;
        CT_VirtualAbstractStep                    *_step;
        CDM_StepManager         *_stepManager;
        DM_DocumentManagerView  *_docManagerView;
        DM_AsynchroneProgress   *_progress;
        DM_AsynchroneProgress   *_secondProgress;

        int                     _totalProgressSize;
        int                     _nProgress;
    };

public:

    static DM_GuiManager* getUniqueInstance()
    {
        return _uniqueInstance;
    }

    virtual ~DM_GuiManager();

    /**
     * @brief return true if another async operation is in progress
     */
    bool isAsyncOperationInProgress() const;

    /**
     * @brief Request a exclusive async operation. If you want to do a asynchronous treatment and do
     *        not want that another elements in the gui do another treatment in same time you must call this method. We create a
     *        token, append them in a list and blocks the workflow (without block the gui) until they are other token before them and until the
     *        async options don't pass (method 'passed'). When it is the turn for this token and when the async options passed the workflows continue.
     *
     * @param options : to specify otions (example : if you want that no step is running. example 2 : if you want that the StepManager don't clear automatically resuls, etc...).
     * @param wait : true if you want to wait until the current operation is finished. If true the workflow is blocked (not the gui thread)
     * @return A object that you must delete when yo have finished your operation to free the access to other elements in the gui. If the
     *         variable 'wait' is false and a current operation is in progress the method return NULL.
     */
    DM_AsyncOperation* requestExclusiveAsyncOperation(const DM_AbstractAsyncOperationOptions *options = NULL, bool wait = true);

    /**
      * \brief Gre l'ajout des CT_AbstractItemDrawable d'un rsultat aux diffrentes
      *  vues qui en ont besoin. (Asynchrone)
      *
      * \param res : le rsultat
      * \param context : le context a renvoyer lorsque l'action est termin (voir signal correspondant)
      * \return false si une demande a dj t formul et n'est pas termine
      */
    virtual bool asyncAddAllItemDrawableOfResultOnView(CT_AbstractResult &res, DM_Context *context);

    /**
      * \brief Gere l'ajout des CT_AbstractItemDrawable à la vue passé en paramètre. (Asynchrone)
      *
      * \param result : le résultat contenant le modèle. On ne peut pas le récupérer à partir du modèle car si c'est une copie
      *                 il contient le résultat du modèle original !
      * \param model : le modele d'CT_AbstractItemDrawable a rechercher et ajouter
      * \param view : la vue à laquelle il faut ajouter les items
      * \param context : le context a renvoyer lorsque l'action est termine (voir signal correspondant)
      * \return false si une demande a deja ete formule et n'est pas terminee
      */
    virtual bool asyncAddAllItemDrawableOfModelOnView(CT_AbstractResult &res, CT_OutAbstractItemModel &model, DM_DocumentView &view, DM_Context *context);

    /**
      * \brief Gre l'ajout de la liste des ItemDrawables pass en paramtres aux
      *  vues qui en ont besoin. (Asynchrone)
      *
      * \param itemList : la liste des itemsDrawable
      * \param view : la vue à laquelle il faut ajouter les items. NULL si vous voulez ajouter au document actif
      * \param context : le context a renvoyer lorsque l'action est termin (voir signal correspondant)
      * \return false si une demande a dj t formul et n'est pas termine
      */
    virtual bool asyncAddAllItemDrawableOfListOnView(QList<CT_AbstractItemDrawable*> &itemList, DM_DocumentView *view, DM_Context *context);

    /**
      * \brief Gre la suppression des CT_AbstractItemDrawable d'un rsultat des diffrentes
      *  vues. (Asynchrone)
      *
      * \param res : le rsultat
      * \param context : le context a renvoyer lorsque l'action est termin (voir signal correspondant)
      * \return false si une demande a dj t formul et n'est pas termine
      */
    virtual bool asyncRemoveAllItemDrawableOfResultFromView(CT_AbstractResult &res, DM_Context *context);

    /**
      * \brief Gere la suppression des CT_AbstractItemDrawable, en fonction d'un modele, des differentes
      *  vues. (Asynchrone)
      *
      * \param model : le modele d'ItemDrawablea rechercher et enlever
      * \param context : le context a renvoyer lorsque l'action est termin (voir signal correspondant)
      * \return false si une demande a dj t formul et n'est pas termine
      */
    virtual bool asyncRemoveAllItemDrawableOfModelFromAllViews(CT_OutAbstractItemModel &model, DM_Context *context);

    /**
      * \brief Gere la suppression des CT_AbstractItemDrawable, en fonction d'un modele, de la vue passé en paramètre. (Asynchrone)
      *
      * \param model : le modele d'CT_AbstractItemDrawable a rechercher et enlever
      * \param view : la vue
      * \param context : le context a renvoyer lorsque l'action est termin (voir signal correspondant)
      * \return false si une demande a dj t formul et n'est pas termine
      */
    virtual bool asyncRemoveAllItemDrawableOfModelFromView(CT_OutAbstractItemModel &model, DM_DocumentView &view, DM_Context *context);

    /**
      * \brief Gre la suppression de la liste des ItemDrawables pass en paramtres des diffrentes
      *  vues. (Asynchrone)
      *
      * \param itemList : la liste des itemsDrawable
      * \param context : le context a renvoyer lorsque l'action est termin (voir signal correspondant)
      * \return false si une demande a dj t formul et n'est pas termine
      */
    virtual bool asyncRemoveAllItemDrawableOfListOnView(QList<CT_AbstractItemDrawable*> &itemList, DM_Context *context);

    /**
      * \brief Gre la suppression d'une tape (suppression des rsultats de la mmoire,
      *        des CT_AbstractItemDrawable des documents, etc...)
      * \param step : l'tape
      * \param context : le context a renvoyer lorsque l'action est termin (voir signal correspondant)
      * \return : false si une demande a dj t formul et n'est pas termine
      */
    virtual bool asyncRemoveStep(CT_VirtualAbstractStep &step, DM_Context *context);

    /**
      * \brief Gre la suppression de toutes les tapes de l'arbre
      * \param context : le context a renvoyer lorsque l'action est termin (voir signal correspondant)
      * \return : 0 si l'action n'est pas termine, 1 si l'action a commence, -1 si l'action n'a pas besoin d'tre effectue
      */
    virtual int asyncRemoveAllStep(DM_Context *context);

    /**
      * \brief Gre le chargement des rsultats des tapes
      * \param step : l'tape  partir de laquelle charger les rsultats (celle-ci ne doit pas avoir d'tape parente !)
      * \param context : le context a renvoyer lorsque l'action est termin (voir signal correspondant)
      * \return : false si une demande a dj t formul et n'est pas termine
      */
    virtual bool asyncLoadResultStep(CT_AbstractStepSerializable &step, DM_Context *context);

    /**
      * \brief Gere l'exportation. Appel les methodes de l'exportateur : configureExport(...) puis avec un thread export(...)
      *
      *        ATTNENTION : veuillez spécifier à l'exportateur le chemin du fichier avant d'appeler cette méthode
      *
      * \param exporter : l'exportateur a utiliser pour l'export
      * \param context : le context a renvoyer lorsque l'action est termine (voir signal correspondant)
      * \return : false si une demande a dj t formul et n'est pas termine
      */
    virtual bool asyncExport(CT_AbstractExporter *exporter, DM_Context *context);

    /**
      * \brief Modifie la couleur de tous les CT_AbstractItemDrawable de tous les documents qui ont comme modèle celui
      *        passé en paramètre
      */
    bool setColorOnAllItemDrawableOnAllDocumentsThatModelCorresponding(const CT_OutAbstractItemModel *model, const QColor &color);

    /**
      * \brief Modifie la couleur de tous les CT_AbstractItemDrawable de tous les documents qui ont comme modèle celui
      *        passé en paramètre parmis une liste de couleur
      */
    bool setColorListOnAllItemDrawableOnAllDocumentsThatModelCorresponding(const CT_OutAbstractItemModel *model, const QList<QColor> &colorList);

    /**
      * \brief Gere l'ajout des itemdrawable modele du resultat pass en paramtre a la vue permettant d'interagir avec eux
      */
    bool editItemDrawableModelOfResult(CT_AbstractResult &res);

    /**
      * \brief Supprime le résultat de la vue permettant d'intéragir avec les modèles de ses CT_AbstractItemDrawable
      */
    bool removeEditItemDrawableModelOfResult(CT_AbstractResult &res);

    /**
      * \brief Supprime tous les CT_AbstractItemDrawable de tous les documents
      */
    bool cleanItemDrawableOfAllDocuments();

    bool isRunning();

    void redrawAllDocuments();

    CDM_PluginManager* getPluginManager() const;

    /**
      * \brief Retourne la vue qui gre les documents (gestionnaire de document)
      */
    virtual DM_DocumentManagerView* getDocumentManagerView() const = 0;

    /**
      * \brief Retourne le gestionnaire d'etape
      */
    virtual CDM_StepManager* getStepManager() const = 0;

    /**
     * @brief Return the actions manager
     */
    virtual DM_ActionsManager* getActionsManager() const = 0;

    /**
     * @brief Returns the language manager
     */
    virtual CDM_Internationalization* getLanguageManager() const = 0;

    /**
     * @brief Returns the manager of itemdrawable's configuration
     */
    virtual DM_ItemDrawableConfigurationManagerView* getItemDrawableConfigurationManagerView() const = 0;

protected:

    static DM_GuiManager*   _uniqueInstance;

    DM_AsyncOperation           *m_currentAsyncOperation;
    QMutex                      m_asyncOperationTokenMutex;
    QList<DM_GuiManagerToken*>  m_tokens;

    QFutureWatcher<void>        _futureWatcher;
    QFuture<void>               _future;
    GMainProgressDialog         *_progressDialog;
    DM_AsynchroneProgress       _asyncProgress;
    DM_AsynchroneProgress       _asyncSecondProgress;

    QList<DM_Context*>          _contextList;

    QQueue<IMainProgressDialog*>  m_queueProgressDialogToDestroy;

    /**
      * \brief Initialisation (connexion des signaux, etc...)
      */
    void init();

    /**
      * \brief Retourne la fentre principale
      */
    virtual DM_MainWindow* getMainWindow() const = 0;

    /**
      * \brief Retourne le gestionnaire de modele d'itemDrawable
      */
    virtual DM_MultipleItemDrawableModelManager* getItemDrawableModelManager() const = 0;

private:

    /**
      * \brief Initialise la fentre de progression avec le texte pass en
      *        paramtre et l'affiche.
      */
    void initProgressDialog(IMainProgressDialog *dialog, QString text, QString secondText = "");

    void addNewContext(DM_Context *context);

    void recursiveDeleteStepConfigurationDialog(CT_VirtualAbstractStep &step);

    static void staticRemoveResultFromOtherView(ActionItemDrawable info);

    static void staticAddAllItemDrawableOfResultOnView(ActionItemDrawable info);
    static void staticAddAllItemDrawableOfModelOnView(ActionItemDrawable info);
    static void staticAddAllItemDrawableOfListOnView(ActionItemDrawable info);
    static void staticRemoveAllItemDrawableOfResultFromView(ActionItemDrawable info);
    static void staticRemoveAllItemDrawablOfModelFromAllViews(ActionItemDrawable info);
    static void staticRemoveAllItemDrawableOfModelFromView(ActionItemDrawable info);
    static void staticRemoveAllItemDrawableOfListFromView(ActionItemDrawable info);

    static void staticClearResultMemoryAndRemoveStep(ActionStep info);
    static int staticRecursiveCountProgress(CT_VirtualAbstractStep *step);
    static int staticRecursiveClearResultMemoryAndRemoveStep(ActionStep info);
    static void staticExport(ActionItemDrawable info);

private slots:

    /**
      * \brief Slot connect au gestionnaire d'tape qui envoie un signal
      *        lorsqu'on dsaloue les CT_AbstractItemDrawable d'un rsultat de la mmoire.
      */
    void resultToBeClearedFromMemory(const CT_AbstractResult *res);

    /**
      * \brief Slot connect au gestionnaire d'tape qui envoie un signal
      *        lorsqu'une tape va supprimer l'un de ses rsultats.
      */
    void resultToBeRemoved(const CT_AbstractResult *res);

    void stepManagerCompletedLoadResultStep();

    /**
     * @brief Called when the current async operation was destroyed
     */
    void slotCurrentAsyncOperationDestroyed();

    /**
     * @brief Called when the current async operation was destroyed
     */
    void slotCurrentAsyncOperationReleased();

signals:
    void currentAsyncOperationDestroyed();
    void currentAsyncOperationReleased();
};

#endif // DM_GUIMANAGER_H
