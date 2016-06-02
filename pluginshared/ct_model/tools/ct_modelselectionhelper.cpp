#include "ct_modelselectionhelper.h"

#include "ct_result/model/inModel/abstract/ct_inabstractresultmodel.h"
#include "ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/outModel/abstract/ct_outabstractmodel.h"

#include <QDebug>

CT_ModelSelectionHelper::CT_ModelSelectionHelper(const CT_InAbstractResultModel *inResultModel)
{
    Q_ASSERT(inResultModel != NULL);

    m_rootInResultModel = (CT_InAbstractResultModel*)inResultModel;

    QList<CT_InStdModelPossibility*> p = m_rootInResultModel->getPossibilitiesSaved();
    QListIterator<CT_InStdModelPossibility*> itP(p);

    while(itP.hasNext()) {
        CT_InStdResultModelPossibility *poss = (CT_InStdResultModelPossibility*)itP.next();

        CT_InModelGraph *graph = new CT_InModelGraph();
        graph->constructStructure(m_rootInResultModel, poss);

        QList<CT_InModelGraph*> validGraphs;
        validGraphs << graph;

        recursiveConstructGraphs(validGraphs, poss->inModel());

        QMutableListIterator<CT_InModelGraph*> it(validGraphs);

        while(it.hasNext()) {
            CT_InModelGraph *g = it.next();

            if(g->isValid()) {
                m_validGraphsByRootPossibility.insert(poss, g);
                it.remove();
            }
        }

        qDeleteAll(validGraphs.begin(), validGraphs.end());
    }

    Q_ASSERT(isValid());
}

CT_ModelSelectionHelper::~CT_ModelSelectionHelper()
{
    qDeleteAll(m_validGraphsByRootPossibility.begin(), m_validGraphsByRootPossibility.end());
}

bool CT_ModelSelectionHelper::isValid() const
{
    // if the number of possibilities for the result is less than the minimum that must be selected
    if((m_validGraphsByRootPossibility.uniqueKeys().size() < m_rootInResultModel->minimumNumberOfPossibilityThatMustBeSelectedForOneTurn())
            && (m_rootInResultModel->choiceMode() != CT_InAbstractModel::C_DontChoose))
        return false; // not valid !!

    return true;
}

bool CT_ModelSelectionHelper::canSelectAllPossibilitiesByDefault() const
{
    if(!isValid())
        return false;

    // we get only unique keys ! a key can exist multi times : it would like to say that there
    // are several choices for a possibility.
    QList<CT_InStdModelPossibility*> keys = m_validGraphsByRootPossibility.uniqueKeys();
    int size = keys.size();

    int maxNumberThatCanBeSelected = m_rootInResultModel->maximumNumberOfPossibilityThatCanBeSelectedForOneTurn();

    // if there are more possibilities than the maximum number that can be selected
    if(size > maxNumberThatCanBeSelected)
        return false; // we can not select by default all possibilities, the user must do a choice

    bool isOptionnal = (m_rootInResultModel->minimumNumberOfPossibilityThatMustBeSelectedForOneTurn() == 0);

    // if the result is optionnal and the result has at least one possibility
    if(isOptionnal
            && (size > 0))
        return false; // we can not select by default all possibilities, the user must do a choice

    // if there was several choice for one possibility
    if((size > 0) && (m_validGraphsByRootPossibility.keys().size() > size))
        return false; // we can not select by default all possibilities, the user must do a choice

    // if one model is optionnal in the structure we can not choose it by default, the user must do a choice
    if(size > 0)
        return !m_validGraphsByRootPossibility.value(keys.first(), NULL)->hasOneModelOptional();

    return true;
}

bool CT_ModelSelectionHelper::selectAllPossibilitiesByDefault()
{
    if(!canSelectAllPossibilitiesByDefault())
        return false;

    return selectAtLeastOnePossibility();
}

bool CT_ModelSelectionHelper::canSelectAtLeastOnePossibility() const
{
    bool isOptionnal = (m_rootInResultModel->minimumNumberOfPossibilityThatMustBeSelectedForOneTurn() == 0);

    return !m_validGraphsByRootPossibility.isEmpty() || isOptionnal;
}

