#ifndef GSTEPMANAGER_H
#define GSTEPMANAGER_H

#include "cdm_stepmanager.h"

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
    explicit GStepManager(QWidget *parent = 0);
    ~GStepManager() {}

    void init(CDM_StepManager &stepManager);

private:
    CDM_StepManager         *_stepManager;          /*!< le gestionnaire d'etape */
    QTreeView               _view;                  /*!< La vue (un arbre) */
    QStandardItemModel      _model;                 /*!< Le modle de la vue */
    MyTreeDelegate          *_delegate;             /*!< Un dlgu pour le style d'affichage des lments de la QTreeView */

    QMutex                  _mutexResList;
    QList<CT_AbstractResult *>         _resToBeAddedList;      /*!< Une liste qui contient les rsultats qui doivent tre ajout (voir slot "resultToBeRemoved" et "resultToAdd") */

    QMutex                  _mutexItemRes;

    /*!
     *  \brief Cre les items compatible avec le modle pour une tape
     *
     *  \return la liste des items (un par colonne)
     */
    QList<QStandardItem *> createItemsForStep(CT_VirtualAbstractStep &step);

    /*!
     *  \brief Cre les items compatible avec le modle pour un rsultat
     *
     *  \return la liste des items (un par colonne)
     */
    QList<QStandardItem *> createItemsForResult(CT_AbstractResult &res);

    /*!
     *  \brief Dfini la couleur de fond des items reprsentant une tape
     */
    void setStepItemBackgroundColor(CT_VirtualAbstractStep &step, QList<QStandardItem *> &list);

    /*!
     *  \brief Dfini la couleur de fond des items reprsentant un rsultat
     */
    void setResultItemBackgroundColor(ResultInfo info, QList<QStandardItem*> &list);

    /*!
     *  \brief Recherche l'item de la premire colonne correspondant  une tape
     *
     *  \return NULL si l'tape n'a pas t trouve, l'item correspondant sinon
     */
    MyQStandardItem* findItem(CT_VirtualAbstractStep *step);

    /*!
     *  \brief Recherche l'item de la premire colonne correspondant  un rsultat
     *
     *  \return NULL si le rsultat n'a pas t trouv, l'item correspondant sinon
     */
    MyQStandardItem* findItem(CT_AbstractResult *res);

    /*!
     *  \brief Retourne l'item (premire colonne) slectionn
     *
     *  \return NULL si aucune ligne n'est slectionne
     */
    MyQStandardItem* getSelectedItem();

    /*!
     *  \brief (INTERNE) Recherche l'item de la premire colonne correspondant
     *          une tape, mthode rcursive. Utiliser plutt findItem(CT_VirtualAbstractStep *step).
     *
     *  \return NULL si l'tape n'a pas t trouve, l'item correspondant sinon
     */
    MyQStandardItem* recursiveFindItem(CT_VirtualAbstractStep *step);

    /*!
     *  \brief (INTERNE) Recherche l'item de la premire colonne correspondant
     *          un rsultat, mthode rcursive. Utiliser plutt findItem(CT_AbstractResult *res).
     *
     *  \return NULL si le rsultat n'a pas t trouv, l'item correspondant sinon
     */
    MyQStandardItem* recursiveFindItem(CT_AbstractResult *res);
    MyQStandardItem* getItemForResult(QStandardItem *stepItem, CT_AbstractResult *res);
    QList<MyQStandardItem *> getItemsForResult(QStandardItem *stepItem, CT_AbstractResult *res);

    /*!
     *  \brief Retourne le nom de l'tape en paramtres (diffrent selon si les paramtres de l'tape sont modifis)
     */
    QString getStepName(CT_VirtualAbstractStep &step);

private slots:

    void stepAdded(CT_VirtualAbstractStep *step);
    void stepToBeRemoved(CT_VirtualAbstractStep *step);

    void resultAdded(CT_AbstractResult *res);

    void resultToAdd(QStandardItem *parentItem, CT_AbstractResult *res);
    void itemToRemove(QStandardItem *item);

signals:

    void addResult(QStandardItem *parentItem, CT_AbstractResult *res);
    void removeItem(QStandardItem *item);
};

#endif // GSTEPMANAGER_H
