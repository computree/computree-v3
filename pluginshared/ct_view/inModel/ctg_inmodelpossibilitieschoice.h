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

/**
 * @brief This class inherit from QStandardItem and is a checkbox. Each checkbox in the treeview represent a
 *        out model ("_outModel" member) of the parent step (step that was before the current step. Current step is the new step that we want
 *        to add). If the user check this item : the possibility displayed in the combobox will be checked.
 */
class CTG_InModelCheckBox : public QObject, public QStandardItem
{
    Q_OBJECT

public:
    typedef QList<CT_InAbstractModel*> ModelContainer;
    typedef QListIterator<CT_InAbstractModel*> ModelContainerIterator;

    CTG_InModelCheckBox(const QString & text) : QStandardItem(text) { _outModel = NULL; }

    /**
     * @brief Return true if this checkbox is checked or not
     */
    bool isChecked() const;

    /**
     * @brief Add a in model in this item. If it is a normal model it will be "activated" by default.
     * @return Return true if one of possibility match with the out model ("_outModel") of this object. Otherwise return false
     *         and the model is not added. Return false if this checkbox contains already this model.
     */
    bool addInModel(const CT_InAbstractModel *model);

    /**
     * @brief Set the model in this model active or not. Only normal models can be desactivated (not "don't choose" models)
     * @return Return true if it was modified, false if it was not contained in this checkbox
     */
    bool setModelActive(const CT_InAbstractModel *model, bool active);

    /**
     * @brief Returns true if this model is active
     */
    bool isModelActive(const CT_InAbstractModel *model);

    /**
     * @brief Remove all models of this item
     */
    void clearInModels();

    /**
     * @brief Returns true if this checkbox contains this model or not
     */
    bool containsInModel(const CT_InAbstractModel *model) const;

    /**
     * @brief Returns true if this checkbox has normal models (active or not)
     */
    bool hasInModelsActiveOrNot() const;

    /**
     * @brief Returns true if this checkbox has models (active or not) of normal or "don't choose" type
     */
    bool hasAtLeastOneInModel() const;

    /**
     * @brief Returns true if this checkbox has normal models (only active)
     */
    bool hasInModelsActive() const;

    /**
     * @brief Returns true if this checkbox has "don't choose" models
     */
    bool hasDontChooseInModels() const;

    /**
     * @brief Returns the first "don't choose" model
     */
    CT_InAbstractModel* firstDontChooseInModel() const;

    /**
     * @brief Returns the first normal model
     */
    CT_InAbstractModel* firstInModelActive() const;

    /**
     * @brief Returns the checked or first normal "active" model or first "don't choose" model, in this order
     */
    CT_InAbstractModel* checkedOrFirstInModelActiveOrFirstDontChooseInModel() const;

    /**
     * @brief Returns the list of normal models (not active)
     */
    ModelContainer getInModelsNotActive() const;

    /**
     * @brief Returns the list of normal models (active or not)
     */
    ModelContainer getAllInModels() const;

    /**
     * @brief Returns the list of normal models only active
     */
    ModelContainer getInModelsActive() const;

    /**
     * @brief Returns the list of "don't choose" models
     */
    ModelContainer getDontChooseInModels() const;

    /**
     * @brief The out model of this checkbox. Each checkbox in the treeview represent a out model of the result.
     */
    CT_OutAbstractModel         *_outModel;


    static CT_InStdModelPossibility* staticSearchPossibilityForModel(const CT_InAbstractModel *model, const CT_OutAbstractModel *outModel);

private:
    typedef ModelContainer InternalModelContainer;
    typedef ModelContainerIterator InternalModelContainerIterator;

    ModelContainer      m_inModelsAndPossibilityNotActivated;
    ModelContainer      m_inModelsAndPossibilityActivated;
    ModelContainer      m_dontChooseModelsAndPossibility;

    void insertModel(const CT_InAbstractModel *model, CT_InStdModelPossibility *p);
    void clearInModels(InternalModelContainer &container);
    CT_InStdModelPossibility* getPossibilityForModel(const CT_InAbstractModel *model);

public slots:
    /**
     * @brief Called when a possibility was checked
     */
    void setChecked(bool checked);
};

/**
 * @brief This class inherit from QStandardItem and is a combobox. Each combobox in the treeview represent a
 *        out model ("_outModel" member) of the parent step (step that was before the current step. Current step is the new step that we want
 *        to add). It will show all model (that have a possibility that match with this out model) that can be selected by the user.
 */
class CTG_InModelComboBox : public QStandardItem
{
public:

    CTG_InModelComboBox(const CTG_InModelCheckBox &checkBoxItem, const QString & text) : QStandardItem(text), m_checkBoxItem(checkBoxItem) { _inModelSelected = NULL; m_possibility = NULL; }

