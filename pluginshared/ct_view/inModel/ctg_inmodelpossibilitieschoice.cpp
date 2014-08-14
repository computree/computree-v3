#include "ctg_inmodelpossibilitieschoice.h"
#include "ui_ctg_inmodelpossibilitieschoice.h"

#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"

#include "ct_outresultmodelgroup.h"

#include "ct_virtualabstractstep.h"
#include "ct_inabstractgroupmodel.h"

#include <QComboBox>

// CTG_InModelPossibilitiesChoiceItem //

void CTG_InModelPossibilitiesChoiceItem::setChecked(bool checked)
{
    // si il est sélectionnable et que l'état a changé
    if(isCheckable()
            && (isChecked() != checked))
    {
        // on change son état
        setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    }
}

bool CTG_InModelPossibilitiesChoiceItem::isChecked() const
{
    return (checkState() == Qt::Checked);
}

void CTG_InModelPossibilitiesChoiceItem::addInModel(CT_InAbstractModel *model)
{
    if(!_inModels.contains(model))
    {
        _inModels.append(model);

        // connecte le signal "selectStateChange(...)" des possibilités dont
        // le modèle est le même que celui de cette classe
        QList<CT_InStdModelPossibility*> pList = model->getPossibilitiesSaved();
        QListIterator<CT_InStdModelPossibility*> it(pList);

        while(it.hasNext())
        {
            CT_InStdModelPossibility *p = it.next();

            if(p->outModel()->uniqueName() == _outModel->uniqueName())
            {
                if(!isChecked())
                    setChecked(p->isSelected());

                connect(p, SIGNAL(selectStateChange(bool)), this, SLOT(setChecked(bool)), Qt::DirectConnection);
            }
        }
    }
}

void CTG_InModelPossibilitiesChoiceItem::addDontChooseInModel(CT_InAbstractModel *model)
{
    if(!_dontChooseInModels.contains(model))
    {
        _dontChooseInModels.append(model);

        // connecte le signal "selectStateChange(...)" des possibilités dont
        // le modèle est le même que celui de cette classe
        QList<CT_InStdModelPossibility*> pList = model->getPossibilitiesSaved();
        QListIterator<CT_InStdModelPossibility*> it(pList);

        while(it.hasNext())
        {
            CT_InStdModelPossibility *p = it.next();

            if(p->outModel()->uniqueName() == _outModel->uniqueName())
            {
                if(!isChecked())
                    setChecked(p->isSelected());

                connect(p, SIGNAL(selectStateChange(bool)), this, SLOT(setChecked(bool)), Qt::DirectConnection);
            }
        }
    }
}

void CTG_InModelPossibilitiesChoiceItem::clearInModels()
{
    _dontChooseInModels.clear();

    while(!_inModels.isEmpty())
    {
        CT_InAbstractModel *model = _inModels.takeLast();

        // déconnecte le signal "selectStateChange(...)" des possibilités dont
        // le modèle est le même que celui de cette classe
        QList<CT_InStdModelPossibility*> pList = model->getPossibilitiesSaved();
        QListIterator<CT_InStdModelPossibility*> it(pList);

        while(it.hasNext())
        {
            CT_InStdModelPossibility *p = it.next();

            if(p->outModel()->uniqueName() == _outModel->uniqueName())
                disconnect(p, SIGNAL(selectStateChange(bool)), this, SLOT(setChecked(bool)));
        }
    }
}

const QList<CT_InAbstractModel*>& CTG_InModelPossibilitiesChoiceItem::getInModels() const
{
    return _inModels;
}

const QList<CT_InAbstractModel*>& CTG_InModelPossibilitiesChoiceItem::getDontChooseInModels() const
{
    return _dontChooseInModels;
}

// CTG_InModelPossibilitiesChoiceItemCombo //

