#include "steptest.h"

#include "step/pb_stepgenericloadfile.h"
#include "step/pb_stepgenericexporter.h"
#include "ct_reader/ct_reader_xyb.h"
#include "exporters/xyb/pb_xybexporter.h"
#include "ct_abstractstepplugin.h"
#include "ct_step/ct_stepinitializedata.h"

StepTest::StepTest()
{
}

void StepTest::testStep()
{
    TextPluginManager pManager;

    CT_VirtualAbstractStep *stepLoadFile = new PB_StepGenericLoadFile(*(new CT_StepInitializeData(NULL, pManager, NULL)), new CT_Reader_XYB());
    CT_VirtualAbstractStep *stepExport = new PB_StepGenericExporter(*(new CT_StepInitializeData(NULL, pManager, NULL)), "", new PB_XYBExporter());

    if(stepExport->acceptAddAfterThisStep(stepLoadFile))
    {

    }
}
