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
    struct StepCitationInfo
    {
        StepCitationInfo(int num, QString stepName, QString stepDescription, QString pluginName)
        {
            _num = num;
            _stepName = stepName;
            _stepDescription = stepDescription;
            _pluginName = pluginName;
        }

        int _num;
        QString _stepName;
        QString _stepDescription;
        QString _pluginName;
    };


    CDM_CitationInfo(CDM_StepManager *stepManager, CDM_PluginManager *pluginManager);

    QList<CDM_CitationInfo::StepCitationInfo> getScriptTable();
    QString getPluginAndStepCitations();
    QString getPluginRIS();
    QString parseRIS(QString ris);
private:
    void recursiveGetStepList(CT_VirtualAbstractStep *step, QList<CT_VirtualAbstractStep *> &stepList);

    QList<CT_VirtualAbstractStep *> _stepList;
    CDM_StepManager*                _stepManager;
    CDM_PluginManager*              _pluginManager;

    QString                         _computreeCitationRIS;
};

#endif // CDM_CITATIONINFO_H
