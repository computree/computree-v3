#include "ctg_inmodelpossibilitieschoice.h"
#include "ui_ctg_inmodelpossibilitieschoice.h"

#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"

#include "ct_outresultmodelgroup.h"

#include "ct_virtualabstractstep.h"
#include "ct_inabstractgroupmodel.h"

#include <QComboBox>
#include <QElapsedTimer>
#include <QDebug>

// CTG_InModelPossibilitiesChoiceItem //

void CTG_InModelCheckBox::setChecked(bool checked)
{
    // si il est sélectionnable et que l'état a changé
    if(isCheckable()
            && (isChecked() != checked))
    {
        // on change son état
        setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    }
}

bool CTG_InModelCheckBox::isChecked() const
{
    return (checkState() == Qt::Checked);
}

bool CTG_InModelCheckBox::addInModel(const CT_InAbstractModel *model)
{
    Q_ASSERT(model != NULL);

    if(_outModel == NULL)
        return false;

    bool ok = false;

    if(!containsInModel(model))
    {
        CT_InStdModelPossibility *p = staticSearchPossibilityForModel(model, _outModel);

        if(p != NULL) {
            //qDebug() << "add " << model->displayableName() << " to out " << _outModel->displayableName();
            connect(p, SIGNAL(selectStateChange(bool)), this, SLOT(setChecked(bool)), Qt::DirectConnection);
            insertModel(model, p);
        }
    }

    return ok;
}

bool CTG_InModelCheckBox::setModelActive(const CT_InAbstractModel *model, bool active)
{
    Q_ASSERT(model != NULL);

    if(active) {
        int index = m_inModelsAndPossibilityNotActivated.indexOf((CT_InAbstractModel*)model);

        if(index < 0)
            return false;

        m_inModelsAndPossibilityActivated.append(m_inModelsAndPossibilityNotActivated.takeAt(index));
        return true;
    }

    int index = m_inModelsAndPossibilityActivated.indexOf((CT_InAbstractModel*)model);

    if(index < 0)
        return false;

    m_inModelsAndPossibilityNotActivated.append(m_inModelsAndPossibilityActivated.takeAt(index));
    return true;
}

bool CTG_InModelCheckBox::isModelActive(const CT_InAbstractModel *model)
{
    return m_inModelsAndPossibilityActivated.contains((CT_InAbstractModel*)model);
}

void CTG_InModelCheckBox::clearInModels()
{
    clearInModels(m_inModelsAndPossibilityNotActivated);
    clearInModels(m_inModelsAndPossibilityActivated);
    clearInModels(m_dontChooseModelsAndPossibility);
}

bool CTG_InModelCheckBox::containsInModel(const CT_InAbstractModel *model) const
{
    if(model->choiceMode() == CT_InAbstractModel::C_DontChoose)
        return m_dontChooseModelsAndPossibility.contains((CT_InAbstractModel*)model);

    if(m_inModelsAndPossibilityNotActivated.contains((CT_InAbstractModel*)model))
        return true;

    return m_inModelsAndPossibilityActivated.contains((CT_InAbstractModel*)model);
}

bool CTG_InModelCheckBox::hasInModelsActiveOrNot() const
{
    return !m_inModelsAndPossibilityNotActivated.isEmpty() || !m_inModelsAndPossibilityActivated.isEmpty();
}

bool CTG_InModelCheckBox::hasAtLeastOneInModel() const
{
    return hasInModelsActiveOrNot() || hasDontChooseInModels();
}

bool CTG_InModelCheckBox::hasInModelsActive() const
{
    return !m_inModelsAndPossibilityActivated.isEmpty();
}

bool CTG_InModelCheckBox::hasDontChooseInModels() const
{
    return !m_dontChooseModelsAndPossibility.isEmpty();
}

CT_InAbstractModel *CTG_InModelCheckBox::firstDontChooseInModel() const
{
    if(m_dontChooseModelsAndPossibility.isEmpty())
        return NULL;

    //return m_dontChooseModelsAndPossibility.begin().key();
    return m_dontChooseModelsAndPossibility.first();
}