bool CT_ModelSelectionHelper::selectAtLeastOnePossibility()
{
    if(!canSelectAtLeastOnePossibility())
        return false;

    // we get only unique keys ! a key can exist multi times : it would like to say that there
    // are several choices for a possibility.
    QList<CT_InStdModelPossibility*> keys = m_validGraphsByRootPossibility.uniqueKeys();
    QListIterator<CT_InStdModelPossibility*> it(keys);

    // We select only one choice for a possibility
    while(it.hasNext())
        m_validGraphsByRootPossibility.value(it.next(), NULL)->select();

    return true;
}

bool CT_ModelSelectionHelper::canSelectOneGraphForPossibilityOfRootModel(const CT_InStdModelPossibility *possibility, qint8 index) const
{
    if(index < 0)
        return !m_validGraphsByRootPossibility.values((CT_InStdModelPossibility*)possibility).isEmpty();

    return index < m_validGraphsByRootPossibility.values((CT_InStdModelPossibility*)possibility).size();
}

bool CT_ModelSelectionHelper::selectOneGraphForPossibilityOfRootModel(const CT_InStdModelPossibility *possibility, qint8 index) const
{
    if(!canSelectOneGraphForPossibilityOfRootModel(possibility, index))
        return false;

    CT_InModelGraph *graph = NULL;

    if(index < 0)
        graph = getOneOfTheBestGraphForPossibilityOfRootModel(possibility);
    else
        graph = m_validGraphsByRootPossibility.values((CT_InStdModelPossibility*)possibility).at(index);

    Q_ASSERT(graph != NULL);

    graph->select();

    return true;
}

int CT_ModelSelectionHelper::nUniqueGraph() const
{
    return m_validGraphsByRootPossibility.uniqueKeys().size();
}

QList<CT_InStdModelPossibility *> CT_ModelSelectionHelper::uniqueGraphPossibilities() const
{
    return m_validGraphsByRootPossibility.uniqueKeys();
}

int CT_ModelSelectionHelper::nGraphForPossibilityOfRootModel(const CT_InStdModelPossibility *possibility) const
{
    return m_validGraphsByRootPossibility.values((CT_InStdModelPossibility*)possibility).size();
}

void CT_ModelSelectionHelper::debugPrintGraphs()
{
    int i = 0;

    QHashIterator<CT_InStdModelPossibility*, CT_InModelGraph*> it(m_validGraphsByRootPossibility);

    while(it.hasNext()) {
        qDebug() << "--------- Graph " << i << " -----------\r\n";
        it.next().value()->debugPrint();
        qDebug() << "---------------------------------------\r\n";
        ++i;
    }
}

void CT_ModelSelectionHelper::recursiveConstructGraphs(QList<CT_InModelGraph *> &graphsToModify, const CT_InAbstractModel *inModel)
{
    QList<CT_InModelGraph*> graphsOfCurrentLevel;

    QList<CT_InStdModelPossibility*> p = inModel->getPossibilitiesSaved();

    if(!p.isEmpty()) {
        QListIterator<CT_InStdModelPossibility*> it(p);

        int nCloned = 0;

        QList<CT_InModelGraph*> nOptionnalGraphFounded;

        const int nMaxToClone = 7;

        // maximum 6 possibilities (to be fastest) !
        while(it.hasNext() && (nCloned < nMaxToClone)) {
            CT_InStdModelPossibility *poss = it.next();

            QListIterator<CT_InModelGraph*> itG(graphsToModify);

            while(itG.hasNext()) {
                CT_InModelGraph *graph = itG.next();

                if(graph->canSetPossibilityForModel(poss, inModel)) {
                    CT_InModelGraph *newGraph = graph->clone();
                    newGraph->setPossibilityForModel(poss, inModel);
                    graphsOfCurrentLevel.append(newGraph);
                    ++nCloned;
                } else if(inModel->finderMode() == CT_InAbstractModel::F_IsOptional) {
                    nOptionnalGraphFounded.append(graph);
                }
            }
        }

        while((nCloned < nMaxToClone) && !nOptionnalGraphFounded.isEmpty()) {
            CT_InModelGraph *newGraph = nOptionnalGraphFounded.takeFirst()->clone();
            graphsOfCurrentLevel.append(newGraph);
            ++nCloned;
        }

        qDeleteAll(graphsToModify.begin(), graphsToModify.end());
        graphsToModify = graphsOfCurrentLevel;
    }

    QList<CT_InAbstractModel*> childrens = inModel->childrensStaticCast<CT_InAbstractModel>();
    QListIterator<CT_InAbstractModel*> itC(childrens);

    while(itC.hasNext())
        recursiveConstructGraphs(graphsToModify, itC.next());
}

