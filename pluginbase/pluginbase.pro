include(../shared.pri)
include($${PLUGIN_SHARED_DIR}/include.pri)

TARGET = plug_base

greaterThan(QT_MAJOR_VERSION, 4): QT += concurrent

HEADERS += $${PLUGIN_SHARED_INTERFACE_DIR}/interfaces.h \
    pb_pluginentry.h \
    step/pb_stepgenericexporter.h \
    step/pb_steploadasciifile.h \
    step/pb_steploadasciifile02.h \
    step/pb_steploadmultixybfiles.h \
    actions/pb_actionselectitemdrawablegv.h \
    actions/pb_actionshowitemdatagv.h \
    views/actions/pb_actionselectitemdrawablegvoptions.h \
    step/pb_stepuseritemselection.h \
    step/pb_stepcomputehitgrid.h \
    tools/pb_computehitsthread.h \
    actions/pb_actionselectcellsingrid3d.h \
    views/actions/pb_actionselectcellsingrid3doptions.h \
    step/pb_stepselectcellsingrid3d.h \
    step/pb_steploadobjfile.h \
    views/actions/pb_actionselectcellsingrid3dcolonizedialog.h \
    step/pb_steploadgrid3dfile.h \
    step/pb_stepfilterpointsbyboolgrid.h \
    exporters/xyb/pb_xybexporter.h \
    exporters/csv/pb_csvexporter.h \
    exporters/csv/pb_csvexportercolumn.h \
    exporters/csv/pb_csvexporterconfiguration.h \
    exporters/grid2d/pb_grid2dexporter.h \
    exporters/grid3d/pb_grid3dexporter.h \
    views/exporters/csv/pbg_csvconfigurationdialog.h \
    views/exporters/csv/pbg_csvdatareflistwidget.h \
    views/exporters/csv/pbg_csvpreviewwidget.h \
    pb_steppluginmanager.h \
    step/pb_stepgenericloadfile.h \
    exporters/ascrgb/pb_ascrgbexporter.h \
    views/actions/pb_actionsegmentcrownsoptions.h \
    actions/pb_actionsegmentcrowns.h \
    step/pb_stepsegmentcrowns.h \
    views/actions/pb_actiondefineheightlayeroptions.h \
    actions/pb_actiondefineheightlayer.h \
    step/pb_stepreducepointsdensity.h \
    step/pb_stepaddaffiliationid.h \
    step/pb_stepsetaffiliationidfromreference.h \
    actions/pb_actionmodifyaffiliations.h \
    views/actions/pb_actionmodifyaffiliationsoptions.h \
    exporters/groupdata/pb_groupdataexporter.h \
    exporters/mesh/pb_meshobjexporter.h \
    step/pb_stepuseritemcopy.h \
    exporters/topology/pb_opfexporter.h \
    step/pb_steploadpbmfile.h \
    step/pb_steploadpgmfile.h \
    exporters/pbm/pb_pbmexporter.h \
    exporters/pgm/pb_pgmexporter.h \
    exporters/grid3d/pb_grid3dastableexporter.h \
    exporters/profile/pb_profileexporter.h \
    step/pb_stepsegmentgaps.h \
    actions/pb_actionsegmentgaps.h \
    views/actions/pb_actionsegmentgapsoptions.h \
    step/pb_stepslicepointcloud.h \
    actions/pb_actionslicepointcloud.h \
    views/actions/pb_actionslicepointcloudoptions.h \
    exporters/polygon2d/pb_polygon2dexporter.h \
    actions/pb_actionpickitemsinlist.h \
    views/actions/pb_actionpickitemsinlistoptions.h \
    exporters/las/pb_lasexporter.h \
    exporters/las/pb_laspointformat.h \
    step/pb_stepmanualinventory.h \
    actions/pb_actionmanualinventory.h \
    views/actions/pb_actionmanualinventoryoptions.h \
    views/actions/pb_actionmanualinventoryattributesdialog.h \
    step/pb_stepmatchitemspositions.h \
    step/pb_stepimportsegmafilesformatching.h \
    step/pb_stepvalidateinventory.h \
    actions/pb_actionvalidateinventory.h \
    views/actions/pb_actionvalidateinventoryoptions.h \
    views/actions/pb_actionvalidateinventoryattributesdialog.h \
    step/pb_stepselectgroupsbyreferenceheight.h \
    step/pb_stepbeginloopthroughgroups.h \
    step/pb_stepcreatedatasource.h \
    step/pb_steploaddatafromitemposition.h \
    step/pb_steptransformpointcloud.h \
    step/pb_stepfilteritemsbyposition.h \
    step/pb_stepbeginloopthroughdatasource.h \
    step/pb_stepcomputepointsmetrics.h \
    step/pb_stepcreateplotmanagerfromfile.h \
    step/pb_stepcreateplotmanagergrid.h \
    step/pb_steploadpositionsformatching.h \
    step/pb_stepcompare3dgridscontents.h \
    step/pb_stepselectcellsingrid3dbybinarypattern.h \
    step/pb_stepextractlogbuffer.h \
    step/pb_stepfitcylinderoncluster.h \
    step/pb_stepextractpositionsfromdensity.h \
    step/pb_stepmergeclustersfrompositions.h \
    actions/pb_actionmodifyclustersgroups.h \
    views/actions/pb_actionmodifyclustersgroupsoptions.h \
    exporters/gdal/pb_gdalexporter.h

