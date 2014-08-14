#ifndef DM_MULTIPLEITEMDRAWABLEMODELMANAGER_H
#define DM_MULTIPLEITEMDRAWABLEMODELMANAGER_H

#include <QList>

class Result;

class DM_MultipleItemDrawableModelManager
{
public:
    DM_MultipleItemDrawableModelManager();
    virtual ~DM_MultipleItemDrawableModelManager();

    virtual void addResult(const Result *res) = 0;
    virtual void removeResult(const Result *res) = 0;

    virtual Result* currentResult() const = 0;
};

#endif // DM_MULTIPLEITEMDRAWABLEMODELMANAGER_H