CT_ModelSelectionHelper::CT_InModelGraph *CT_ModelSelectionHelper::getOneOfTheBestGraphForPossibilityOfRootModel(const CT_InStdModelPossibility *possibility) const
{
    CT_ModelSelectionHelper::CT_InModelGraph *bestGraph = NULL;

    QList<CT_InModelGraph*> values = m_validGraphsByRootPossibility.values((CT_InStdModelPossibility*)possibility);

    if(!values.isEmpty()) {
        bestGraph = values.first();
        int bestScore = bestGraph->howManyPossibilitiesIsNotNullInStructure();

        QListIterator<CT_InModelGraph*> it(values);
        it.next();

        while(it.hasNext()) {
            CT_InModelGraph *g = it.next();
            int score = g->howManyPossibilitiesIsNotNullInStructure();

            if(score > bestScore) {
                bestScore = score;
                bestGraph = g;
            }
        }
    }

    return bestGraph;
}


CT_ModelSelectionHelper::CT_InModelGraph::CT_InModelGraph()
{
    m_rootNode = NULL;
}

CT_ModelSelectionHelper::CT_InModelGraph::~CT_InModelGraph()
{
    delete m_rootNode;
}

void CT_ModelSelectionHelper::CT_InModelGraph::constructStructure(const CT_InAbstractResultModel *inResultModel, const CT_InStdResultModelPossibility *possibility)
{
    delete m_rootNode;
    m_rootNode = new CT_InModelGraphNode(inResultModel);
    m_rootNode->m_possibility = (CT_InStdResultModelPossibility*)possibility;

    // root model
    staticRecursiveConstructStructure(possibility->inModel(), m_rootNode);
}

bool CT_ModelSelectionHelper::CT_InModelGraph::isValid() const
{
    return staticRecursiveIsValid(m_rootNode);
}

bool CT_ModelSelectionHelper::CT_InModelGraph::canSetPossibilityForModel(const CT_InStdModelPossibility *possibility, const CT_InAbstractModel *model) const
{
    CT_InModelGraphNode *node = staticRecursiveSearchNodeOfModel(m_rootNode, model);

    Q_ASSERT(node != NULL);

    // if it is the root node or a children of the root node we always set the possibility.
    // explanation : 1. the root node (result) don't have a parent so the possibility can always be set.
    //               2. a child of the root node (a group of the result) has no parent too (the result model is not the parent) so the possibility can always be set.
    if((node == m_rootNode) || m_rootNode->m_childrensNode.contains(node))
        return true;

    // get the parent model of the out model of the possibility to set
    CT_OutAbstractModel *parentOutModel = (CT_OutAbstractModel*)possibility->outModel()->parentModel();

    // if it was NULL -> it is a result model
    // if the parent was NULL -> it is a root group
    if((parentOutModel == NULL) || (parentOutModel->parentModel() == NULL))
        return true;

    CT_InAbstractModel *parentModel = static_cast<CT_InAbstractModel*>(model->parentModel());

    Q_ASSERT(parentModel != NULL);

    CT_InModelGraphNode *parentNode = staticRecursiveSearchNodeOfModel(m_rootNode, parentModel);

    Q_ASSERT(parentNode != NULL);

    if((parentNode->m_possibility == NULL) && (parentNode->m_model->choiceMode() == CT_InAbstractModel::C_DontChoose))
        return true;

    bool isOptionnal = (parentNode->m_model->finderMode() == CT_InAbstractModel::F_IsOptional);

    if(!isOptionnal || (parentNode->m_possibility != NULL)) {
        Q_ASSERT(parentNode->m_possibility != NULL);

        if(parentNode->m_possibility->outModel() == possibility->outModel()->parentModel()) {

            CT_InModelGraphNode *nodeP = staticRecursiveSearchNodeOfOutModel(m_rootNode, possibility->outModel());

            // the out model of the possibility must not be already used by another model !
            return (nodeP == NULL);
        }
    }

    return false;
}

