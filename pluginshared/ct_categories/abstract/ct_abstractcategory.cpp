#include "ct_abstractcategory.h"

#include "math.h"

#include <QMetaEnum>

const QString CT_AbstractCategory::DATA_VALUE                   = "CT_VALUE";
const QString CT_AbstractCategory::DATA_ID                      = "CT_ID";
const QString CT_AbstractCategory::DATA_DISPLAYABLE_NAME        = "CT_DISPLAYABLE_NAME";
const QString CT_AbstractCategory::DATA_COORDINATE              = "CT_COORDINATE";
const QString CT_AbstractCategory::DATA_X                       = "CT_X";
const QString CT_AbstractCategory::DATA_Y                       = "CT_Y";
const QString CT_AbstractCategory::DATA_Z                       = "CT_Z";
const QString CT_AbstractCategory::DATA_CX                      = "CT_CX";
const QString CT_AbstractCategory::DATA_CY                      = "CT_CY";
const QString CT_AbstractCategory::DATA_CZ                      = "CT_CZ";
const QString CT_AbstractCategory::DATA_DIRECTION               = "CT_DIRECTION";
const QString CT_AbstractCategory::DATA_X_DIRECTION             = "CT_X_DIRECTION";
const QString CT_AbstractCategory::DATA_Y_DIRECTION             = "CT_Y_DIRECTION";
const QString CT_AbstractCategory::DATA_Z_DIRECTION             = "CT_Z_DIRECTION";
const QString CT_AbstractCategory::DATA_DIMENSION               = "CT_DIMENSION";
const QString CT_AbstractCategory::DATA_X_DIMENSION             = "CT_X_DIMENSION";
const QString CT_AbstractCategory::DATA_Y_DIMENSION             = "CT_Y_DIMENSION";
const QString CT_AbstractCategory::DATA_Z_DIMENSION             = "CT_Z_DIMENSION";
const QString CT_AbstractCategory::DATA_RADIUS                  = "CT_RADIUS";
const QString CT_AbstractCategory::DATA_R2                      = "CT_R2";
const QString CT_AbstractCategory::DATA_SIZE                    = "CT_SIZE";
const QString CT_AbstractCategory::DATA_LENGTH                  = "CT_LENGTH";
const QString CT_AbstractCategory::DATA_WIDTH                   = "CT_WIDTH";
const QString CT_AbstractCategory::DATA_HEIGHT                  = "CT_HEIGHT";
const QString CT_AbstractCategory::DATA_ANGLE                   = "CT_ANGLE";
const QString CT_AbstractCategory::DATA_RATIO                   = "CT_RATIO";
const QString CT_AbstractCategory::DATA_RESOLUTION              = "CT_RESOLUTION";
const QString CT_AbstractCategory::DATA_X_RESOLUTION            = "CT_X_RESOLUTION";
const QString CT_AbstractCategory::DATA_Y_RESOLUTION            = "CT_Y_RESOLUTION";
const QString CT_AbstractCategory::DATA_Z_RESOLUTION            = "CT_Z_RESOLUTION";
const QString CT_AbstractCategory::DATA_NA                      = "CT_NA";

CT_AbstractCategory::CT_AbstractCategory(const QString &uniqueName,
                                         const QString &categoryEquivalentUniqueName,
                                         const QString &displayableName,
                                         const QString &description)
{
    m_uName = uniqueName;
    m_dName = displayableName;
    m_description = description;

    if(m_dName.isEmpty())
        m_dName = m_uName;

    if(!categoryEquivalentUniqueName.isEmpty())
        m_eCategories.append(categoryEquivalentUniqueName);

    internalTestAttributes("CT_AbstractCategory constructor");
}

CT_AbstractCategory::CT_AbstractCategory(const QString &uniqueName,
                                         const QList<QString> &categoryEquivalentUniqueNameCollection,
                                         const QString &displayableName,
                                         const QString &description)
{
    m_uName = uniqueName;
    m_dName = displayableName;
    m_description = description;

    if(m_dName.isEmpty())
        m_dName = m_uName;

    m_eCategories = categoryEquivalentUniqueNameCollection;

    internalTestAttributes("CT_AbstractCategory constructor");
}

bool CT_AbstractCategory::addEquivalentCategory(const QString &categoryEquivalentUniqueName)
{
    if(!m_eCategories.contains(categoryEquivalentUniqueName))
        m_eCategories.append(categoryEquivalentUniqueName);

    return true;
}

QString CT_AbstractCategory::uniqueName() const
{
    return m_uName;
}

QString CT_AbstractCategory::valueTypeToString(CT_AbstractCategory::ValueType v) const
{
    int index = metaObject()->indexOfEnumerator("ValueType");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    return metaEnum.valueToKey(v);
}

QString CT_AbstractCategory::displayableName() const
{
    return m_dName;
}

QString CT_AbstractCategory::description() const
{
    return m_description;
}

bool CT_AbstractCategory::isEquivalentTo(const CT_AbstractCategory *c) const
{
    return m_eCategories.contains(c->uniqueName());
}

bool CT_AbstractCategory::isEquivalentTo(const QString &categoryUniqueName) const
{
    return m_eCategories.contains(categoryUniqueName);
}

void CT_AbstractCategory::internalTestAttributes(const QString &where)
{
    Q_ASSERT_X(!m_uName.isEmpty(), qPrintable(where), "uniqueName is empty");
}

