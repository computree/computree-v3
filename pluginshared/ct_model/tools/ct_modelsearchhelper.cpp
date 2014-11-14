#include "ct_modelsearchhelper.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/inModel/abstract/ct_inabstractmodel.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"

CT_ModelSearchHelper::CT_ModelSearchHelper()
{
}

CT_ModelSearchHelper::~CT_ModelSearchHelper()
{
    clearCache();
}

CT_OutAbstractModel* CT_ModelSearchHelper::searchModelForCreation(const QString &outModelUniqueName,
                                                                  const CT_AbstractResult *outResult)
{
    QHash<QString, CT_AbstractModel*> *outHash = NULL;

    CT_OutAbstractModel *model = (CT_OutAbstractModel*)getInCache(outResult->model(), outModelUniqueName, &outHash);

    if(model == NULL)
    {
        model = outResult->parentStep()->getOutModelForCreation((CT_ResultGroup*)outResult, outModelUniqueName);

        if(model != NULL)
            outHash->insert(outModelUniqueName, model);
    }

    return model;
}

CT_AbstractModel* CT_ModelSearchHelper::searchModel(const QString &inOrOutModelUniqueName,
                                                    const CT_AbstractResult *inOrOutResult,
                                                    const CT_VirtualAbstractStep *yourStep)
{
    QHash<QString, CT_AbstractModel*> *outHash = NULL;

    CT_AbstractModel *model = getInCache(inOrOutResult->model(), inOrOutModelUniqueName, &outHash);

    if(model == NULL)
    {
        // if the result passed in parameter is a OUT result
        if(inOrOutResult->parentStep() == yourStep)
        {
            // check if the result is a copy
            CT_OutResultModelGroupCopy *outCopyModel = dynamic_cast<CT_OutResultModelGroupCopy*>(inOrOutResult->model());

            // if yes
            if(outCopyModel != NULL)
            {
                // Two possibility : search a OUT model or search a IN model

                // Search first the out model
                model = yourStep->getOutModelForCreation((CT_ResultGroup*)inOrOutResult, inOrOutModelUniqueName);

                // if founded we must check if it was a new model created in this step
                if((model != NULL) && (dynamic_cast<CT_OutAbstractModel*>(model)->lastOriginalModelWithAStep()->step() != yourStep)) // if no
                    model = NULL; // seach a in model

                // we search a in model if a out model was not found
                if(model == NULL)
                    model = yourStep->getInModelForResearch(outCopyModel->outResultModelCopy()->outModelForSearch(), inOrOutModelUniqueName);
            }
            // if no
            else
            {
                // we want to seach a model for creation
                model = inOrOutResult->parentStep()->getOutModelForCreation((CT_ResultGroup*)inOrOutResult, inOrOutModelUniqueName);
            }
        }
        // if the result passed in parameter in a IN result
        else
        {
            // so we want to search a IN model
            model = yourStep->getInModelForResearch(inOrOutResult->model(), inOrOutModelUniqueName);
        }

        if(model != NULL)
            outHash->insert(inOrOutModelUniqueName, model);
    }

    return model;
}

CT_AbstractModel *CT_ModelSearchHelper::searchModel(const QString &inOrOutModelUniqueName,
                                                    const CT_OutAbstractResultModel *inOrOutResultModel,
                                                    const CT_VirtualAbstractStep *yourStep)
{
    CT_AbstractModel *model = NULL;

    QHash<QString, CT_AbstractModel*> *outHash = NULL;

    if(inOrOutResultModel->result() == NULL)
    {
        model = getInCache(inOrOutResultModel, inOrOutModelUniqueName, &outHash);

        if(model == NULL)
            model = yourStep->getInModelForResearch(inOrOutResultModel, inOrOutModelUniqueName);
    }
    else
    {
        return searchModel(inOrOutModelUniqueName, inOrOutResultModel->result(), yourStep);
    }

    if(model != NULL)
        outHash->insert(inOrOutModelUniqueName, model);

    return model;
}

void CT_ModelSearchHelper::removeCacheForResult(const CT_AbstractResult *res)
{
    removeCacheForResultModel(res->model());
}

void CT_ModelSearchHelper::removeCacheForResultModel(const CT_OutAbstractResultModel *resM)
{
    QHash<QString, CT_AbstractModel*> *hash = m_cache.take((CT_OutAbstractResultModel*)resM);
    delete hash;
}

void CT_ModelSearchHelper::clearCache()
{
    qDeleteAll(m_cache.begin(), m_cache.end());
    m_cache.clear();
}

CT_AbstractModel* CT_ModelSearchHelper::getInCache(const CT_OutAbstractResultModel *resultModel,
                                                   const QString &modelUniqueName,
                                                   QHash<QString, CT_AbstractModel*> **outHash)
{
    QHash<QString, CT_AbstractModel*> *hash = m_cache.value((CT_OutAbstractResultModel*)resultModel, NULL);

    if(hash == NULL)
    {
        hash = new QHash<QString, CT_AbstractModel*>();
        m_cache.insert((CT_OutAbstractResultModel*)resultModel, hash);

        connect(resultModel, SIGNAL(destroyed(QObject*)), this, SLOT(resultModelDeleted(QObject*)), Qt::DirectConnection);

        *outHash = hash;
        return NULL;
    }

    *outHash = hash;

    return hash->value(modelUniqueName, NULL);
}

void CT_ModelSearchHelper::resultModelDeleted(QObject *o)
{
    removeCacheForResultModel((CT_OutAbstractResultModel*)o);
}