SOURCES += \
    pb_pluginentry.cpp \
    step/pb_steploadasciifile.cpp \
    step/pb_stepgenericexporter.cpp \
    step/pb_steploadasciifile02.cpp \
    step/pb_steploadmultixybfiles.cpp \
    actions/pb_actionselectitemdrawablegv.cpp \
    actions/pb_actionshowitemdatagv.cpp \
    views/actions/pb_actionselectitemdrawablegvoptions.cpp \
    step/pb_stepuseritemselection.cpp \
    step/pb_stepcomputehitgrid.cpp \
    tools/pb_computehitsthread.cpp \
    actions/pb_actionselectcellsingrid3d.cpp \
    views/actions/pb_actionselectcellsingrid3doptions.cpp \
    step/pb_stepselectcellsingrid3d.cpp \
    step/pb_steploadobjfile.cpp \
    views/actions/pb_actionselectcellsingrid3dcolonizedialog.cpp \
    step/pb_steploadgrid3dfile.cpp \
    step/pb_stepfilterpointsbyboolgrid.cpp \
    exporters/xyb/pb_xybexporter.cpp \
    exporters/csv/pb_csvexporter.cpp \
    exporters/csv/pb_csvexportercolumn.cpp \
    exporters/csv/pb_csvexporterconfiguration.cpp \
    exporters/grid2d/pb_grid2dexporter.cpp \
    exporters/grid3d/pb_grid3dexporter.cpp \
    views/exporters/csv/pbg_csvconfigurationdialog.cpp \
    views/exporters/csv/pbg_csvdatareflistwidget.cpp \
    views/exporters/csv/pbg_csvpreviewwidget.cpp \
    pb_steppluginmanager.cpp \
    step/pb_stepgenericloadfile.cpp \
    exporters/ascrgb/pb_ascrgbexporter.cpp \
    views/actions/pb_actionsegmentcrownsoptions.cpp \
    actions/pb_actionsegmentcrowns.cpp \
    step/pb_stepsegmentcrowns.cpp \
    views/actions/pb_actiondefineheightlayeroptions.cpp \
    actions/pb_actiondefineheightlayer.cpp \
    step/pb_stepreducepointsdensity.cpp \
    step/pb_stepaddaffiliationid.cpp \
    step/pb_stepsetaffiliationidfromreference.cpp \
    actions/pb_actionmodifyaffiliations.cpp \
    views/actions/pb_actionmodifyaffiliationsoptions.cpp \
    exporters/groupdata/pb_groupdataexporter.cpp \
    exporters/mesh/pb_meshobjexporter.cpp \
    step/pb_stepuseritemcopy.cpp \
    exporters/topology/pb_opfexporter.cpp \
    step/pb_steploadpbmfile.cpp \
    step/pb_steploadpgmfile.cpp \
    exporters/pbm/pb_pbmexporter.cpp \
    exporters/pgm/pb_pgmexporter.cpp \
    exporters/grid3d/pb_grid3dastableexporter.cpp \
    exporters/profile/pb_profileexporter.cpp \
    step/pb_stepsegmentgaps.cpp \
    actions/pb_actionsegmentgaps.cpp \
    views/actions/pb_actionsegmentgapsoptions.cpp \
    step/pb_stepslicepointcloud.cpp \
    actions/pb_actionslicepointcloud.cpp \
    views/actions/pb_actionslicepointcloudoptions.cpp \
    exporters/polygon2d/pb_polygon2dexporter.cpp \
    actions/pb_actionpickitemsinlist.cpp \
    views/actions/pb_actionpickitemsinlistoptions.cpp \
    exporters/las/pb_lasexporter.cpp \
    step/pb_stepmanualinventory.cpp \
    actions/pb_actionmanualinventory.cpp \
    views/actions/pb_actionmanualinventoryoptions.cpp \
    views/actions/pb_actionmanualinventoryattributesdialog.cpp \
    step/pb_stepmatchitemspositions.cpp \
    step/pb_stepimportsegmafilesformatching.cpp \
    step/pb_stepvalidateinventory.cpp \
    actions/pb_actionvalidateinventory.cpp \
    views/actions/pb_actionvalidateinventoryoptions.cpp \
    views/actions/pb_actionvalidateinventoryattributesdialog.cpp \
    step/pb_stepselectgroupsbyreferenceheight.cpp \
    step/pb_stepbeginloopthroughgroups.cpp \
    step/pb_stepcreatedatasource.cpp \
    step/pb_steploaddatafromitemposition.cpp \
    step/pb_steptransformpointcloud.cpp \
    step/pb_stepfilteritemsbyposition.cpp \
    step/pb_stepbeginloopthroughdatasource.cpp \
    step/pb_stepcomputepointsmetrics.cpp \
    step/pb_stepcreateplotmanagerfromfile.cpp \
    step/pb_stepcreateplotmanagergrid.cpp \
    step/pb_steploadpositionsformatching.cpp \
    step/pb_stepcompare3dgridscontents.cpp \
    step/pb_stepselectcellsingrid3dbybinarypattern.cpp \
    step/pb_stepextractlogbuffer.cpp \
    step/pb_stepfitcylinderoncluster.cpp \
    step/pb_stepextractpositionsfromdensity.cpp \
    step/pb_stepmergeclustersfrompositions.cpp \
    actions/pb_actionmodifyclustersgroups.cpp \
    views/actions/pb_actionmodifyclustersgroupsoptions.cpp \
    exporters/gdal/pb_gdalexporter.cpp

