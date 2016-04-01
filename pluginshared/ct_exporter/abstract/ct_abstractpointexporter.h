#ifndef CT_ABSTRACTPOINTEXPORTER_H
#define CT_ABSTRACTPOINTEXPORTER_H

#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractPointExporter
{

public:
    CT_AbstractPointExporter();
    ~CT_AbstractPointExporter();

    virtual bool createExportFile() = 0;
    virtual bool exportPointsToFile(CT_AbstractPointCloudIndex *indexVector) = 0;
    virtual void finalizeExportFile() = 0;

};

#endif // CT_ABSTRACTPOINTEXPORTER_H