bool CTG_InModelPossibilitiesChoiceItemCombo::setPossibilityOfSelectedInModelChecked(bool checked)
{
    bool atLeastOneDontChooseSelected = false;

    // coche les DontChoose
    QListIterator<CT_InAbstractModel*> it(getDontChooseInModels());

    while(it.hasNext())
    {
        QList<CT_InStdModelPossibility*> list = it.next()->getPossibilitiesSaved();
        QListIterator<CT_InStdModelPossibility*> itP(list);
        bool continueLoop = true;

        // pour chacune d'elles
        while(itP.hasNext()
              && continueLoop)
        {
            CT_InStdModelPossibility *possibility = itP.next();

            // si la possibilité partage le même modèle de sortie
            // que celui qui a été coché/décoché
            if(possibility->outModel()->uniqueName() == _outModel->uniqueName())
            {
                possibility->setSelected(checked);
                continueLoop = false;
                atLeastOneDontChooseSelected = true;
            }
        }
    }

    // coche la sélectionnée
    CT_InStdModelPossibility *possibility = getPossibilityOfSelectedInModel();

    if(possibility != NULL)
    {
        // on change son état
        return possibility->setSelected(checked);
    }

    return atLeastOneDontChooseSelected;
}

bool CTG_InModelPossibilitiesChoiceItemCombo::setSelectedInModel(int index)
{
    if((index < 0)
            || (index >= _inModels.size()))
        return false;

    setSelectedInModel(_inModels.at(index));
    return true;
}

void CTG_InModelPossibilitiesChoiceItemCombo::setInModels(const QList<CT_InAbstractModel*> &inModels)
{
    _inModels = inModels;

    // on met à jour le nom du modèle d'entrée dont la possibilité est sélectionnée
    setSelectedInModel(searchSelectedInModel());
}

void CTG_InModelPossibilitiesChoiceItemCombo::setDontChooseInModels(const QList<CT_InAbstractModel*> &inModels)
{
    _dontChooseInModels = inModels;
}

const QList<CT_InAbstractModel*>& CTG_InModelPossibilitiesChoiceItemCombo::getInModels() const
{
    return _inModels;
}

const QList<CT_InAbstractModel*>& CTG_InModelPossibilitiesChoiceItemCombo::getDontChooseInModels() const
{
    return _dontChooseInModels;
}

CT_InAbstractModel* CTG_InModelPossibilitiesChoiceItemCombo::getInModelSelected() const
{
    return _inModelSelected;
}

CT_InAbstractModel* CTG_InModelPossibilitiesChoiceItemCombo::searchSelectedInModel() const
{
    if(_inModels.isEmpty())
        return NULL;

    QListIterator<CT_InAbstractModel*> it(_inModels);

    // on va rechercher le premier modèle qui a une possibilité
    // sélectionné dont le outModel est égual à celui de notre classe
    while(it.hasNext())
    {
        CT_InAbstractModel *model = it.next();

        // on récupère ses possibilités
        QList<CT_InStdModelPossibility*> list = model->getPossibilitiesSavedSelected();
        QListIterator<CT_InStdModelPossibility*> itP(list);

        // pour chacune d'elles
        while(itP.hasNext())
        {
            CT_InStdModelPossibility *possibility = itP.next();

            // si la possibilité partage le même modèle de sortie
            // que celui de cette classe
            if(possibility->outModel()->uniqueName() == _outModel->uniqueName())
            {
                // on retourne ce modèle
                return model;
            }
        }
    }

    // si aucun modèle n'a de possibilité sélectionné
    // on retourne le premier de la liste
    return _inModels.first();
}

void CTG_InModelPossibilitiesChoiceItemCombo::setSelectedInModel(CT_InAbstractModel *model)
{
    _inModelSelected = model;

    if((_inModelSelected != NULL) && (isEditable()))
        setData(_inModelSelected->displayableName(), Qt::DisplayRole);
    else
        setData("", Qt::DisplayRole);
}