CT_InAbstractModel *CTG_InModelCheckBox::firstInModelActive() const
{
    if(m_inModelsAndPossibilityActivated.isEmpty())
        return NULL;

    //return m_inModelsAndPossibility.begin().key();
    return m_inModelsAndPossibilityActivated.first();
}

CT_InAbstractModel *CTG_InModelCheckBox::checkedOrFirstInModelActiveOrFirstDontChooseInModel() const
{
    if(_outModel == NULL)
        return NULL;

    InternalModelContainerIterator it(m_inModelsAndPossibilityActivated);

    while(it.hasNext()) {
        CT_InAbstractModel *inModel = it.next();

        QList<CT_InStdModelPossibility*> l = inModel->getPossibilitiesSavedSelected();
        QListIterator<CT_InStdModelPossibility*> itL(l);

        while(itL.hasNext()) {
            CT_InStdModelPossibility *p = itL.next();

            if(p->outModel()->uniqueName() == _outModel->uniqueName())
                return inModel;
        }
    }

    CT_InAbstractModel *inModel = firstInModelActive();

    if(inModel == NULL)
        return firstDontChooseInModel();

    return inModel;
}

CTG_InModelCheckBox::ModelContainer CTG_InModelCheckBox::getInModelsNotActive() const
{
    //return m_inModelsAndPossibility.keys();
    return m_inModelsAndPossibilityNotActivated;
}

CTG_InModelCheckBox::ModelContainer CTG_InModelCheckBox::getAllInModels() const
{
    CTG_InModelCheckBox::ModelContainer l;
    l.append(m_inModelsAndPossibilityNotActivated);
    l.append(m_inModelsAndPossibilityActivated);

    return l;
}

CTG_InModelCheckBox::ModelContainer CTG_InModelCheckBox::getInModelsActive() const
{
    return m_inModelsAndPossibilityActivated;
}

CTG_InModelCheckBox::ModelContainer CTG_InModelCheckBox::getDontChooseInModels() const
{
    //return m_dontChooseModelsAndPossibility.keys();
    return m_dontChooseModelsAndPossibility;
}

void CTG_InModelCheckBox::insertModel(const CT_InAbstractModel *model, CT_InStdModelPossibility *p)
{
    Q_UNUSED(p)

    // check if we must add this model in the "_dontChooseInModels" container or in the "_inModels" container
    if(model->choiceMode() == CT_InAbstractModel::C_DontChoose)
        m_dontChooseModelsAndPossibility.append((CT_InAbstractModel*)model);
    else
        m_inModelsAndPossibilityActivated.append((CT_InAbstractModel*)model);
}

void CTG_InModelCheckBox::clearInModels(InternalModelContainer &container)
{
    CTG_InModelCheckBox::InternalModelContainerIterator it(container);

    while(it.hasNext())
    {
        CT_InAbstractModel *model = it.next();
        CT_InStdModelPossibility *p = getPossibilityForModel(model);
        disconnect(p, SIGNAL(selectStateChange(bool)), this, SLOT(setChecked(bool)));
    }

    container.clear();
}

CT_InStdModelPossibility *CTG_InModelCheckBox::getPossibilityForModel(const CT_InAbstractModel *model)
{
    return staticSearchPossibilityForModel(model, _outModel);
}

CT_InStdModelPossibility* CTG_InModelCheckBox::staticSearchPossibilityForModel(const CT_InAbstractModel *model, const CT_OutAbstractModel *outModel)
{
    Q_ASSERT(model != NULL);
    Q_ASSERT(outModel != NULL);

    QList<CT_InStdModelPossibility*> pList = model->getPossibilitiesSaved();
    QListIterator<CT_InStdModelPossibility*> it(pList);

    CT_InStdModelPossibility *possibilityFound = NULL;

    // for each possibility in this model
    while(it.hasNext() && (possibilityFound == NULL))
    {
        CT_InStdModelPossibility *p = it.next();

        // if this possibility match with the out model of this item
        if(p->outModel()->uniqueName() == outModel->uniqueName())
            return p; // we return the possibility
    }

    return NULL;
}

// CTG_InModelPossibilitiesChoiceItemCombo //

