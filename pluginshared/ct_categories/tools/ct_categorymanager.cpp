#include "ct_categorymanager.h"

#include "ct_categories/abstract/ct_abstractcategory.h"

CT_CategoryManager::CT_CategoryManager()
{
}

CT_CategoryManager::~CT_CategoryManager()
{
    qDeleteAll(m_categories.begin(), m_categories.end());
}

void CT_CategoryManager::registerCategory(CT_AbstractCategory *c)
{
    m_categories.insert(c->uniqueName(), c);
}

const CT_AbstractCategory* CT_CategoryManager::findByUniqueName(const QString &uniqueName) const
{
    return m_categories.value(uniqueName, NULL);
}

QList<const CT_AbstractCategory *> CT_CategoryManager::categories() const
{
    return m_categories.values();
}