CT_InStdModelPossibility* CTG_InModelPossibilitiesChoiceItemCombo::getPossibilityOfSelectedInModel() const
{
    // si un inItemModel est sélectionné (toujours vrai sinon bug dans le code !)
    if(_inModelSelected != NULL)
    {
        // on récupère ses possibilités
        QList<CT_InStdModelPossibility*> list = _inModelSelected->getPossibilitiesSaved();
        QListIterator<CT_InStdModelPossibility*> itP(list);

        // pour chacune d'elles
        while(itP.hasNext())
        {
            CT_InStdModelPossibility *possibility = itP.next();

            // si la possibilité partage le même modèle de sortie
            // que celui qui a été coché/décoché
            if(possibility->outModel()->uniqueName() == _outModel->uniqueName())
            {
                // on la retourne
                return possibility;
            }
        }
    }

    return NULL;
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
    const CTG_InModelPossibilitiesChoiceItemCombo *comboModel = (CTG_InModelPossibilitiesChoiceItemCombo*)((const QStandardItemModel*)index.model())->itemFromIndex(index);

    if(comboModel != NULL)
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);

        QListIterator<CT_InAbstractModel*> it(comboModel->getInModels());

        while(it.hasNext())
            comboBox->addItem(it.next()->displayableName());

        if(comboModel->getInModelSelected() != NULL)
            comboBox->setCurrentIndex(comboBox->findText(comboModel->getInModelSelected()->displayableName()));
    }
}

void CTG_InModelPossibilitiesChoiceComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    CTG_InModelPossibilitiesChoiceItemCombo *comboModel = (CTG_InModelPossibilitiesChoiceItemCombo*)((QStandardItemModel*)model)->itemFromIndex(index);

    if(comboModel != NULL)
    {
        bool isChecked = ((CTG_InModelPossibilitiesChoiceItem*)comboModel->parent()->child(comboModel->row(), CTG_InModelPossibilitiesChoice::COLUMN_CHECK))->isChecked();

        QComboBox *comboBox = static_cast<QComboBox*>(editor);

        if(comboModel->getInModelSelected() != NULL)
        {
            int lastValue = comboModel->getInModels().indexOf(comboModel->getInModelSelected());

            // si le nouveau modèle sélectionné est différent de l'actuel
            if(lastValue != comboBox->currentIndex())
            {
                // on décoche l'ancien modèle
                comboModel->setPossibilityOfSelectedInModelChecked(false);

                // on sélectionne le nouveau modèle
                comboModel->setSelectedInModel(comboBox->currentIndex());

                // on coche/décoche le nouveau modèle
                comboModel->setPossibilityOfSelectedInModelChecked(isChecked);
            }
        }
    }
}

void CTG_InModelPossibilitiesChoiceComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
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

                // on va parcourir l'arbre pour définir lesquelles sont sélectionnables
                recursiveSetWhereIsSelectable(inGroupModel);

                _itemChangedSlotIsEnabled = true;
            }

            ui->treeView->expandAll();
        }
    }
}

