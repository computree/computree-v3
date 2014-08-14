#ifndef CT_INRESULTSBYMODEL_H
#define CT_INRESULTSBYMODEL_H

#include "pluginShared_global.h"

#include <QList>

class CT_InAbstractResultModel;
class CT_AbstractResult;

class PLUGINSHAREDSHARED_EXPORT CT_InResultsByModel
{
public:

    CT_InResultsByModel(const CT_InAbstractResultModel *model);

    CT_InAbstractResultModel* getInModel() const;
    void addResult(const CT_AbstractResult *result);
    const QList<CT_AbstractResult*>& getResults() const;
    void clearResults();

private:

    CT_InAbstractResultModel    *_model;
    QList<CT_AbstractResult*>   _results;
};

#endif // CT_INRESULTSBYMODEL_H
