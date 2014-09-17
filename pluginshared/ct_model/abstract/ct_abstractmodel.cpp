#include "ct_abstractmodel.h"

#include "ct_tools/model/ct_autorenamemodels.h"

CT_AbstractModel::CT_AbstractModel(const QString &uniqueName,
                                   const QString &description,
                                   const QString &displayableName)
{
    m_uName = uniqueName;
    m_uNamePlusTurn = uniqueName;
    m_description = description.isEmpty() ? displayableName : description;
    m_autoRename = NULL;
    m_step = NULL;
    m_parent = NULL;

    setTurnIndex(-1);
    setDisplayableName(displayableName);
}

CT_AbstractModel::~CT_AbstractModel()
{
    if(m_autoRename != NULL)
        m_autoRename->removeModel(this);
}

QString CT_AbstractModel::uniqueName() const
{
    return m_uName;
}

QString CT_AbstractModel::uniqueNamePlusTurn() const
{
    return m_uNamePlusTurn;
}

void CT_AbstractModel::setTurnIndex(int turnIndex)
{
    // ATTENTION NE JAMAIS MODIFIER CES LIGNES
    // Si vous le faite quand même les scripts enregistrés ne pourront plus être utilisés
    // car les modèles d'entrées ne retrouveront plus les modèles de sorties lors du chargement
    // d'un script
    m_turnIndex = turnIndex;

    if(m_turnIndex != -1)
        m_uNamePlusTurn = (uniqueName() + QString().setNum(m_turnIndex));
    else
        m_uNamePlusTurn = uniqueName();
}

int CT_AbstractModel::turnIndex() const
{
    return m_turnIndex;
}

QString CT_AbstractModel::description() const
{
    return m_description;
}

QString CT_AbstractModel::displayableName() const
{
    return m_dName;
}

void CT_AbstractModel::setDisplayableName(const QString &displayableName)
{
    m_dName = (displayableName.isEmpty() ? uniqueName() : displayableName);
}

bool CT_AbstractModel::setUniqueName(const QString &uniqueName)
{
    if(!canSetUniqueName(uniqueName))
        return false;

    m_uName = uniqueName;
    m_uNamePlusTurn = uniqueName;

    setTurnIndex(turnIndex());

    return true;
}

bool CT_AbstractModel::canSetUniqueName(const QString &uniqueName) const
{
    if((parentModel() == NULL) && isEmpty())
        return true;

    return !existInTree(uniqueName);
}

QList<CT_AbstractModel *> CT_AbstractModel::recursiveGetAllModels() const
{
    QList<CT_AbstractModel *> l;
    l.append(const_cast<CT_AbstractModel*>(this));

    QList<CT_AbstractModel *> c = childrens();
    QListIterator<CT_AbstractModel *> it(c);

    while(it.hasNext())
        l.append(it.next()->recursiveGetAllModels());

    return l;
}

CT_AbstractModel* CT_AbstractModel::parentModel() const
{
    return m_parent;
}

CT_AbstractModel* CT_AbstractModel::rootModel() const
{
    CT_AbstractModel* root = const_cast<CT_AbstractModel*>(this);

    while(root->parentModel() != NULL)
        root = root->parentModel();

    return root;
}

bool CT_AbstractModel::existInTree(const QString &uniqueName) const
{
    return internalExistInTree(rootModel(), uniqueName);
}

bool CT_AbstractModel::existModelOrChildrensInTree(CT_AbstractModel *modelToSearch) const
{
    return internalExistModelOrChildrensInTree(rootModel(), modelToSearch);
}

CT_AbstractModel* CT_AbstractModel::findModelInTree(const QString &uniqueName) const
{
    return internalFindModelInTree(rootModel(), uniqueName);
}

CT_VirtualAbstractStep* CT_AbstractModel::step() const
{
    return m_step;
}

void CT_AbstractModel::setParentModel(const CT_AbstractModel *parent)
{
    m_parent = (CT_AbstractModel*)parent;
}

void CT_AbstractModel::staticSetParentModelToModel(CT_AbstractModel *model, const CT_AbstractModel *parent)
{
    if(model != NULL)
        model->setParentModel(parent);
}

bool CT_AbstractModel::internalExistModelOrChildrensInTree(CT_AbstractModel *rootModel,
                                                           CT_AbstractModel *search) const
{
    if(internalExistInTree(rootModel, search->uniqueName()))
        return true;

    QList<CT_AbstractModel*> l = search->childrens();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
    {
        if(internalExistModelOrChildrensInTree(rootModel, it.next()))
            return true;
    }

    return false;
}

bool CT_AbstractModel::internalExistInTree(CT_AbstractModel *parent,
                                           const QString &uniqueName) const
{
    // si ce groupe modèle à le même nom
    if(parent->uniqueName() == uniqueName)
        return true;

    QList<CT_AbstractModel*> l = parent->childrens();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
    {
        CT_AbstractModel *model = it.next();

        // on descend dans l'arbre
        if(internalExistInTree(model, uniqueName))
            return true;
    }

    return false;
}

CT_AbstractModel* CT_AbstractModel::internalFindModelInTree(CT_AbstractModel *parent,
                                                            const QString &uniqueName) const
{
    // si ce groupe modèle à le même nom
    if(parent->uniqueName() == uniqueName)
        return parent;

    QList<CT_AbstractModel*> l = parent->childrens();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
    {
        CT_AbstractModel *model = it.next();

        // on descend dans l'arbre
        if((model = internalFindModelInTree(model, uniqueName)) != NULL)
            return model;
    }

    return NULL;
}

void CT_AbstractModel::setAutoRenameObject(const CT_AutoRenameModels *autoRename)
{
    if((m_autoRename != NULL)
            && (m_autoRename != autoRename))
        m_autoRename->removeModel(this);

    m_autoRename = (CT_AutoRenameModels*)autoRename;
}

void CT_AbstractModel::setStep(const CT_VirtualAbstractStep *step)
{
    m_step = (CT_VirtualAbstractStep*)step;

    QList<CT_AbstractModel*> l = childrens();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
        it.next()->setStep(step);
}
