#include "ct_abstractitem.h"

#include "ct_model/outModel/abstract/ct_outabstractmodel.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_global/ct_context.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

CT_AbstractItem::CT_AbstractItem()
{
    m_model = NULL;
    m_result = NULL;
}

CT_AbstractItem::CT_AbstractItem(const CT_OutAbstractModel *model, const CT_AbstractResult *result)
{
    m_model = NULL;
    m_result = NULL;

    internalSetResult(result);
    internalSetModel(model);
}

CT_AbstractItem::CT_AbstractItem(const QString &modelUniqueName, const CT_AbstractResult *result)
{
    m_model = NULL;
    m_result = NULL;

    Q_ASSERT_X(!modelUniqueName.isEmpty(), "CT_AbstractItem constructor", "When you create a Element the modelName must not be empty !");
    Q_ASSERT_X((result != NULL), "CT_AbstractItem constructor", "When you create a Element with a modelName the result must not be NULL !");
    Q_ASSERT_X((result->parentStep() != NULL), "CT_AbstractItem constructor", "When you create a Element with a modelName the result must know its parent step");

    CT_OutAbstractModel *model = PS_MODELS->searchModelForCreation(modelUniqueName, result);

    Q_ASSERT_X(model != NULL, "CT_AbstractItem constructor", "You created a CT_AbstractItem with a modelName but the model was not found");

    internalSetResult(result);
    internalSetModel(model);
}

CT_AbstractItem::~CT_AbstractItem()
{
}

CT_OutAbstractModel* CT_AbstractItem::abstractModel() const
{
    return m_model;
}

CT_AbstractResult* CT_AbstractItem::result() const
{
    return m_result;
}

QList<CT_AbstractItem *> CT_AbstractItem::childrensForGui() const
{
    return QList<CT_AbstractItem *>();
}

void CT_AbstractItem::staticInternalSetWillBeRemoveFromResult(CT_AbstractItem *item, const CT_AbstractResult *res)
{
    item->internalSetWillBeRemovedFromResult(res);
}

void CT_AbstractItem::internalSetResult(const CT_AbstractResult *newRes)
{
    m_result = (CT_AbstractResult*)newRes;

    if(m_model != NULL)
        m_model->setResult(newRes);
}

void CT_AbstractItem::internalSetModel(const CT_OutAbstractModel *model)
{
    QString errorModel = internalVerifyModel(model);

    Q_ASSERT_X(errorModel.isEmpty(), "CT_AbstractItem internalSetModel", qPrintable(errorModel));

    m_model = (CT_OutAbstractModel*)model;

    if(m_model != NULL)
        m_model->setResult(result());
}

QString CT_AbstractItem::internalVerifyModel(const CT_OutAbstractModel *model) const
{
    Q_UNUSED(model)

    return QString();
}
