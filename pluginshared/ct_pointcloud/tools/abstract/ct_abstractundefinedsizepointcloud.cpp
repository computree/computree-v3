#include "ct_abstractundefinedsizepointcloud.h"

#include "ct_global/ct_context.h"

CT_AbstractUndefinedSizePointCloud::CT_AbstractUndefinedSizePointCloud() : QObject()
{
}

CT_AbstractUndefinedSizePointCloud::~CT_AbstractUndefinedSizePointCloud()
{
}

CT_AbstractPointCloud* CT_AbstractUndefinedSizePointCloud::globalPointCloud() const
{
    return PS_REPOSITORY->globalPointCloud();
}
