#ifndef DM_MULTIPLEITEMDRAWABLEMODELMANAGER_H
#define DM_MULTIPLEITEMDRAWABLEMODELMANAGER_H

#include <QList>

class CT_AbstractResult;

class DM_MultipleItemDrawableModelManager
{
public:
    DM_MultipleItemDrawableModelManager();
    virtual ~DM_MultipleItemDrawableModelManager();

    virtual void addResult(const CT_AbstractResult *res) = 0;
    virtual void removeResult(const CT_AbstractResult *res) = 0;

    virtual CT_AbstractResult* currentResult() const = 0;
};

#endif // DM_MULTIPLEITEMDRAWABLEMODELMANAGER_H