bool CTG_InModelComboBox::setPossibilityOfCurrentInModelChecked(bool checked)
{
    bool atLeastOneDontChooseSelected = false;

    // coche/decoche les DontChoose
    CTG_InModelCheckBox::ModelContainer models = m_checkBoxItem.getDontChooseInModels();
    CTG_InModelCheckBox::ModelContainerIterator it(models);

    while(it.hasNext())
    {
        CT_InStdModelPossibility *possibility = CTG_InModelCheckBox::staticSearchPossibilityForModel(it.next(), m_checkBoxItem._outModel);
        possibility->setSelected(checked);
        atLeastOneDontChooseSelected = true;
    }

    // coche la sélectionnée
    CT_InStdModelPossibility *possibility = getPossibilityOfCurrentInModel();

    if(possibility != NULL)
        return possibility->setSelected(checked);

    return atLeastOneDontChooseSelected;
}

CT_InAbstractModel* CTG_InModelComboBox::currentInModel() const
{
    return _inModelSelected;
}

void CTG_InModelComboBox::setCurrentInModel(CT_InAbstractModel *model)
{
    if(_inModelSelected != model) {
        _inModelSelected = model;
        if(_inModelSelected != NULL) {
            m_possibility = findPossibilityOfCurrentInModel();
            Q_ASSERT(m_possibility != NULL);
        } else {
            m_possibility = NULL;
        }
    }
}

void CTG_InModelComboBox::updateDisplayData()
{
    if(_inModelSelected != NULL)
        setData(_inModelSelected->displayableName(), Qt::DisplayRole);
    else
        setData("", Qt::DisplayRole);
}

CT_InStdModelPossibility* CTG_InModelComboBox::getPossibilityOfCurrentInModel() const
{
    return m_possibility;
}

CT_InStdModelPossibility *CTG_InModelComboBox::findPossibilityOfCurrentInModel() const
{
    return CTG_InModelCheckBox::staticSearchPossibilityForModel(_inModelSelected, m_checkBoxItem._outModel);
}

// CTG_InModelPossibilitiesChoiceComboBoxDelegate //

CTG_InModelPossibilitiesChoiceComboBoxDelegate::CTG_InModelPossibilitiesChoiceComboBoxDelegate(QObject *parent): QItemDelegate(parent)
{
}

QWidget* CTG_InModelPossibilitiesChoiceComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    return new QComboBox(parent);
}

void CTG_InModelPossibilitiesChoiceComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const QStandardItemModel* model = (const QStandardItemModel*)index.model();

    CTG_InModelComboBox *comboBoxItem = dynamic_cast<CTG_InModelComboBox*>(model->itemFromIndex(index));
    CTG_InModelCheckBox *checkBoxItem = dynamic_cast<CTG_InModelCheckBox*>(model->itemFromIndex(model->sibling(index.row(), CTG_InModelPossibilitiesChoice::COLUMN_CHECK, index)));

    if((comboBoxItem != NULL) && (checkBoxItem != NULL))
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        CTG_InModelCheckBox::ModelContainer inModelsActive = checkBoxItem->getInModelsActive();
        CTG_InModelCheckBox::ModelContainerIterator it(inModelsActive);

        while(it.hasNext()) {
            CT_InAbstractModel *inModel = it.next();
            comboBox->addItem(inModel->displayableName(), qVariantFromValue((void*)inModel));
        }

        CT_InAbstractModel *selectedModel = comboBoxItem->currentInModel();
        int index = comboBox->findData(qVariantFromValue((void*)selectedModel));

        Q_ASSERT(index != -1);

        comboBox->setCurrentIndex(index);

        connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIndexChanged()), Qt::QueuedConnection);
    }
}

void CTG_InModelPossibilitiesChoiceComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const QStandardItemModel* stdModel = (const QStandardItemModel*)index.model();

    CTG_InModelComboBox *comboBoxItem = dynamic_cast<CTG_InModelComboBox*>(stdModel->itemFromIndex(index));
    CTG_InModelCheckBox *checkBoxItem = dynamic_cast<CTG_InModelCheckBox*>(stdModel->itemFromIndex(model->sibling(index.row(), CTG_InModelPossibilitiesChoice::COLUMN_CHECK, index)));

    if((comboBoxItem != NULL) && (checkBoxItem != NULL))
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);

        bool previousChecked = checkBoxItem->isChecked();

        // uncheck previous selected model
        comboBoxItem->setPossibilityOfCurrentInModelChecked(false);

        // set new selected model
        CT_InAbstractModel *newSelectedModel = (CT_InAbstractModel*)comboBox->currentData().value<void*>();
        comboBoxItem->setCurrentInModel(newSelectedModel);
        comboBoxItem->updateDisplayData();

        // check/uncheck the new selected model
        comboBoxItem->setPossibilityOfCurrentInModelChecked(previousChecked);

        const_cast<CTG_InModelPossibilitiesChoiceComboBoxDelegate*>(this)->closeEditor(editor, QAbstractItemDelegate::NoHint);
    }
}

void CTG_InModelPossibilitiesChoiceComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

void CTG_InModelPossibilitiesChoiceComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QStandardItemModel* model = (const QStandardItemModel*)index.model();

    CTG_InModelComboBox *comboBoxItem = dynamic_cast<CTG_InModelComboBox*>(model->itemFromIndex(index));

    if((comboBoxItem != NULL) && (comboBoxItem->isEditable())) {
        QStyleOptionComboBox box;
        box.palette = option.palette;
        box.rect = option.rect;
        box.state = QStyle::State_Active;
        box.currentText = index.data().toString();
        painter->save();
        QApplication::style()->drawComplexControl(QStyle::CC_ComboBox,&box, painter);
        QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel,&box, painter);
        painter->restore();
        return;
    }

    QItemDelegate::paint(painter, option, index);
}

void CTG_InModelPossibilitiesChoiceComboBoxDelegate::comboBoxIndexChanged()
{
    emit commitData((QWidget*)sender());
}

// CTG_InModelPossibilitiesChoice //

CTG_InModelPossibilitiesChoice::CTG_InModelPossibilitiesChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTG_InModelPossibilitiesChoice)
{
    ui->setupUi(this);

    ui->treeView->setModel(&_viewModel);
    ui->treeView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->treeView->setItemDelegateForColumn(COLUMN_COMBO, new CTG_InModelPossibilitiesChoiceComboBoxDelegate(ui->treeView));
    ui->treeView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    connect(&_viewModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)), Qt::DirectConnection);

    _possibility = NULL;
    _itemChangedSlotIsEnabled = true;
    m_readOnly = false;

    constructHeader();
}

CTG_InModelPossibilitiesChoice::~CTG_InModelPossibilitiesChoice()
{
    delete ui;
}

void CTG_InModelPossibilitiesChoice::setInResultModelPossibility(const CT_InStdResultModelPossibility *possibility)
{
    _possibility = (CT_InStdResultModelPossibility*)possibility;

    constructModel();
}

void CTG_InModelPossibilitiesChoice::setReadOnly(bool enabled)
{
    m_readOnly = enabled;
}

void CTG_InModelPossibilitiesChoice::clearModel()
{
    _viewModel.invisibleRootItem()->removeRows(0, _viewModel.invisibleRootItem()->rowCount());
}

void CTG_InModelPossibilitiesChoice::constructModel()
{
    m_firstLoad = true;
    clearModel();
    constructHeader();

    if(_possibility != NULL)
    {
        // on récupère le résultat modèle de sortie de cette possibilité
        CT_OutAbstractResultModelGroup *outResModel = dynamic_cast<CT_OutAbstractResultModelGroup*>(_possibility->outModel());
        DEF_CT_AbstractGroupModelIn *inGroupModel =  dynamic_cast<DEF_CT_AbstractGroupModelIn*>(_possibility->inModel());

        if((outResModel != NULL)
                && (inGroupModel != NULL))
        {
            // on construit l'arbre de ses modèles
            QList<QStandardItem*> items = createItemsForOutResultModel(outResModel);

            if(!items.isEmpty())
            {
                _itemChangedSlotIsEnabled = false;

                _viewModel.invisibleRootItem()->appendRow(items);

                // on va parcourir l'arbre pour définir quelle modele (et possibilité) est utilise par tel checkbox
                constructCheckBox();

                // on parcours pour supprimer les éléments non sélectionnable
                removeItemsThatCanNotBeChoosed();

                _itemChangedSlotIsEnabled = true;
            }

            ui->treeView->expandAll();
        }
    }

    m_firstLoad = false;
    setMinimumWidth(ui->treeView->header()->length());
}

