#include "ct_abstractitemattribute.h"

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"
#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

CT_AbstractItemAttribute::CT_AbstractItemAttribute(const CT_OutAbstractItemAttributeModel *model,
                                                   const CT_AbstractCategory *category,
                                                   const CT_AbstractResult *result)
{
    Q_ASSERT_X(category != NULL, "CT_AbstractItemAttribute constructor", "When you create a Attribute the category must not be null !");

    m_result = (CT_AbstractResult*)result;
    m_category = (CT_AbstractCategory*)category;

    setModel(model);
}

CT_AbstractItemAttribute::CT_AbstractItemAttribute(const QString &modelName,
                                                   const QString &categoryName,
                                                   const CT_AbstractResult *result)
{
    m_category = (CT_AbstractCategory*)PS_CATEGORY_MANAGER->findByUniqueName(categoryName);

    Q_ASSERT_X(m_category != NULL, "CT_AbstractItemAttribute constructor", "You created a Attribute with a categoryName but the category was not found");

    Q_ASSERT_X(!modelName.isEmpty(), "CT_AbstractItemAttribute constructor", "When you create a Attribute the modelName must not be empty !");
    Q_ASSERT_X((result != NULL), "CT_AbstractItemAttribute constructor", "When you create a Attribute with a modelName the result must not be NULL !");
    Q_ASSERT_X((result->parentStep() != NULL), "CT_AbstractItemAttribute constructor", "When you create a Attribute with a modelName the result must know its parent step");

    CT_OutAbstractItemAttributeModel *model = dynamic_cast<CT_OutAbstractItemAttributeModel*>(result->parentStep()->getOutModelForCreation((CT_ResultGroup*)result, modelName));

    Q_ASSERT_X(model != NULL, "CT_AbstractItemAttribute constructor", "You created a Attribute with a modelName but the model was not found");

    m_result = (CT_AbstractResult*)result;
    setModel(model);
}

CT_AbstractItemAttribute::~CT_AbstractItemAttribute()
{
}

bool CT_AbstractItemAttribute::isValid() const
{
    return type() != CT_AbstractCategory::UNKNOWN;
}

CT_AbstractCategory::ValueType CT_AbstractItemAttribute::type() const
{
    return m_category->valueType();
}

CT_OutAbstractItemAttributeModel* CT_AbstractItemAttribute::model() const
{
    return m_model;
}

CT_AbstractResult* CT_AbstractItemAttribute::result() const
{
    return m_result;
}

CT_AbstractCategory* CT_AbstractItemAttribute::category() const
{
    return m_category;
}

void CT_AbstractItemAttribute::setModel(const CT_OutAbstractItemAttributeModel *model)
{
    m_model = (CT_OutAbstractItemAttributeModel*)model;

    if(m_model != NULL)
        m_model->setResult(result());
}
