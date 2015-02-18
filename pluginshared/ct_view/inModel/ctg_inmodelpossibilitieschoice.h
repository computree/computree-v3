#ifndef CTG_INMODELPOSSIBILITIESCHOICE_H
#define CTG_INMODELPOSSIBILITIESCHOICE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QItemDelegate>

#include "def_ct_abstractgroupmodelout.h"
#include "def_ct_abstractgroupmodelin.h"

class CT_OutAbstractResultModelGroup;
class CT_InStdResultModelPossibility;

namespace Ui {
class CTG_InModelPossibilitiesChoice;
}

/*!
 *  Classe utilisé par le widget pour garder certaines informations dans l'arbre
 */
class CTG_InModelPossibilitiesChoiceItem : public QObject, public QStandardItem
{
    Q_OBJECT

public:

    CTG_InModelPossibilitiesChoiceItem(const QString & text) : QStandardItem(text) { _outModel = NULL; }

    bool isChecked() const;

    /*!
     *  \brief Ajouter un modèle d'entrée checkable
     */
    void addInModel(CT_InAbstractModel *model);

    /*!
     *  \brief Ajouter un modèle d'entrée sélectionnable mais dont la case à cocher ne s'affiche pas
     */
    void addDontChooseInModel(CT_InAbstractModel *model);

    void clearInModels();

    const QList<CT_InAbstractModel*>& getInModels() const;
    const QList<CT_InAbstractModel*>& getDontChooseInModels() const;

    CT_OutAbstractModel         *_outModel;

private:

    QList<CT_InAbstractModel*>      _inModels;
    QList<CT_InAbstractModel*>      _dontChooseInModels;

private slots:

    void setChecked(bool checked);
};

/*!
 *  Classe utilisé par le widget pour garder certaines informations dans l'arbre
 */
class CTG_InModelPossibilitiesChoiceItemCombo : public QStandardItem
{
public:

    CTG_InModelPossibilitiesChoiceItemCombo(const QString & text) : QStandardItem(text) { _inModelSelected = NULL; }

    /*!
     *  \brief Définir les modèles d'entrée dont les possibilités ont le même outModel que cette classe
     */
    void setInModels(const QList<CT_InAbstractModel *> &inModels);

    /*!
     *  \brief Définir les modèles d'entrée dont les possibilités ont le même outModel que cette classe mais qui
     *         ne doivent pas s'afficher dans la ComboBox
     */
    void setDontChooseInModels(const QList<CT_InAbstractModel *> &inModels);

    /*!
     *  \brief Si un modèle d'entrée est sélectionné, coche/décoche la possibilité (attention pas la vue mais l'objet)
     *         qui a le même "outModel" que cette classe
     *
     *  \return false si aucun modèle d'entrée n'est sélectionné
     */
    bool setPossibilityOfSelectedInModelChecked(bool checked);

    /*!
     *  \brief Sélectionne le modèle d'entrée "index" de la liste
     *
     *  \return false si l'index est erroné
     */
    bool setSelectedInModel(int index);

    const QList<CT_InAbstractModel*>& getInModels() const;
    const QList<CT_InAbstractModel*>& getDontChooseInModels() const;
    CT_InAbstractModel* getInModelSelected() const;

    CT_OutAbstractModel             *_outModel;

private:

    QList<CT_InAbstractModel*>      _inModels;
    QList<CT_InAbstractModel*>      _dontChooseInModels;
    CT_InAbstractModel              *_inModelSelected;

    CT_InAbstractModel* searchSelectedInModel() const;
    void setSelectedInModel(CT_InAbstractModel *model);
    CT_InStdModelPossibility *getPossibilityOfSelectedInModel() const;
};


/*!
 *  Classe utilisé par le widget pour afficher une combobox
 */
class CTG_InModelPossibilitiesChoiceComboBoxDelegate : public QItemDelegate
{
public:
      CTG_InModelPossibilitiesChoiceComboBoxDelegate(QObject *parent = 0);

      QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
      void setEditorData(QWidget *editor, const QModelIndex &index) const;
      void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
      void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

/*!
 *  Widget permettant d'afficher les modèles de sortie (ItemDrawable ou Groupe) d'une possibilité d'un
 *  résultat modèle d'entrée sous forme d'un arbre. L'utilisateur peut ainsi choisir (cocher)
 *  les ItemDrawable ou Groupe à utiliser par le traitement.
 */
class CTG_InModelPossibilitiesChoice : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTG_InModelPossibilitiesChoice(QWidget *parent = 0);
    ~CTG_InModelPossibilitiesChoice();

    static const int                            COLUMN_NAME = 0;
    static const int                            COLUMN_CHECK = 1;
    static const int                            COLUMN_COMBO = 2;

public slots:

    /*!
     *  \brief Définir la possibilité à afficher
     */
    void setInResultModelPossibility(const CT_InStdResultModelPossibility *possibility);

    void setReadOnly(bool enabled);

private:
    Ui::CTG_InModelPossibilitiesChoice          *ui;
    CT_InStdResultModelPossibility              *_possibility;

    QStandardItemModel                          _viewModel;

    bool                                        _itemChangedSlotIsEnabled;
    bool                                        m_readOnly;

    void clearModel();
    void constructModel();
    void constructHeader();

    QList<QStandardItem*> createItemsForOutResultModel(CT_OutAbstractResultModelGroup *resModel);

    void recursiveCreateItemsForGroupModel(QStandardItem *root,
                                           DEF_CT_AbstractGroupModelOut *group);

    void recursiveSetWhereIsSelectable(DEF_CT_AbstractGroupModelIn *inGroupModel);

    void recursiveSearchAndRefreshSelectedItem(QStandardItem *root,
                                               const CT_InAbstractModel *inModel,
                                               const QList<CT_InStdModelPossibility*> &list);

    void recursiveRemoveNotSelectable(QStandardItem *root);

    void recursiveSetCheckedParent(QStandardItem *parent, const bool &checked);
    void recursiveSetCheckedParentWhileModelHasNoPossibilityChecked(QStandardItem *parent);
    void recursiveSetCheckedChildren(QStandardItem *parent, const bool &checked);

    void setCheckedParent(QStandardItem *parent, const bool &checked);

private slots:

    void itemChanged(QStandardItem *item);
};

#endif // CTG_INMODELPOSSIBILITIESCHOICE_H