void CTG_InModelPossibilitiesChoice::constructHeader()
{
    QString stepName;

    if(_possibility != NULL)
    {
        stepName = _possibility->outModel()->step()->getStepCustomName() == _possibility->outModel()->step()->getStepDisplayableName() ? _possibility->outModel()->step()->getStepExtendedDisplayableName() : _possibility->outModel()->step()->getStepCustomName();
    }

    QStringList header;
    header << (tr("Sortie") + (!stepName.isEmpty() ? (tr(" de ") + stepName) : ""));
    header << tr("Sélection");
    header << tr("Entrée");

    _viewModel.setHorizontalHeaderLabels(header);

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    ui->treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setResizeMode(2, QHeaderView::ResizeToContents);
#else
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
#endif
}

QList<QStandardItem*> CTG_InModelPossibilitiesChoice::createItemsForOutResultModel(CT_OutAbstractResultModelGroup *resModel)
{
    QList<QStandardItem*> retList;

    // Nom du résultat modèle d'entrée
    QStandardItem *rootName = new QStandardItem(resModel->displayableName());
    rootName->setEditable(false);

    retList.append(rootName);

    // un item vide
    CTG_InModelCheckBox *emptyItem = new CTG_InModelCheckBox("");
    emptyItem->setEditable(false);
    emptyItem->setCheckable(false);
    retList.append(emptyItem);

    // un item vide
    CTG_InModelComboBox *emptyItem2 = new CTG_InModelComboBox(*emptyItem, "");
    emptyItem2->setEditable(false);
    retList.append(emptyItem2);

    recursiveCreateItemsForGroupModel(rootName,
                                      resModel->rootGroup());

    return retList;
}

void CTG_InModelPossibilitiesChoice::recursiveCreateItemsForGroupModel(QStandardItem *root,
                                                                       DEF_CT_AbstractGroupModelOut *group)
{
    QList<QStandardItem*> list;

    // le nom du modèle de sortie
    QStandardItem *rootItem = new QStandardItem(group->displayableName());
    rootItem->setEditable(false);

    list.append(rootItem);

    // un item pour la sélection
    CTG_InModelCheckBox *selectableRootItem = new CTG_InModelCheckBox("");
    selectableRootItem->setEditable(false);
    selectableRootItem->setEnabled(!m_readOnly);
    selectableRootItem->_outModel = group;
    list.append(selectableRootItem);

    // un item pour la sélection du modèle d'entrée si il y en a plusieurs (combobox)
    CTG_InModelComboBox *comboBoxRootItem = new CTG_InModelComboBox(*selectableRootItem, "");
    comboBoxRootItem->setEditable(false);
    comboBoxRootItem->setEnabled(!m_readOnly);
    list.append(comboBoxRootItem);

    QListIterator< DEF_CT_AbstractGroupModelOut* > itG(group->groups());

    while(itG.hasNext())
        recursiveCreateItemsForGroupModel(rootItem, itG.next());

    QListIterator<CT_OutAbstractSingularItemModel*> itI(group->items());

    while(itI.hasNext())
    {
        QList<QStandardItem*> list2;

        CT_OutAbstractSingularItemModel *item = itI.next();

        // le nom du modèle de sortie
        QStandardItem *itemItem = new QStandardItem(item->displayableName());
        itemItem->setEditable(false);
        list2.append(itemItem);

        // un item pour la sélection (case à cocher)
        CTG_InModelCheckBox *checkableItemItem = new CTG_InModelCheckBox("");
        checkableItemItem->setEditable(false);
        checkableItemItem->setEnabled(!m_readOnly);
        checkableItemItem->_outModel = item;
        list2.append(checkableItemItem);

        // un item pour la sélection du modèle d'entrée si il y en a plusieurs (combobox)
        CTG_InModelComboBox *comboBoxItemItem = new CTG_InModelComboBox(*checkableItemItem, "");
        comboBoxItemItem->setEditable(false);
        comboBoxItemItem->setEnabled(!m_readOnly);
        list2.append(comboBoxItemItem);

        QListIterator<CT_OutAbstractItemAttributeModel*> itAtt(item->itemAttributes());

        while(itAtt.hasNext())
        {
            QList<QStandardItem*> list3;

            CT_OutAbstractItemAttributeModel *itemAtt = itAtt.next();

            // le nom du modèle de sortie
            QStandardItem *itemItemAtt = new QStandardItem(itemAtt->displayableName());
            itemItemAtt->setEditable(false);
            list3.append(itemItemAtt);

            // un item pour la sélection (case à cocher)
            CTG_InModelCheckBox *checkableItemItemAtt = new CTG_InModelCheckBox("");
            checkableItemItemAtt->setEditable(false);
            checkableItemItemAtt->setEnabled(!m_readOnly);
            checkableItemItemAtt->_outModel = itemAtt;
            list3.append(checkableItemItemAtt);

            // un item pour la sélection du modèle d'entrée si il y en a plusieurs (combobox)
            CTG_InModelComboBox *comboBoxItemItemAtt = new CTG_InModelComboBox(*checkableItemItemAtt, "");
            comboBoxItemItemAtt->setEditable(false);
            comboBoxItemItemAtt->setEnabled(!m_readOnly);
            list3.append(comboBoxItemItemAtt);

            itemItem->appendRow(list3);
        }

        rootItem->appendRow(list2);
    }

    root->appendRow(list);
}

