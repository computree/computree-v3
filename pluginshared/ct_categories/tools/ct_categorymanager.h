#ifndef CT_CATEGORYMANAGER_H
#define CT_CATEGORYMANAGER_H

#include "pluginShared_global.h"
#include <QHash>

class CT_AbstractCategory;

/**
 * @brief The manager of categories. A category must be registered to use it with a attribute.
 */
class PLUGINSHAREDSHARED_EXPORT CT_CategoryManager
{
public:
    CT_CategoryManager();
    ~CT_CategoryManager();

    /**
     * @brief Register a new category
     */
    void registerCategory(CT_AbstractCategory *c);

    /**
     * @brief Find a category by a unique name
     * @return NULL if the category was not found
     */
    const CT_AbstractCategory* findByUniqueName(const QString &uniqueName) const;

    /**
     * @brief Returns all categories registered
     */
    QList<const CT_AbstractCategory*> categories() const;

private:
    QHash<QString, const CT_AbstractCategory*> m_categories;
};

#endif // CT_CATEGORYMANAGER_H
