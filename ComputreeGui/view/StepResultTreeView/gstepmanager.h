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


#ifndef GSTEPMANAGER_H
#define GSTEPMANAGER_H

#include "cdm_stepmanager.h"

#include "gtreestepcontextmenu.h"
#include "myqstandarditem.h"
#include "mytreedelegate.h"

#include <QTreeView>
#include <QStandardItemModel>

/**
  * Classe qui permet de grer la vue d'un arbre d'tapes et de leurs rsultats.
  */
class GStepManager : public QWidget
{
    Q_OBJECT

private:

    class ResultInfo{

    public :

        ResultInfo(bool clearedFromMemory,
                   bool serialized)
        {
            _isClearedFromMemory = clearedFromMemory;
            _isSerialized = serialized;
        }

        bool    _isClearedFromMemory;
        bool    _isSerialized;
    };

public:
    explicit GStepManager(CDM_StepManager &stepManager,
                          QWidget *parent = 0);
    ~GStepManager();

    CDM_StepManager* getStepManager() const;

    /*!
     *  \brief Retourne le nom de l'tape en paramtres (diffrent selon si les paramtres de l'tape sont modifis)
     */
    static QString staticGetStepName(Step &step);

private:
    CDM_StepManager         *_stepManager;              /*!< le gestionnaire d'etape */
    QTreeView               _view;                      /*!< La vue (un arbre) */
    QStandardItemModel      _model;                     /*!< Le modle de la vue */
    GTreeStepContextMenu    *_contextMenuStep;          /*!< Un menu qui s'affiche lors d'un clique droit sur une tape */
    QMenu                   *m_contextMenuGroupResult;  /*!< Context menu for the group of results */
    MyTreeDelegate          *_delegate;                 /*!< Un dlgu pour le style d'affichage des lments de la QTreeView */

    QMutex                  _mutexResList;
    QList<Result *>         _resToBeAddedList;          /*!< Une liste qui contient les rsultats qui doivent tre ajout (voir slot "resultToBeRemoved" et "resultToAdd") */

    QMutex                  _mutexItemRes;

    /*!
     *  \brief Cre les items compatible avec le modle pour une tape
     *
     *  \return la liste des items (un par colonne)
     */
    QList<QStandardItem *> createItemsForStep(Step &step);

    /*!
     *  \brief Cre les items compatible avec le modle pour un rsultat
     *
     *  \return la liste des items (un par colonne)
     */
    QList<QStandardItem *> createItemsForResult(Result &res);

    /*!
     *  \brief Dfini la couleur de fond des items reprsentant une tape
     */
    void setStepItemBackgroundColor(Step &step, QList<QStandardItem *> &list);

    /*!
     *  \brief Dfini la couleur de fond des items reprsentant un rsultat
     */
    void setResultItemBackgroundColor(ResultInfo info, QList<QStandardItem*> &list);

    /*!
     *  \brief Recherche l'item de la premire colonne correspondant  une tape
     *
     *  \return NULL si l'tape n'a pas t trouve, l'item correspondant sinon
     */
    MyQStandardItem* findItem(Step *step);

    /*!
     *  \brief Recherche l'item de la premire colonne correspondant  un rsultat
     *
     *  \return NULL si le rsultat n'a pas t trouv, l'item correspondant sinon
     */
    MyQStandardItem* findItem(Result *res);

    /*!
     *  \brief Retourne l'item (premire colonne) slectionn
     *
     *  \return NULL si aucune ligne n'est slectionne
     */
    MyQStandardItem* getSelectedItem();

    /*!
     *  \brief (INTERNE) Recherche l'item de la premire colonne correspondant
     *          une tape, mthode rcursive. Utiliser plutt findItem(Step *step).
     *
     *  \return NULL si l'tape n'a pas t trouve, l'item correspondant sinon
     */
    MyQStandardItem* recursiveFindItem(Step *step);

    /*!
     *  \brief (INTERNE) Recherche l'item de la premire colonne correspondant
     *          un rsultat, mthode rcursive. Utiliser plutt findItem(Result *res).
     *
     *  \return NULL si le rsultat n'a pas t trouv, l'item correspondant sinon
     */
    MyQStandardItem* recursiveFindItem(Result *res);
    MyQStandardItem* getItemForResult(QStandardItem *stepItem, Result *res);
    QList<MyQStandardItem *> getItemsForResult(QStandardItem *stepItem, Result *res);

    /*!
     *  \brief Appel lorsque l'item (colonne n) correspondant  une tape
     *          t modifi. Apelle les mthodes adquates correspondant au changement
     *         survenu en fonction du type de la colonne.
     */
    void stepDataChanged(MyQStandardItem *item);

    /*!
     *  \brief Appel lorsque l'item (colonne n) correspondant  un rsultat
     *          t modifi. Apelle les mthodes adquates correspondant au changement
     *         survenu en fonction du type de la colonne.
     */
    void resultDataChanged(MyQStandardItem *item);

    /*!
     *  \brief Affiche un message indiquant que l'tape n'est pas dbogable
     */
    void showMessageStepNotDebuggable();

    /*!
     *  \brief Vrifie certaines choses avant l'execution de l'tape pass en paramtre et affiche des messages  l'utilisateur en fonction.
     *
     *  \return true si l'execution peut continuer
     */
    bool checkExecuteStepAndShowWarningMessage(Step *step, bool debugMode);

    bool configureStepAndAdd(Step *newStep, Step *parentStep = NULL);

    void recursiveExpandCollapseItemOfStep(MyQStandardItem *item, bool expand);
    void recursiveExpandCollapseItemOfResultsGroup(MyQStandardItem *item, bool expand);

public slots:

    void addOpenFileStep(QString filePath);
    void addGenericStepAndConfigure(Step *parentStep, Step *stepToCopy);
    void addCanBeAddedFirstStepAndConfigure(StepCanBeAddedFirst *stepToCopy);
    void insertGenericStepAndConfigure(Step *parentStep, Step *stepToCopy);
    bool executeStep(Step *step = NULL);
    bool executeModifyStep(Step *step);
    bool executeOrForwardStepInDebugMode(Step *step = NULL);
    bool executeOrForwardStepFastInDebugMode(Step *step = NULL);
    bool configureInputResultOfStep(Step *step);
    bool configureStep(Step *step);
    bool removeStep(Step *step);
    bool loadResultStep(StepSerializable *step);
    bool editItemDrawableModelOfResult(Result *res);
    bool removeEditItemDrawableModelOfResult(Result *res);
    bool removeItemDrawableOfResult(Result *res);

    void showStepManagerOptions();
private slots:

    void stepAdded(Step *step);
    void stepInserted(int row, Step *step);
    void stepToBeRemoved(Step *step);
    void stepSettingsModified();

    void resultAdded(Result *res);
    void resultToBeClearedFromMemory(Result *res);
    void resultToBeRemoved(Result *res);
    void resultToBeSerialized(Result *res);

    void itemDataChanged(QStandardItem *item);
    void indexDoubleClicked(QModelIndex index);

    void resultToAdd(QStandardItem *parentItem, Result *res);
    void resultToRemove(QStandardItem *parentItem, MyQStandardItem *resItem);
    void itemToRemove(QStandardItem *item);

    void expandSelected();
    void expandAllTypeOfSelected();
    void collapseSelected();
    void collapseAllTypeOfSelected();

    void showViewContextMenu(const QPoint &point);

signals:

    void addResult(QStandardItem *parentItem, Result *res);
    void removeResult(QStandardItem *parentItem, MyQStandardItem *resItem);
    void removeItem(QStandardItem *item);
};

#endif // GSTEPMANAGER_H
