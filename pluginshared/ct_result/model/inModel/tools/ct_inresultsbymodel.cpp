#include "ct_inresultsbymodel.h"

CT_InResultsByModel::CT_InResultsByModel(const CT_InAbstractResultModel *model)
{
    _model = (CT_InAbstractResultModel*)model;
}

CT_InAbstractResultModel* CT_InResultsByModel::getInModel() const
{
    return _model;
}

void CT_InResultsByModel::addResult(const CT_AbstractResult *result)
{
    _results.append((CT_AbstractResult*)result);
}

const QList<CT_AbstractResult*>& CT_InResultsByModel::getResults() const
{
    return _results;
}

void CT_InResultsByModel::clearResults()
{
    _results.clear();
}