void CTG_InModelPossibilitiesChoice::constructCheckBox()
{
    _itemChangedSlotIsEnabled = false;

    QStandardItem *parentOfItemsToTest = _viewModel.invisibleRootItem();

    recursiveConstructCheckBox(parentOfItemsToTest);

    int size = parentOfItemsToTest->rowCount();

    for(int i=0; i<size; ++i)
    {
        QStandardItem *child = parentOfItemsToTest->child(i, 0);

        int childSize = child->rowCount();

        // only for children of the result item
        for(int j=0; j<childSize; ++j)
        {
            QStandardItem *childJ = child->child(j, 0);

            // get the checkbox and combobox item to set in the combobox the selected model or the first model of the checkbox item
            CTG_InModelCheckBox *checkableItem = staticToCheckBox(childJ);
            CTG_InModelComboBox *comboItem = staticToComboBox(childJ);

            Q_ASSERT(checkableItem != NULL);
            Q_ASSERT(comboItem != NULL);

            CT_InAbstractModel *model = checkableItem->checkedOrFirstInModelActiveOrFirstDontChooseInModel();

            // set it the selected or first model
            comboItem->setCurrentInModel(model);
        }
    }

    // update (recursively) all checkbox and combobox now
    updateCheckBoxAndComboBox();

    _itemChangedSlotIsEnabled = true;
}

void CTG_InModelPossibilitiesChoice::recursiveConstructCheckBox(QStandardItem *parentOfItemsToTest)
{
    int size = parentOfItemsToTest->rowCount();

    for(int i=0; i<size; ++i)
    {
        QStandardItem *child = parentOfItemsToTest->child(i, 0);

        // go recursively in childrens of the model and search model that can be added in this item, set it if can
        recursiveSearchModelThatMatchWithItemAndSetIt(_possibility->inModel(), parentOfItemsToTest, child);

        // go recursively in childrens of the tree and do the same process
        recursiveConstructCheckBox(child);
    }
}

void CTG_InModelPossibilitiesChoice::recursiveSearchModelThatMatchWithItemAndSetIt(const CT_InAbstractModel *inModel,
                                                                                   QStandardItem *parentOfItemToTest,
                                                                                   QStandardItem *itemToTest)
{
    /*qDebug() << inModel->displayableName() << " possibilities : ";
    QList<CT_InStdModelPossibility*> pp = inModel->getPossibilitiesSaved();
    foreach (CT_InStdModelPossibility *p, pp) {
        qDebug() << p->outModel()->displayableName();
    }*/
    // get the checkable item if exist in this row
    CTG_InModelCheckBox *checkableItem = staticToCheckBox(itemToTest);

    Q_ASSERT(checkableItem != NULL);

    // if this checkable item match with a possibility of this in model, the model was added to "inModels" or
    // "dontChooseInModels" automatically
    checkableItem->addInModel(inModel);

    // go recursively to next childrens in the tree
    QList<CT_InAbstractModel*> l = inModel->childrensStaticCast<CT_InAbstractModel>();
    QListIterator<CT_InAbstractModel*> it(l);

    while(it.hasNext())
        recursiveSearchModelThatMatchWithItemAndSetIt(it.next(), parentOfItemToTest, itemToTest);
}

