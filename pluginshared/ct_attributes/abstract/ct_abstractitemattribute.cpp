#include "ct_abstractitemattribute.h"

#include "ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h"

#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"
#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

CT_AbstractItemAttribute::CT_AbstractItemAttribute() : CT_AbstractItem()
{
    m_category = NULL;
}

CT_AbstractItemAttribute::CT_AbstractItemAttribute(const CT_OutAbstractItemAttributeModel *model,
                                                   const CT_AbstractCategory *category,
                                                   const CT_AbstractResult *result) : CT_AbstractItem(model, result)
{
    Q_ASSERT_X(category != NULL, "CT_AbstractItemAttribute constructor", "When you create a ItemAttribute the category must not be null !");
    m_category = (CT_AbstractCategory*)category;
}

CT_AbstractItemAttribute::CT_AbstractItemAttribute(const QString &modelName,
                                                   const QString &categoryName,
                                                   const CT_AbstractResult *result) : CT_AbstractItem(modelName, result)
{
    m_category = (CT_AbstractCategory*)PS_CATEGORY_MANAGER->findByUniqueName(categoryName);
    Q_ASSERT_X(m_category != NULL, "CT_AbstractItemAttribute constructor", "You created a ItemAttribute with a categoryName but the category was not found");
}

CT_AbstractItemAttribute::CT_AbstractItemAttribute(const QString &categoryName) : CT_AbstractItem(NULL, NULL)
{
    m_category = (CT_AbstractCategory*)PS_CATEGORY_MANAGER->findByUniqueName(categoryName);
    Q_ASSERT_X(m_category != NULL, "CT_AbstractItemAttribute constructor", "You created a ItemAttribute with a categoryName but the category was not found");
}

CT_AbstractItemAttribute::~CT_AbstractItemAttribute()
{
}

bool CT_AbstractItemAttribute::isValid() const
{
    return type() != CT_AbstractCategory::UNKNOWN;
}

QString CT_AbstractItemAttribute::displayableName() const
{
    if(model() != NULL)
        return model()->displayableName();
    else if(category() != NULL)
        return category()->displayableName();

    return QString();
}

CT_OutAbstractItemAttributeModel* CT_AbstractItemAttribute::model() const
{
    return (CT_OutAbstractItemAttributeModel*)abstractModel();
}

CT_AbstractCategory* CT_AbstractItemAttribute::category() const
{
    return m_category;
}

void CT_AbstractItemAttribute::setModel(const CT_OutAbstractItemAttributeModel *model)
{
    internalSetModel(model);
}

void CT_AbstractItemAttribute::changeResult(const CT_AbstractResult *newRes)
{
    internalSetResult(newRes);
}
