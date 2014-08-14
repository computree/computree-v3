#ifndef PB_STEPGENERICGROUPEXPORTER_H
#define PB_STEPGENERICGROUPEXPORTER_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "exporters/groupdata/pb_groupdataexporter.h"

/*!
 * \class PB_StepGenericGroupExporter
 * \ingroup Steps_PB
 * \brief <b>Generic exporter</b>
 *
 * This step can export any ItemDrawable by the way of
 * loaded exporter plugins.
 *
 */
class PB_StepGenericGroupExporter : public CT_AbstractStep
{
    Q_OBJECT

public:
    PB_StepGenericGroupExporter(CT_StepInitializeData &dataInit);
    ~PB_StepGenericGroupExporter();

    // CT_AbstractStep non obligatoire :
    QString getStepDescription() const;

    // CT_AbstractStep obligatoire :
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:

    // CT_AbstractStep obligatoire :
    void createInResultModelListProtected();


    void createPostConfigurationDialog();

    // CT_AbstractStep obligatoire :
    void createOutResultModelListProtected();

    // CT_AbstractStep obligatoire :
    void compute();

private:
    PB_GroupDataExporter    *_exporter;
    QStringList             _exportFilename;
};

#endif // PB_STEPGENERICGROUPEXPORTER_H