bool CT_ModelSelectionHelper::CT_InModelGraph::setPossibilityForModel(const CT_InStdModelPossibility *possibility, const CT_InAbstractModel *model)
{
    if(!canSetPossibilityForModel(possibility, model))
        return false;

    CT_InModelGraphNode *node = staticRecursiveSearchNodeOfModel(m_rootNode, model);

    node->m_possibility = (CT_InStdModelPossibility*)possibility;

    return true;
}

CT_InStdModelPossibility* CT_ModelSelectionHelper::CT_InModelGraph::possibilityForModel(const CT_InAbstractModel *model) const
{
    CT_InModelGraphNode *node = staticRecursiveSearchNodeOfModel(m_rootNode, model);
    Q_ASSERT(node != NULL);

    return node->m_possibility;
}

bool CT_ModelSelectionHelper::CT_InModelGraph::hasOneModelOptional() const
{
    return staticRecursiveIsOneModelOptional(m_rootNode);
}

void CT_ModelSelectionHelper::CT_InModelGraph::select()
{
    staticRecursiveSelectPossibility(m_rootNode, true);
}

void CT_ModelSelectionHelper::CT_InModelGraph::clearSelection()
{
    staticRecursiveSelectPossibility(m_rootNode, false);
}

CT_ModelSelectionHelper::CT_InModelGraph *CT_ModelSelectionHelper::CT_InModelGraph::clone() const
{
    CT_ModelSelectionHelper::CT_InModelGraph *graph = new CT_ModelSelectionHelper::CT_InModelGraph();

    if(m_rootNode == NULL)
        return graph;

    graph->m_rootNode = m_rootNode->cloneAlone();

    staticRecursiveClone(m_rootNode, graph->m_rootNode);

    return graph;
}

int CT_ModelSelectionHelper::CT_InModelGraph::howManyPossibilitiesIsNotNullInStructure() const
{
    return staticRecursiveCountPossibilityNotNULL(m_rootNode);
}

void CT_ModelSelectionHelper::CT_InModelGraph::debugPrint()
{
    staticRecursiveDebugPrintGraph(m_rootNode);
}

void CT_ModelSelectionHelper::CT_InModelGraph::staticRecursiveConstructStructure(const CT_InAbstractModel *inModel, CT_ModelSelectionHelper::CT_InModelGraphNode *parentNode)
{
    CT_InModelGraphNode *node = new CT_InModelGraphNode(inModel);
    parentNode->m_childrensNode.append(node);

    QList<CT_InAbstractModel*> childrens = inModel->childrensStaticCast<CT_InAbstractModel>();
    QListIterator<CT_InAbstractModel*> itC(childrens);

    while(itC.hasNext())
        staticRecursiveConstructStructure(itC.next(), node);
}

bool CT_ModelSelectionHelper::CT_InModelGraph::staticRecursiveIsValid(const CT_ModelSelectionHelper::CT_InModelGraphNode *node)
{
    if(node == NULL)
        return true;

    Q_ASSERT(node->m_model != NULL);

    if((node->m_possibility == NULL) && (node->m_model->finderMode() == CT_InAbstractModel::F_IsOptional))
        return true;
    else if((node->m_possibility == NULL) && (node->m_model->choiceMode() != CT_InAbstractModel::C_DontChoose))
        return false;

    QListIterator<CT_InModelGraphNode*> it(node->m_childrensNode);

    while(it.hasNext()) {
        if(!staticRecursiveIsValid(it.next()))
            return false;
    }

    return true;
}

