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

    CT_OutAbstractModel *model = (CT_OutAbstractModel*)getInCache(outResult, outModelUniqueName, &outHash);

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

    CT_AbstractModel *model = getInCache(inOrOutResult, inOrOutModelUniqueName, &outHash);

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
                model = inOrOutResult->parentStep()->getOutModelForCreation((CT_ResultGroup*)inOrOutResult, inOrOutModelUniqueName);

                // This model contains the possibility wish was used for create the out result (a copy of the in result). Here we want to
                //
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

void CT_ModelSearchHelper::removeCacheForResult(const CT_AbstractResult *res)
{
    QHash<QString, CT_AbstractModel*> *hash = m_cache.take((CT_AbstractResult*)res);
    delete hash;
}

void CT_ModelSearchHelper::clearCache()
{
    qDeleteAll(m_cache.begin(), m_cache.end());
    m_cache.clear();
}

CT_AbstractModel* CT_ModelSearchHelper::getInCache(const CT_AbstractResult *result,
                                                   const QString &modelUniqueName,
                                                   QHash<QString, CT_AbstractModel*> **outHash)
{
    QHash<QString, CT_AbstractModel*> *hash = m_cache.value((CT_AbstractResult*)result, NULL);

    if(hash == NULL)
    {
        hash = new QHash<QString, CT_AbstractModel*>();
        m_cache.insert((CT_AbstractResult*)result, hash);

        connect(result, SIGNAL(destroyed(QObject*)), this, SLOT(resultDeleted(QObject*)), Qt::DirectConnection);

        *outHash = hash;
        return NULL;
    }

    *outHash = hash;

    return hash->value(modelUniqueName, NULL);
}

void CT_ModelSearchHelper::resultDeleted(QObject *o)
{
    removeCacheForResult((CT_AbstractResult*)o);
}
