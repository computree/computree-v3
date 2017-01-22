#ifndef CDM_CITATIONINFO_H
#define CDM_CITATIONINFO_H

#include "computreeCore_global.h"
#include "interfaces.h"
#include "cdm_stepmanager.h"
#include "cdm_pluginmanager.h"


#include <QObject>

class COMPUTREECORESHARED_EXPORT CDM_CitationInfo : public QObject
{
    Q_OBJECT
public:
    CDM_CitationInfo(CDM_StepManager *stepManager, CDM_PluginManager *pluginManager);

    QString getStepByPluginList();    
    QString getPluginBibTex();
    QString parseBibTex(QString bibTex);
private:
    void recursiveGetStepList(CT_VirtualAbstractStep *step, QList<CT_VirtualAbstractStep *> &stepList);

    QList<CT_VirtualAbstractStep *> _stepList;
    CDM_StepManager*                _stepManager;
    CDM_PluginManager*              _pluginManager;
};

#endif // CDM_CITATIONINFO_H