CT_ModelSelectionHelper::CT_InModelGraphNode* CT_ModelSelectionHelper::CT_InModelGraph::staticRecursiveSearchNodeOfModel(const CT_ModelSelectionHelper::CT_InModelGraphNode *node, const CT_InAbstractModel *model)
{
    if(node == NULL)
        return NULL;

    if(node->m_model == model)
        return (CT_ModelSelectionHelper::CT_InModelGraphNode*)node;

    QListIterator<CT_InModelGraphNode*> it(node->m_childrensNode);

    while(it.hasNext()) {
        CT_InModelGraphNode *foundedNode;

        if((foundedNode = staticRecursiveSearchNodeOfModel(it.next(), model)) != NULL)
            return foundedNode;
    }

    return NULL;
}

CT_ModelSelectionHelper::CT_InModelGraphNode *CT_ModelSelectionHelper::CT_InModelGraph::staticRecursiveSearchNodeOfOutModel(const CT_ModelSelectionHelper::CT_InModelGraphNode *node, const CT_OutAbstractModel *outModel)
{
    if(node == NULL)
        return NULL;

    if((node->m_possibility != NULL) && (node->m_possibility->outModel() == outModel))
        return (CT_ModelSelectionHelper::CT_InModelGraphNode*)node;

    QListIterator<CT_InModelGraphNode*> it(node->m_childrensNode);

    while(it.hasNext()) {
        CT_InModelGraphNode *foundedNode;

        if((foundedNode = staticRecursiveSearchNodeOfOutModel(it.next(), outModel)) != NULL)
            return foundedNode;
    }

    return NULL;
}

void CT_ModelSelectionHelper::CT_InModelGraph::staticRecursiveSelectPossibility(CT_ModelSelectionHelper::CT_InModelGraphNode *node, bool select)
{
    if(node == NULL)
        return;

    if(node->m_possibility != NULL)
        node->m_possibility->setSelected(select);

    QListIterator<CT_InModelGraphNode*> it(node->m_childrensNode);

    while(it.hasNext())
        staticRecursiveSelectPossibility(it.next(), select);
}

void CT_ModelSelectionHelper::CT_InModelGraph::staticRecursiveClone(CT_ModelSelectionHelper::CT_InModelGraphNode *myNode, CT_ModelSelectionHelper::CT_InModelGraphNode *cloneNode)
{
    QListIterator<CT_InModelGraphNode*> it(myNode->m_childrensNode);

    while(it.hasNext()) {
        CT_InModelGraphNode *myCNode = it.next();
        CT_InModelGraphNode *cloneCNode = myCNode->cloneAlone();

        cloneNode->m_childrensNode.append(cloneCNode);

        staticRecursiveClone(myCNode, cloneCNode);
    }
}

bool CT_ModelSelectionHelper::CT_InModelGraph::staticRecursiveIsOneModelOptional(CT_ModelSelectionHelper::CT_InModelGraphNode *node)
{
    if(node == NULL)
        return false;

    Q_ASSERT(node->m_model != NULL);

    if(node->m_model->finderMode() == CT_InAbstractModel::F_IsOptional)
        return true;

    QListIterator<CT_InModelGraphNode*> it(node->m_childrensNode);

    while(it.hasNext()) {
        if(staticRecursiveIsOneModelOptional(it.next()))
            return true;
    }

    return false;
}

int CT_ModelSelectionHelper::CT_InModelGraph::staticRecursiveCountPossibilityNotNULL(CT_ModelSelectionHelper::CT_InModelGraphNode *node)
{
    if(node == NULL)
        return 0;

    int count = 0;

    if(node->m_possibility != NULL)
        count = 1;

    QListIterator<CT_InModelGraphNode*> it(node->m_childrensNode);

    while(it.hasNext())
        count += staticRecursiveCountPossibilityNotNULL(it.next());

    return count;
}

void CT_ModelSelectionHelper::CT_InModelGraph::staticRecursiveDebugPrintGraph(CT_ModelSelectionHelper::CT_InModelGraphNode *node, QString preString)
{
    QString preStringNext = preString;

    if(node->m_possibility != NULL) {
        qDebug() << preString << node->m_possibility->outModel()->displayableName() << "(" << node->m_model->displayableName() << ")";
        preStringNext += "\t";
    }

    QListIterator<CT_InModelGraphNode*> it(node->m_childrensNode);

    while(it.hasNext())
        staticRecursiveDebugPrintGraph(it.next(), preStringNext);
}
