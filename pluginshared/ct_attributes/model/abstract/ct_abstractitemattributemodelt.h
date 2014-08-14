#ifndef CT_ABSTRACTITEMATTRIBUTEMODELT_H
#define CT_ABSTRACTITEMATTRIBUTEMODELT_H

#include "pluginShared_global.h"

#include <QList>
#include <QString>

class CT_AbstractModel;

/**
 * @brief Represent a model for an item attribute (IN or OUT). A item attribute has no children.
 *
 *        Use the template parameters to define if you want a IN or OUT model
 */
template<class ItemModelT, class InheritedT>
class CT_AbstractItemAttributeModelT : public InheritedT
{
public:
    CT_AbstractItemAttributeModelT(const QString &uniqueName,
                                   const QString &description,
                                   const QString &displayableName);

    virtual ~CT_AbstractItemAttributeModelT();

    // INHERITED FROM CT_AbstractModel
    QList<CT_AbstractModel*> childrens() const;
    bool isEmpty() const;
};

#include "ct_attributes/model/abstract/ct_abstractitemattributemodelt.hpp"

#endif // CT_ABSTRACTITEMATTRIBUTEMODELT_H