INCLUDEPATH += .
INCLUDEPATH += ./actions
INCLUDEPATH += ./views
INCLUDEPATH += ./views/actions

RESOURCES += \
    resource.qrc

FORMS += \
    views/actions/pb_actionselectitemdrawablegvoptions.ui \
    views/actions/pb_actionselectcellsingrid3doptions.ui \
    views/actions/pb_actionselectcellsingrid3dcolonizedialog.ui \
    views/exporters/csv/pbg_csvconfigurationdialog.ui \
    views/exporters/csv/pbg_csvdatareflistwidget.ui \
    views/exporters/csv/pbg_csvpreviewwidget.ui \
    views/actions/pb_actionsegmentcrownsoptions.ui \
    views/actions/pb_actiondefineheightlayeroptions.ui \
    views/actions/pb_actionmodifyaffiliationsoptions.ui \
    views/actions/pb_actionsegmentgapsoptions.ui \
    views/actions/pb_actionslicepointcloudoptions.ui \
    views/actions/pb_actionpickitemsinlistoptions.ui \
    views/actions/pb_actionmanualinventoryoptions.ui \
    views/actions/pb_actionmanualinventoryattributesdialog.ui \
    views/actions/pb_actionvalidateinventoryoptions.ui \
    views/actions/pb_actionvalidateinventoryattributesdialog.ui \
    views/actions/pb_actionmodifyclustersgroupsoptions.ui

TRANSLATIONS += languages/pluginbase_fr.ts \
                languages/pluginbase_en.ts