void CTG_InModelPossibilitiesChoice::updateCheckBoxAndComboBox()
{
    _itemChangedSlotIsEnabled = false;

    QStandardItem *parentOfItemsToTest = _viewModel.invisibleRootItem();

    int size = parentOfItemsToTest->rowCount();

    for(int i=0; i<size; ++i)
        recursiveUpdateCheckBoxAndComboBox(parentOfItemsToTest->child(i, 0));

    _itemChangedSlotIsEnabled = true;
}

void CTG_InModelPossibilitiesChoice::recursiveUpdateCheckBoxAndComboBox(QStandardItem *parentOfItemsToTest)
{
    int size = parentOfItemsToTest->rowCount();

    for(int i=0; i<size; ++i)
    {
        QStandardItem *child = parentOfItemsToTest->child(i, 0);

        CTG_InModelCheckBox *checkableItem = staticToCheckBox(child);
        CTG_InModelComboBox *comboItem = staticToComboBox(child);

        Q_ASSERT(checkableItem != NULL);
        Q_ASSERT(comboItem != NULL);

        // get the inModel of the parent combobox
        CT_InAbstractModel *parentSelectedModel = NULL;
        CTG_InModelComboBox *parentComboBoxItem = staticToComboBox(parentOfItemsToTest);

        if(parentComboBoxItem != NULL)
            parentSelectedModel = parentComboBoxItem->currentInModel();

        CTG_InModelCheckBox::ModelContainer inModels = checkableItem->getAllInModels();
        CTG_InModelCheckBox::ModelContainerIterator it(inModels);

        while(it.hasNext()) {
            CT_InAbstractModel *model = it.next();

            bool active = (parentSelectedModel == NULL) || !staticIsRecursiveCurrentInModelNULL(parentComboBoxItem);

            if((parentSelectedModel != NULL) && active)
                active = (parentSelectedModel == model->parentModel());

            // set the model "active" (displayed and selectable by the user) only if it has no selected parent model or if the parent model is its parent
            // otherwise we set it not active
            checkableItem->setModelActive(model, active);

            // if we desactivated it and the combobox selected model is this model
            if(!active && (comboItem->currentInModel() == model)) {
                // we uncheck it
                comboItem->setPossibilityOfCurrentInModelChecked(false);

                // and remove the selected model of the combobox
                comboItem->setCurrentInModel(NULL);
            }
        }

        // if the combobox has no selected model, we set it the selected "active" model or the first "active" model or the first "don't choose" model
        if(comboItem->currentInModel() == NULL)
            comboItem->setCurrentInModel(checkableItem->checkedOrFirstInModelActiveOrFirstDontChooseInModel());

        // checkbox can be check if it has at least one model
        checkableItem->setCheckable(checkableItem->hasAtLeastOneInModel());

        // checkbox is enable if we are not in read only mode and if it has at least one model "active"
        checkableItem->setEnabled(!m_readOnly && checkableItem->hasInModelsActive());

        // combobox is editable if checkbox is enabled and the user must choose between multiple element
        comboItem->setEditable(checkableItem->isEnabled() && checkableItem->getInModelsActive().size() > 1);

        // update the widget display data
        comboItem->updateDisplayData();

        // get the possibility selected of the current model
        CT_InStdModelPossibility *poss = comboItem->getPossibilityOfCurrentInModel();

        // set checkbox checked if a possibility is selected
        if(poss != NULL)
            checkableItem->setChecked(poss->isSelected());
        else
            checkableItem->setChecked(false);

        recursiveUpdateCheckBoxAndComboBox(child);
    }
}

void CTG_InModelPossibilitiesChoice::removeItemsThatCanNotBeChoosed()
{
    recursiveRemoveItemsThatCanNotBeChoosed(_viewModel.invisibleRootItem());
}