    /**
     * @brief Check/Uncheck the possibility (of the current model if it was set) that match with the out model of this item
     * @return false if they are no current model, true otherwise
     */
    bool setPossibilityOfCurrentInModelChecked(bool checked);

    /**
     * @brief Set the current in model, you must call "updateDisplayData" if you want that the user show change in view
     */
    void setCurrentInModel(CT_InAbstractModel *model);

    /**
     * @brief Call it to update display data if you change the current model
     */
    void updateDisplayData();

    /**
     * @brief Get the current model
     */
    CT_InAbstractModel* currentInModel() const;

    /**
     * @brief Get the possibility (of the selected model if it was set) that match with the out model of this item
     */
    CT_InStdModelPossibility *getPossibilityOfCurrentInModel() const;

private:
    CT_InAbstractModel              *_inModelSelected;
    CT_InStdModelPossibility        *m_possibility;
    const CTG_InModelCheckBox       &m_checkBoxItem;

    /**
     * @brief Returns the possibility of the current model that match with the out model (_outModel) of this item
     */
    CT_InStdModelPossibility *findPossibilityOfCurrentInModel() const;
};


/**
 * @brief This class is a delegate to show a combobox and possibilities that the user can select in this combobox.
 */
class CTG_InModelPossibilitiesChoiceComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
      CTG_InModelPossibilitiesChoiceComboBoxDelegate(QObject *parent = 0);

      /**
       * @brief Just return the empty widget to show in the treeview
       */
      QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

      /**
       * @brief Fill the combobox with name of model to select
       */
      void setEditorData(QWidget *editor, const QModelIndex &index) const;

      /**
       * @brief Modify the selected possibility. Called when a user change the current element in the combobox.
       */
      void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

      /**
       * @brief Just update the geometry of the combobox
       */
      void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private slots:
      void comboBoxIndexChanged();
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
    bool                                        m_firstLoad;

    /**
     * @brief Remove all items in the treeview
     */
    void clearModel();

    /**
     * @brief (Re)Construct the model
     */
    void constructModel();

    /**
     * @brief (Re)Construct the header
     */
    void constructHeader();

    /**
     * @brief Call it to construct the treeview model for a result
     */
    QList<QStandardItem*> createItemsForOutResultModel(CT_OutAbstractResultModelGroup *resModel);

    /**
     * @brief Called by "createItemsForOutResultModel". Don't call it directly !
     */
    void recursiveCreateItemsForGroupModel(QStandardItem *root,
                                           DEF_CT_AbstractGroupModelOut *group);

    /**
     * @brief Call it after you have constructed the model to add models to checkbox
     */
    void constructCheckBox();
    void recursiveConstructCheckBox(QStandardItem *parentOfItemsToTest);

    /**
     * @brief Called by "recursiveConstructCheckBox". Don't call it directly !
     */
    void recursiveSearchModelThatMatchWithItemAndSetIt(const CT_InAbstractModel *inModel,
                                                       QStandardItem *parentOfItemToTest,
                                                       QStandardItem *itemToTest);

    /**
     * @brief Call it to refresh possibility in combobox and set what checkbox can be checked or not
     */
    void updateCheckBoxAndComboBox();

    /**
     * @brief Called by "updateCheckBoxAndComboBox". Don't call it directly !
     */
    void recursiveUpdateCheckBoxAndComboBox(QStandardItem *parentOfItemsToTest);

    /**
     * @brief Call it to remove all items in the treeview that can not be checked or that has no possibility. If
     *        childrens has possibilities the parent will not be removed of course !
     */
    void removeItemsThatCanNotBeChoosed();

    /**
     * @brief Called by "removeItemsThatCanNotBeChoosed". Don't call it directly !
     */
    void recursiveRemoveItemsThatCanNotBeChoosed(QStandardItem *itemToTest);

    static QStandardItem* staticToFirstColumn(QStandardItem *itemToConvert);
    static CTG_InModelCheckBox* staticToCheckBox(QStandardItem *itemToConvert);
    static CTG_InModelComboBox *staticToComboBox(QStandardItem *itemToConvert);

    /*void recursiveSetCheckedParent(QStandardItem *parent, const bool &checked);
    void recursiveSetCheckedParentWhileModelHasNoPossibilityChecked(QStandardItem *parent);
    void recursiveSetCheckedChildren(QStandardItem *parent, const bool &checked);

    void setCheckedParent(QStandardItem *parent, const bool &checked);*/

private slots:
    /**
     * @brief Called when a checkbox wes checked/unchecked or when an element in a combobox has changed
     * @param item : the item that was modified (checkbox or combobox)
     */
    void itemChanged(QStandardItem *item);
};

#endif // CTG_INMODELPOSSIBILITIESCHOICE_H