void CTG_InModelPossibilitiesChoice::constructHeader()
{
    QString stepName;

    if(_possibility != NULL)
    {
        stepName = _possibility->outModel()->step()->getStepCustomName() == _possibility->outModel()->step()->getStepName() ? _possibility->outModel()->step()->getStepExtendedName() : _possibility->outModel()->step()->getStepCustomName();
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
    QStandardItem *emptyItem = new QStandardItem("");
    emptyItem->setEditable(false);
    retList.append(emptyItem);

    // un item vide
    emptyItem = new QStandardItem("");
    emptyItem->setEditable(false);
    retList.append(emptyItem);

    recursiveCreateItemsForGroupModel(rootName,
                                      resModel->rootGroup());

    return retList;
}

void CTG_InModelPossibilitiesChoice::recursiveCreateItemsForGroupModel(QStandardItem *root,
                                                                       DEF_CT_AbstractGroupModelOut *group)
{
    QList<QStandardItem*> list;

    // le nom du modèle de sortie
    CTG_InModelPossibilitiesChoiceItem *rootItem = new CTG_InModelPossibilitiesChoiceItem(group->displayableName());
    rootItem->setEditable(false);
    rootItem->_outModel = group;

    list.append(rootItem);

    // un item pour la sélection
    CTG_InModelPossibilitiesChoiceItem *selectableRootItem = new CTG_InModelPossibilitiesChoiceItem("");
    selectableRootItem->setEditable(false);
    selectableRootItem->_outModel = group;
    list.append(selectableRootItem);

    // un item pour la sélection du modèle d'entrée si il y en a plusieurs (combobox)
    CTG_InModelPossibilitiesChoiceItemCombo *comboBoxRootItem = new CTG_InModelPossibilitiesChoiceItemCombo("");
    comboBoxRootItem->setEditable(false);
    comboBoxRootItem->_outModel = group;
    list.append(comboBoxRootItem);

    QListIterator< DEF_CT_AbstractGroupModelOut* > itG(group->groups());

    while(itG.hasNext())
    {
        recursiveCreateItemsForGroupModel(rootItem, itG.next());
    }

    QListIterator<CT_OutAbstractSingularItemModel*> itI(group->items());

    while(itI.hasNext())
    {
        QList<QStandardItem*> list2;

        CT_OutAbstractSingularItemModel *item = itI.next();

        // le nom du modèle de sortie
        CTG_InModelPossibilitiesChoiceItem *itemItem = new CTG_InModelPossibilitiesChoiceItem(item->displayableName());
        itemItem->setEditable(false);
        itemItem->_outModel = item;
        list2.append(itemItem);

        // un item pour la sélection (case à cocher)
        CTG_InModelPossibilitiesChoiceItem *checkableItemItem = new CTG_InModelPossibilitiesChoiceItem("");
        checkableItemItem->setEditable(false);
        checkableItemItem->setEnabled(!m_readOnly);
        checkableItemItem->_outModel = item;
        list2.append(checkableItemItem);

        // un item pour la sélection du modèle d'entrée si il y en a plusieurs (combobox)
        CTG_InModelPossibilitiesChoiceItemCombo *comboBoxItemItem = new CTG_InModelPossibilitiesChoiceItemCombo("");
        comboBoxItemItem->setEditable(false);
        comboBoxItemItem->setEnabled(!m_readOnly);
        comboBoxItemItem->_outModel = item;
        list2.append(comboBoxItemItem);

        rootItem->appendRow(list2);
    }

    root->appendRow(list);
}

void CTG_InModelPossibilitiesChoice::recursiveSetWhereIsSelectable(DEF_CT_AbstractGroupModelIn *inGroupModel)
{
    QList<CT_InStdModelPossibility*> list = inGroupModel->getPossibilitiesSaved();

    if(!list.isEmpty())
        recursiveSearchAndRefreshSelectedItem(_viewModel.invisibleRootItem(),
                                              inGroupModel,
                                              list);

    QListIterator< DEF_CT_AbstractGroupModelIn* > itG(inGroupModel->groups());

    while(itG.hasNext())
        recursiveSetWhereIsSelectable(itG.next());

    QListIterator<CT_InAbstractSingularItemModel*> itI(inGroupModel->items());

    while(itI.hasNext())
    {
        CT_InAbstractSingularItemModel *item = itI.next();

        list = item->getPossibilitiesSaved();

        if(!list.isEmpty())
            recursiveSearchAndRefreshSelectedItem(_viewModel.invisibleRootItem(),
                                                  item,
                                                  list);
    }
}

void CTG_InModelPossibilitiesChoice::recursiveSearchAndRefreshSelectedItem(QStandardItem *root,
                                                                           const CT_InAbstractModel *inModel,
                                                                           const QList<CT_InStdModelPossibility *> &list)
{
    int size = root->rowCount();

    for(int i=0; i<size; ++i)
    {
        CTG_InModelPossibilitiesChoiceItem *checkableItem = dynamic_cast<CTG_InModelPossibilitiesChoiceItem*>(root->child(i, COLUMN_CHECK));

        if(checkableItem != NULL)
        {
            bool continueLoop = true;
            QListIterator<CT_InStdModelPossibility*> it(list);

            while(it.hasNext()
                  && continueLoop)
            {
                CT_InStdModelPossibility *possibility = it.next();

                if(possibility->outModel()->uniqueName() == checkableItem->_outModel->uniqueName())
                {
                    continueLoop = false;

                    checkableItem->setCheckable(true);
                    checkableItem->setEnabled(!m_readOnly && (inModel->choiceMode() != CT_InAbstractModel::C_DontChoose));

                    // il se peut que plusieurs modèles d'entrées on comme possibilité ce modèle de sortie
                    // donc on l'ajoute dans la liste pour pouvoir afficher une QComboBox si il y en a plusieurs
                    // et laisser l'utilisateur choisir
                    if(inModel->choiceMode() != CT_InAbstractModel::C_DontChoose)
                        checkableItem->addInModel((CT_InAbstractModel*)inModel);
                    else
                        checkableItem->addDontChooseInModel((CT_InAbstractModel*)inModel);

                }
            }

            if(!continueLoop)
            {
                CTG_InModelPossibilitiesChoiceItemCombo *comboItem = (CTG_InModelPossibilitiesChoiceItemCombo*)root->child(i, COLUMN_COMBO);
                comboItem->setEditable(checkableItem->isCheckable() && checkableItem->isEnabled());
                comboItem->setInModels(checkableItem->getInModels());
                comboItem->setDontChooseInModels(checkableItem->getDontChooseInModels());
            }
        }

        recursiveSearchAndRefreshSelectedItem(root->child(i, 0),
                                              inModel,
                                              list);
    }
}

void CTG_InModelPossibilitiesChoice::itemChanged(QStandardItem *item)
{
    if(_itemChangedSlotIsEnabled)
    {
        CTG_InModelPossibilitiesChoiceItem *checkableItem = dynamic_cast<CTG_InModelPossibilitiesChoiceItem*>(item);

        // si on a modifié l'état d'une case à cocher
        if(checkableItem != NULL)
        {
            // on récupère son nouvel état
            bool checked = checkableItem->isChecked();

            // on récupère l'item combo
            CTG_InModelPossibilitiesChoiceItemCombo *comboItem = (CTG_InModelPossibilitiesChoiceItemCombo*)item->parent()->child(item->row(), COLUMN_COMBO);

            // et on change l'état de la possibilité du modèle sélectionné
            if(comboItem->setPossibilityOfSelectedInModelChecked(checked))
            {   // si on a pu changer son état

                // si on l'a sélectionné
                if(checked)
                {
                    _itemChangedSlotIsEnabled = false;

                    // sélection des parents seulement si c'est un item (sélectionne les parents qui n'ont pas au moins un modèle déjà sélectionné)
                    if(dynamic_cast<CT_OutAbstractSingularItemModel*>(checkableItem->_outModel) != NULL)
                    {
                        setCheckedParent(item->parent(), true);

                        if(item->parent()->parent() != NULL)
                            recursiveSetCheckedParentWhileModelHasNoPossibilityChecked(item->parent()->parent());
                    }

                    _itemChangedSlotIsEnabled = true;
                }
                else // sinon
                {
                    _itemChangedSlotIsEnabled = false;

                    // on déselectionne ses enfants
                    recursiveSetCheckedChildren(item->parent()->child(item->row(), 0), false);

                    // désélection du parent seulement si c'est un groupe
                    if(dynamic_cast<CT_OutAbstractSingularItemModel*>(checkableItem->_outModel) != NULL)
                        setCheckedParent(item->parent(), false);

                    _itemChangedSlotIsEnabled = true;
                }
            }
        }
    }
}

void CTG_InModelPossibilitiesChoice::recursiveSetCheckedParent(QStandardItem *parent, const bool &checked)
{
    // si on a encore un parent avant ça
    if(parent->parent() != NULL)
    {
        // on récupère l'item de la colonne des items checkables
        CTG_InModelPossibilitiesChoiceItem *checkableItem = dynamic_cast<CTG_InModelPossibilitiesChoiceItem*>(parent->parent()->child(parent->row(), COLUMN_CHECK));

        if(checkableItem != NULL)
        {
            // on récupère l'item combo
            CTG_InModelPossibilitiesChoiceItemCombo *comboItem = (CTG_InModelPossibilitiesChoiceItemCombo*)parent->parent()->child(parent->row(), COLUMN_COMBO);

            // et on change l'état de sa possibilité
            if(comboItem->setPossibilityOfSelectedInModelChecked(checked))
                recursiveSetCheckedParent(parent->parent(), checked); // on continue vers le parent si la modification a réussi
        }
    }
}

void CTG_InModelPossibilitiesChoice::recursiveSetCheckedParentWhileModelHasNoPossibilityChecked(QStandardItem *parent)
{
    // si on a encore un parent avant ça
    if(parent->parent() != NULL)
    {
        // on récupère l'item de la colonne des items checkables
        CTG_InModelPossibilitiesChoiceItem *checkableItem = dynamic_cast<CTG_InModelPossibilitiesChoiceItem*>(parent->parent()->child(parent->row(), COLUMN_CHECK));

        if(checkableItem != NULL)
        {
            // on récupère l'item combo
            CTG_InModelPossibilitiesChoiceItemCombo *comboItem = (CTG_InModelPossibilitiesChoiceItemCombo*)parent->parent()->child(parent->row(), COLUMN_COMBO);

            if((comboItem->getInModelSelected() == NULL)
                    || (comboItem->getInModelSelected()->getPossibilitiesSavedSelected().isEmpty()))
            {
                // et on change l'état de sa possibilité
                if(comboItem->setPossibilityOfSelectedInModelChecked(true))
                    recursiveSetCheckedParentWhileModelHasNoPossibilityChecked(parent->parent()); // on continue vers le parent si la modification a réussi
            }
        }
    }
}

void CTG_InModelPossibilitiesChoice::recursiveSetCheckedChildren(QStandardItem *parent, const bool &checked)
{
    int size = parent->rowCount();

    for(int i=0; i<size; ++i)
    {
        // on récupère l'item de la colonne des items checkables
        CTG_InModelPossibilitiesChoiceItem *checkableItem = dynamic_cast<CTG_InModelPossibilitiesChoiceItem*>(parent->child(i, COLUMN_CHECK));

        if(checkableItem != NULL)
        {
            // on récupère l'item combo
            CTG_InModelPossibilitiesChoiceItemCombo *comboItem = (CTG_InModelPossibilitiesChoiceItemCombo*)parent->child(i, COLUMN_COMBO);

            // et on change l'état de sa possibilité
            if(comboItem->setPossibilityOfSelectedInModelChecked(checked))
                recursiveSetCheckedChildren(parent->child(i, 0), checked); // on continue à partir de l'enfant si la modification a réussi
        }
    }
}

void CTG_InModelPossibilitiesChoice::setCheckedParent(QStandardItem *parent, const bool &checked)
{
    // si on a encore un parent avant ça
    if(parent->parent() != NULL)
    {
        // on récupère l'item de la colonne des items checkables
        CTG_InModelPossibilitiesChoiceItem *checkableItem = dynamic_cast<CTG_InModelPossibilitiesChoiceItem*>(parent->parent()->child(parent->row(), COLUMN_CHECK));

        if(checkableItem != NULL)
        {
            // on récupère l'item combo
            CTG_InModelPossibilitiesChoiceItemCombo *comboItem = (CTG_InModelPossibilitiesChoiceItemCombo*)parent->parent()->child(parent->row(), COLUMN_COMBO);

            // et on change l'état de sa possibilité
            comboItem->setPossibilityOfSelectedInModelChecked(checked);
        }
    }
}