void CTG_InModelPossibilitiesChoice::recursiveRemoveItemsThatCanNotBeChoosed(QStandardItem *itemToTest)
{
    int size = itemToTest->rowCount();

    for(int i=0; i<size; ++i)
    {
        QStandardItem *child = itemToTest->child(i, 0);

        if(child->rowCount() > 0)
            recursiveRemoveItemsThatCanNotBeChoosed(itemToTest->child(i, 0));

        CTG_InModelCheckBox *checkableItem = static_cast<CTG_InModelCheckBox*>(itemToTest->child(i, COLUMN_CHECK));

        if(!checkableItem->isCheckable() && (child->rowCount() == 0))
        {
            itemToTest->removeRow(i);
            --i;
            --size;
        }
    }
}

QStandardItem* CTG_InModelPossibilitiesChoice::staticToFirstColumn(QStandardItem *itemToConvert)
{
    QStandardItem *root = itemToConvert->model()->invisibleRootItem();
    Q_ASSERT(itemToConvert != root);

    QStandardItem *parent = itemToConvert->parent();

    if(parent == NULL)
        parent = root;

    return parent->child(itemToConvert->row(), 0);
}

CTG_InModelCheckBox *CTG_InModelPossibilitiesChoice::staticToCheckBox(QStandardItem *itemToConvert)
{
    QStandardItem *root = itemToConvert->model()->invisibleRootItem();
    Q_ASSERT(itemToConvert != root);

    QStandardItem *parent = itemToConvert->parent();

    if(parent == NULL)
        parent = root;

    return static_cast<CTG_InModelCheckBox*>(parent->child(itemToConvert->row(), COLUMN_CHECK));
}

CTG_InModelComboBox *CTG_InModelPossibilitiesChoice::staticToComboBox(QStandardItem *itemToConvert)
{
    QStandardItem *root = itemToConvert->model()->invisibleRootItem();
    Q_ASSERT(itemToConvert != root);

    QStandardItem *parent = itemToConvert->parent();

    if(parent == NULL)
        parent = root;

    return static_cast<CTG_InModelComboBox*>(parent->child(itemToConvert->row(), COLUMN_COMBO));
}

bool CTG_InModelPossibilitiesChoice::staticIsRecursiveCurrentInModelNULL(CTG_InModelComboBox *parentToTest)
{
    if(parentToTest == NULL)
        return true;

    if(parentToTest != NULL) {
        if(parentToTest->currentInModel() != NULL)
            return false;
    }

    QStandardItem *first = staticToFirstColumn(parentToTest);

    if((first->parent() != NULL) && (first->parent() != parentToTest->model()->invisibleRootItem()))
        return staticIsRecursiveCurrentInModelNULL(staticToComboBox(first->parent()));

    return true;
}

void CTG_InModelPossibilitiesChoice::itemChanged(QStandardItem *item)
{
    if(_itemChangedSlotIsEnabled)
    {
        // if a checkbox was modified
        if(item->column() == COLUMN_CHECK) {
            CTG_InModelCheckBox *checkableItem = static_cast<CTG_InModelCheckBox*>(item);

            // we get his current state
            bool checked = checkableItem->isChecked();

            // we get the combobox to check/uncheck the selected model
            CTG_InModelComboBox *comboItem = staticToComboBox(item);

            // if the selected model state has changed
            if(comboItem->setPossibilityOfCurrentInModelChecked(checked))
            {
                // if we check it
                if(checked)
                {
                    // we check as its parent
                    checkableItem = staticToCheckBox(item->parent());

                    if(checkableItem != NULL)
                        checkableItem->setChecked(true);
                }
                else
                {
                    // else we uncheck his childrens (recursively)
                    QStandardItem *itemFirstColumn = staticToFirstColumn(item);

                    int size = itemFirstColumn->rowCount();

                    for(int i=0; i<size; ++i)
                        static_cast<CTG_InModelCheckBox*>(itemFirstColumn->child(i, COLUMN_CHECK))->setChecked(false);
                }
            }
        } else if(item->column() == COLUMN_COMBO) {
            // else if a combobox was modified, we check what we must change in other combobox, etc...
            updateCheckBoxAndComboBox();
        }
    }
}
