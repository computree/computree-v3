TARGET = pluginShared
QT += xml
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets concurrent

isEmpty(TEST_FEATURES) {
    TEMPLATE = lib
} else {
    message("TEST du plugin")
    TEMPLATE = app
}

include(../destdir.pri)

DESTDIR = $${PLUGINSHARED_DESTDIR}
OBJECTS_DIR = .obj
MOC_DIR = .moc
UI_DIR = .ui
RCC_DIR = .rcc

# n'affiche plus les warnings lorsqu'un parametre de mthode n'est pas utilis
win32-g++ {
    QMAKE_CXXFLAGS += -Wno-unused-parameter
}

DEFINES += PLUGINSHARED_LIBRARY

include(include.pri)

!isEmpty(TEST_FEATURES) {
    SOURCES +=
}

SOURCES += \
    ct_abstractstepplugin.cpp \
    ct_stepseparator.cpp \
    ct_steploadfileseparator.cpp \
    ct_step/abstract/ct_abstractstep.cpp \
    ct_step/abstract/ct_abstractsteploadfile.cpp \
    ct_step/abstract/ct_virtualabstractstep.cpp \
    ct_step/ct_stepinitializedata.cpp \
    ct_result/abstract/ct_abstractresult.cpp \
    ct_result/ct_resultinitializedata.cpp \
    ct_point.cpp \
    ct_itemdrawable/abstract/ct_abstractitemdrawable.cpp \
    ct_itemdrawable/ct_scene.cpp \
    ct_itemdrawable/ct_itemdrawableconfiguration.cpp \
    ct_view/ct_stepconfigurabledialog.cpp \
    ct_view/ct_doublespinbox.cpp \
    ct_view/ct_checkbox.cpp \
    ct_view/ct_spinbox.cpp \
    ct_view/ct_radiobutton.cpp \
    ct_view/ct_buttongroup.cpp \
    ct_result/ct_resultcopymodelist.cpp \
    ct_pointcloud/abstract/ct_abstractpointcloud.cpp \
    ct_pointcloud/ct_pointcloudstdvector.cpp \
    ct_pointcloudindex/abstract/ct_abstractpointcloudindex.cpp \
    ct_pointcloudindex/ct_pointcloudindexvector.cpp \
    ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.cpp \
    ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.cpp \
    ct_math/ct_mathboundingshape.cpp \
    ct_itemdrawable/ct_pointcluster.cpp \
    ct_math/ct_mathpoint.cpp \
    ct_itemdrawable/ct_container.cpp \
    ct_itemdrawable/abstract/ct_abstractshape.cpp \
    ct_itemdrawable/ct_circle.cpp \
    ct_itemdrawable/ct_ellipse.cpp \
    ct_itemdrawable/ct_line.cpp \
    ct_triangulation/ct_voronoinodet.cpp \
    ct_triangulation/ct_trianglet.cpp \
    ct_triangulation/ct_delaunayt.cpp \
    ct_triangulation/ct_edget.cpp \
    ct_triangulation/ct_nodet.cpp \
    ct_itemdrawable/ct_cylinder.cpp \
    ct_shapedata/ct_shapedata.cpp \
    ct_shapedata/ct_circledata.cpp \
    ct_shapedata/ct_cylinderdata.cpp \
    ct_shapedata/ct_linedata.cpp \
    ct_shapedata/ct_ellipsedata.cpp \
    ct_itemdrawable/ct_scanner.cpp \
    ct_itemdrawable/ct_triangulation2d.cpp \
    ct_math/ct_mathfittedline2d.cpp \
    ct_view/ct_combobox.cpp \
    ct_step/abstract/ct_abstractsteploadfileinscene.cpp \
    ct_view/ct_filechoicebutton.cpp \
    ct_step/abstract/ct_abstractstepserializable.cpp \
    ct_step/abstract/ct_abstractstepcanbeaddedfirst.cpp \
    ct_stepcanbeaddedfirstseparator.cpp \
    ct_itemdrawable/abstract/ct_abstractitemgroup.cpp \
    ct_result/ct_resultgroup.cpp \
    ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.cpp \
    ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.cpp \
    ct_result/model/inModel/ct_inresultmodelgroup.cpp \
    ct_itemdrawable/ct_standarditemgroup.cpp \
    ct_result/model/outModel/ct_outresultmodelgroup.cpp \
    ct_result/model/inModel/ct_inresultmodelnotneedinputresult.cpp \
    ct_result/model/outModel/abstract/ct_outabstractresultmodel.cpp \
    ct_result/model/inModel/tools/ct_inresultmodelmanager.cpp \
    ct_turn/inTurn/ct_inturn.cpp \
    ct_turn/inTurn/tools/ct_inturnmanager.cpp \
    ct_result/tools/ct_inmanager.cpp \
    ct_result/model/outModel/tools/ct_outresultmodelmanager.cpp \
    ct_result/tools/ct_outmanager.cpp \
    ct_result/model/inModel/tools/ct_inresultmodelconfigurationmanager.cpp \
    ct_result/tools/ct_inresultmanager.cpp \
    ct_result/tools/ct_outresultmanager.cpp \
    ct_turn/outTurn/ct_outturn.cpp \
    ct_turn/outTurn/tools/ct_outturnmanager.cpp \
    ct_result/tools/ct_turnindexmanager.cpp \
    ct_result/tools/iterator/private/ct_treestructureforiterator.cpp \
    ct_view/inModel/ctg_inresultmodelpossibilities.cpp \
    ct_view/inModel/ctg_inresultmodelconfiguration.cpp \
    ct_view/inModel/ctg_inresultmodelturnchoice.cpp \
    ct_view/inModel/ctg_inturnmanager.cpp \
    ct_view/inModel/ctg_inmodelpossibilitieschoice.cpp \
    ct_result/model/inModel/tools/ct_inresultsbymodel.cpp \
    ct_result/model/inModel/ct_inresultmodelgrouptocopy.cpp \
    ct_result/model/outModel/ct_outresultmodelgroupcopy.cpp \
    ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibility.cpp \
    ct_result/model/outModel/tools/ct_iteratoritemsmodelforresultmodelgroup.cpp \
    ct_tools/model/ct_autorenamemodels.cpp \
    ct_shapedata/ct_boxdata.cpp \
    ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.cpp \
    ct_tools/model/ct_generateoutmodelname.cpp \
    ct_itemdrawable/tools/ct_standardcontext.cpp \
    ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.cpp \
    ct_itemdrawable/ct_referencepoint.cpp \
    ct_view/tools/ct_textfileconfigurationdialog.cpp \
    ct_math/ct_math2dlines.cpp \
    ct_math/ct_sphericalline3d.cpp \
    ct_itemdrawable/tools/drawmanager/abstract/ct_abstractitemdrawabledrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithpointclouddrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardcircledrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractshapedrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardellipsedrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardscannerdrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardstandarditemgroupdrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardcontainerdrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardcylinderdrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardlinedrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardpointclusterdrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardreferencepointdrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardtriangulation2ddrawmanager.cpp \
    ct_tools/model/abstract/ct_abstractoutmodelcopyaction.cpp \
    ct_tools/model/ct_generateoutresultmodelname.cpp \
    ct_tools/model/ct_outmodelcopyactionaddmodelgroupingroup.cpp \
    ct_tools/model/ct_outmodelcopyactionaddmodelitemingroup.cpp \
    ct_tools/model/ct_outmodelcopyactionremovemodelgroupingroup.cpp \
    ct_tools/model/ct_outmodelcopyactionremovemodelitemingroup.cpp \
    ct_result/model/outModel/tools/ct_iteratorgroupsmodelforresultmodelgroup.cpp \
    ct_view/ct_lineedit.cpp \
    ct_math/ct_math.cpp \
    ct_itemdrawable/ct_grid3d.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid3ddrawmanager.cpp \
    ct_itemdrawable/ct_beam.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardbeamdrawmanager.cpp \
    ct_itemdrawable/tools/gridtools/ct_grid3dwootraversalalgorithm.cpp \
    ct_itemdrawable/tools/gridtools/ct_abstractgrid3dbeamvisitor.cpp \
    ct_global/ct_context.cpp \
    ct_global/ct_repository.cpp \
    ct_tools/ct_monitoredqthread.cpp \
    ct_pointcloudindex/ct_pointcloudindexlessmemory.cpp \
    ct_global/ct_repositorymanager.cpp \
    ct_pointcloud/tools/abstract/ct_abstractundefinedsizepointcloud.cpp \
    ct_pointcloud/tools/ct_standardundefinedsizepointcloud.cpp \
    ct_reader/abstract/ct_abstractreader.cpp \
    ct_reader/ct_reader_xyb.cpp \
    ct_itemdrawable/abstract/ct_abstractpointsattributes.cpp \
    ct_colorcloud/abstract/ct_abstractcolorcloud.cpp \
    ct_colorcloud/ct_colorcloudstdvector.cpp \
    ct_colorcloud/registered/ct_standardcolorcloudregistered.cpp \
    ct_itemdrawable/ct_pointsattributescolor.cpp \
    ct_itemdrawable/ct_pointsattributesnormal.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractpointsattributesdrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardpointsattributesnormaldrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardpointsattributescolordrawmanager.cpp \
    ct_normalcloud/ct_normalcloudstdvector.cpp \
    ct_normalcloud/abstract/ct_abstractnormalcloud.cpp \
    ct_normal.cpp \
    ct_reader/ct_reader_ascrgb.cpp \
    ct_actions/abstract/ct_abstractaction.cpp \
    ct_actions/abstract/ct_abstractactionforgraphicsview.cpp \
    ct_actions/abstract/ct_abstractactionfortreeview.cpp \
    ct_actions/ct_actionsseparator.cpp \
    ct_view/actions/abstract/ct_gabstractactionoptions.cpp \
    ct_itemdrawable/tools/pointclustertools/ct_polylinesalgorithms.cpp \
    ct_normalcloud/registered/ct_standardnormalcloudregistered.cpp \
    ct_reader/ct_reader_larchitect_grid.cpp \
    ct_itemdrawable/abstract/ct_abstractgrid3d.cpp \
    ct_itemdrawable/abstract/ct_abstractgrid2d.cpp \
    ct_itemdrawable/ct_grid2dxy.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid2dxydrawmanager.cpp \
    ct_itemdrawable/abstract/ct_virtualgrid2d.cpp \
    ct_itemdrawable/ct_grid2dyz.cpp \
    ct_itemdrawable/ct_grid2dxz.cpp \
    ct_mesh/ct_face.cpp \
    ct_mesh/ct_edge.cpp \
    ct_mesh/ct_mesh.cpp \
    ct_cloud/abstract/ct_abstractcloud.cpp \
    ct_cloud/registered/abstract/ct_abstractcloudregistered.cpp \
    ct_cloudindex/abstract/ct_abstractcloudindex.cpp \
    ct_cloud/tools/abstract/ct_abstractcloudsynctoglobalcloudmanager.cpp \
    ct_cloudindex/registered/abstract/ct_abstractcloudindexregistered.cpp \
    ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanager.cpp \
    ct_cloud/tools/ct_globalpointcloudmanager.cpp \
    ct_itemdrawable/ct_meshmodel.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardmeshmodeldrawmanager.cpp \
    ct_mesh/cloud/abstract/ct_abstractfacecloudindex.cpp \
    ct_mesh/cloud/abstract/ct_abstractedgecloudindex.cpp \
    ct_mesh/cloud/ct_facecloudindexlessmemory.cpp \
    ct_mesh/cloud/ct_facecloudindexvector.cpp \
    ct_mesh/cloud/ct_edgecloudindexlessmemory.cpp \
    ct_mesh/cloud/ct_edgecloudindexvector.cpp \
    ct_itemdrawable/abstract/ct_abstractattributes.cpp \
    ct_attributes/ct_attributescolor.cpp \
    ct_attributes/ct_attributesnormal.cpp \
    ct_attributes/ct_attributesscalart.cpp \
    ct_attributes/abstract/ct_abstractattributesscalar.cpp \
    ct_itemdrawable/abstract/ct_abstractfaceattributes.cpp \
    ct_itemdrawable/abstract/ct_abstractedgeattributes.cpp \
    ct_itemdrawable/ct_faceattributescolor.cpp \
    ct_itemdrawable/ct_edgeattributescolor.cpp \
    ct_itemdrawable/ct_edgeattributesnormal.cpp \
    ct_itemdrawable/ct_edgeattributesscalart.cpp \
    ct_itemdrawable/abstract/ct_abstractpointattributesscalar.cpp \
    ct_itemdrawable/abstract/ct_abstractfaceattributesscalar.cpp \
    ct_itemdrawable/abstract/ct_abstractedgeattributesscalar.cpp \
    ct_cloud/tools/ct_globaledgecloudmanager.cpp \
    ct_cloud/tools/ct_globalfacecloudmanager.cpp \
    ct_cloud/tools/iglobalcloudlistener.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid2dxzdrawmanager.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid2dyzdrawmanager.cpp \
    ct_itemdrawable/tools/gridtools/ct_grid3dbeamvisitor_indexlist.cpp \
    ct_log/ct_basicostreamloglistener.cpp \
    ct_log/ct_fileloglistener.cpp \
    ct_log/abstract/ct_abstractloglistener.cpp \
    ct_exporter/ct_standardexporterseparator.cpp \
    ct_exporter/abstract/ct_abstractexporter.cpp \
    ct_reader/ct_standardreaderseparator.cpp \
    ct_tools/itemdrawable/ct_itemdrawablecollectionhierarchystep.cpp \
    ct_tools/itemdrawable/ct_itemdrawablecollectionhierarchyresult.cpp \
    ct_view/itemdrawable/ct_itemdrawablehierarchycollectionwidget.cpp \
    ct_view/itemdrawable/ct_itemdrawablehierarchycollectionmodel.cpp \
    ct_view/itemdrawable/ct_itemdrawablehierarchycollectionselectionmodel.cpp \
    ct_tools/attributes/ct_abstractattributestocloudworker.cpp \
    ct_exporter/abstract/ct_abstractexporterattributesselection.cpp \
    ct_tools/itemdrawable/abstract/ct_abstractitemdrawablecollectionbuilder.cpp \
    ct_exporter/abstract/ct_abstractexporterpointattributesselection.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardpolygon2ddrawmanager.cpp \
    ct_itemdrawable/abstract/ct_abstractmetric.cpp \
    ct_itemdrawable/ct_affiliationid.cpp \
    ct_itemdrawable/abstract/ct_abstractprofile.cpp \
    ct_itemdrawable/ct_profile.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardprofiledrawmanager.cpp \
    ct_itemdrawable/ct_grid4d.cpp \
    ct_itemdrawable/abstract/ct_abstractgrid4d.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid4ddrawmanager.cpp \
    ct_reader/ct_reader_opf.cpp \
    ct_itemdrawable/ct_ttreegroup.cpp \
    ct_itemdrawable/ct_tnodegroup.cpp \
    ct_itemdrawable/abstract/ct_abstractstandarditemgroup.cpp \
    ct_global/ct_temporarylog.cpp \
    ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.cpp \
    ct_itemdrawable/ct_topfnodegroup.cpp \
    ct_itemdrawable/tools/iterator/ct_groupiterator.cpp \
    ct_itemdrawable/tools/iterator/ct_itemiterator.cpp \
    ct_itemdrawable/tools/iterator/ct_mutablegroupiterator.cpp \
    ct_itemdrawable/tools/iterator/ct_mutableitemiterator.cpp \
    ct_itemdrawable/tools/iterator/ct_containeriterator.cpp \
    ct_itemdrawable/model/outModel/ct_outopfnodegroupmodel.cpp \
    ct_result/tools/iterator/ct_resultgroupiterator.cpp \
    ct_result/tools/iterator/ct_resultitemiterator.cpp \
    ct_tools/ct_mimedata.cpp \
    ct_categories/abstract/ct_abstractcategory.cpp \
    ct_categories/ct_stdcategory.cpp \
    ct_categories/tools/ct_categorymanager.cpp \
    ct_attributes/abstract/ct_abstractitemattribute.cpp \
    ct_attributes/model/inModel/abstract/ct_inabstractattributemodel.cpp \
    ct_attributes/tools/ct_itemattributecontainer.cpp \
    ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.cpp \
    ct_model/abstract/ct_abstractmodel.cpp \
    ct_itemdrawable/model/inModel/abstract/ct_inabstractsingularitemmodel.cpp \
    ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.cpp \
    ct_itemdrawable/model/inModel/ct_instdgroupmodel.cpp \
    ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.cpp \
    ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.cpp \
    ct_itemdrawable/model/outModel/ct_outstdgroupmodel.cpp \
    ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.cpp \
    ct_attributes/model/inModel/ct_instditemattributemodel.cpp \
    ct_attributes/model/outModel/ct_outstditemattributemodel.cpp \
    ct_model/inModel/abstract/ct_inabstractmodel.cpp \
    ct_model/outModel/abstract/ct_outabstractmodel.cpp \
    ct_model/inModel/tools/ct_instdmodelpossibility.cpp \
    ct_model/inModel/tools/ct_instdmodelpossibilitygroup.cpp \
    ct_result/model/inModel/abstract/ct_inabstractresultmodel.cpp \
    ct_result/model/outModel/abstract/ct_outabstractresultmodelgroup.cpp \
    ct_attributes/tools/ct_defaultitemattributemanager.cpp \
    ct_result/model/inModel/tools/ct_instdresultmodelpossibility.cpp \
    ct_itemdrawable/abstract/ct_abstractmeshmodel.cpp \
    ct_result/tools/iterator/ct_resultiterator.cpp \
    ct_item/abstract/ct_abstractitem.cpp \
    ct_colorcloud/abstract/ct_abstractmodifiableindexcloudcolormap.cpp \
    ct_itemdrawable/model/inModel/abstract/ct_inabstractitemmodel.cpp \
    ct_model/tools/ct_modelsearchhelper.cpp \
    ct_item/tools/iterator/ct_childiterator.cpp \
    ct_itemdrawable/ct_polygon2d_old.cpp \
    ct_shapedata/ct_polygon2ddata_old.cpp \
    ct_shapedata/ct_planarbsplinedata.cpp \
    ct_itemdrawable/ct_planarbspline.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardplanarbsplinedrawmanager.cpp \
    ct_itemdrawable/ct_itemattributelist.cpp \
    ct_itemdrawable/tools/drawmanager/ct_standardmeshmodelopfdrawmanager.cpp \
    ct_itemdrawable/ct_faceattributesnormal.cpp \
    ct_itemdrawable/ct_attributeslist.cpp \
    ct_coordinates/ct_defaultcoordinatesystem.cpp \
    ct_coordinates/tools/ct_coordinatesystemmanager.cpp \
    ct_coordinates/view/ct_gdefaultcoordinatesystem.cpp \
    ct_reader/ct_reader_las.cpp \
    ct_itemdrawable/tools/ct_counter.cpp \
    ct_itemdrawable/ct_loopcounter.cpp \
    ct_step/ct_stepbeginloop.cpp \
    ct_step/ct_stependloop.cpp \
    ct_cloud/tools/abstract/ct_abstractglobalcloudmanager.cpp \
    ct_cloud/tools/ct_cloudsynctoglobalcloudmanager.cpp \
    ct_itemdrawable/ct_stdpointsattributescontainer.cpp \
    ct_itemdrawable/abstract/ct_abstractpointsattributescontainer.cpp \
    ct_itemdrawable/ct_stdlaspointsattributescontainer.cpp \
    ct_reader/tools/las/abstract/ct_abstractlaspointformat.cpp \
    ct_reader/tools/las/ct_laspointformat0.cpp \
    ct_reader/tools/las/ct_laspointinfo.cpp \
    ct_reader/tools/las/ct_laspointformat1.cpp \
    ct_reader/tools/las/ct_laspointformat2.cpp \
    ct_reader/tools/las/ct_laspointformat3.cpp \
    ct_reader/tools/las/ct_laspointformat4.cpp \
    ct_reader/tools/las/ct_laspointformat5.cpp \
    ct_reader/tools/las/ct_laspointformat6.cpp \
    ct_reader/tools/las/ct_laspointformat7.cpp \
    ct_reader/tools/las/ct_laspointformat8.cpp \
    ct_reader/tools/las/ct_laswavepacket.cpp \
    ct_reader/tools/las/ct_lascolorpacket.cpp \
    ct_reader/tools/las/ct_lasnirpacket.cpp \
    ct_reader/tools/las/ct_lasgpspacket.cpp \
    ct_reader/tools/las/ct_laspointformat9.cpp \
    ct_reader/tools/las/ct_laspointformat10.cpp \
    ct_reader/tools/las/ct_lasheader.cpp \
    ct_shape2ddata/ct_box2ddata.cpp \
    ct_shape2ddata/ct_circle2ddata.cpp \
    ct_shape2ddata/ct_line2ddata.cpp \
    ct_shape2ddata/ct_polygon2ddata.cpp \
    ct_shape2ddata/ct_shape2ddata.cpp \
    ct_shape2ddata/ct_polyline2ddata.cpp \
    ct_shape2ddata/ct_areashape2ddata.cpp \
    ct_itemdrawable/abstract/ct_abstractshape2d.cpp

HEADERS += interfaces.h \
    pluginShared_global.h \
    ct_abstractstepplugin.h \
    ct_stepseparator.h \
    ct_steploadfileseparator.h \
    ct_step/abstract/ct_abstractstep.h \
    ct_step/abstract/ct_abstractsteploadfile.h \
    ct_step/abstract/ct_virtualabstractstep.h \
    ct_step/ct_stepinitializedata.h \
    ct_result/abstract/ct_abstractresult.h \
    ct_result/ct_resultinitializedata.h \
    ct_point.h \
    ct_itemdrawable/abstract/ct_abstractitemdrawable.h \
    ct_itemdrawable/ct_scene.h \
    ct_itemdrawable/ct_itemdrawableconfiguration.h \
    ct_view/ct_stepconfigurabledialog.h \
    serialization.h \
    ct_view/ct_doublespinbox.h \
    ct_view/ct_widgetwithvaluereferenceinterface.h \
    ct_view/ct_checkbox.h \
    ct_view/ct_spinbox.h \
    ct_view/ct_radiobutton.h \
    ct_view/ct_buttongroup.h \
    ct_result/ct_resultcopymodelist.h \
    ct_pointcloud/abstract/ct_abstractpointcloud.h \
    ct_pointcloud/ct_pointcloudstdvector.h \
    ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h \
    ct_pointcloudindex/ct_pointcloudindexvector.h \
    ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h \
    ct_itemdrawable/abstract/ct_abstractitemdrawablewithoutpointcloud.h \
    ct_math/ct_mathboundingshape.h \
    ct_itemdrawable/ct_pointcluster.h \
    ct_math/ct_mathpoint.h \
    ct_itemdrawable/ct_container.h \
    ct_itemdrawable/abstract/ct_abstractshape.h \
    ct_itemdrawable/ct_circle.h \
    ct_itemdrawable/ct_ellipse.h \
    ct_itemdrawable/ct_line.h \
    ct_triangulation/ct_voronoinodet.h \
    ct_triangulation/ct_trianglet.h \
    ct_triangulation/ct_delaunayt.h \
    ct_triangulation/ct_edget.h \
    ct_triangulation/ct_nodet.h \
    ct_itemdrawable/ct_cylinder.h \
    ct_shapedata/ct_shapedata.h \
    ct_shapedata/ct_circledata.h \
    ct_shapedata/ct_cylinderdata.h \
    ct_shapedata/ct_linedata.h \
    ct_shapedata/ct_ellipsedata.h \
    ct_itemdrawable/ct_scanner.h \
    ct_itemdrawable/ct_triangulation2d.h \
    ct_math/ct_mathfittedline2d.h \
    ct_view/ct_combobox.h \
    ct_step/abstract/ct_abstractsteploadfileinscene.h \
    ct_view/ct_filechoicebutton.h \
    ct_step/abstract/ct_abstractstepserializable.h \
    ct_step/abstract/ct_abstractstepcanbeaddedfirst.h \
    ct_stepcanbeaddedfirstseparator.h \
    ct_itemdrawable/abstract/ct_abstractitemgroup.h \
    ct_result/ct_resultgroup.h \
    ct_itemdrawable/model/inModel/abstract/ct_inabstractgroupmodel.h \
    ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h \
    ct_result/model/inModel/ct_inresultmodelgroup.h \
    ct_itemdrawable/ct_standarditemgroup.h \
    ct_result/model/outModel/ct_outresultmodelgroup.h \
    ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h \
    ct_result/model/outModel/abstract/ct_outabstractresultmodel.h \
    ct_result/model/inModel/tools/ct_inresultmodelmanager.h \
    ct_turn/inTurn/ct_inturn.h \
    ct_turn/inTurn/tools/ct_inturnmanager.h \
    ct_result/tools/ct_inmanager.h \
    ct_result/model/outModel/tools/ct_outresultmodelmanager.h \
    ct_result/tools/ct_outmanager.h \
    ct_result/model/inModel/tools/ct_inresultmodelconfigurationmanager.h \
    ct_result/tools/ct_inresultmanager.h \
    ct_result/tools/ct_outresultmanager.h \
    ct_turn/outTurn/ct_outturn.h \
    ct_turn/outTurn/tools/ct_outturnmanager.h \
    ct_result/tools/ct_turnindexmanager.h \
    ct_result/tools/iterator/private/ct_treestructureforiterator.h \
    ct_itemdrawable/model/outModel/abstract/def_ct_abstractgroupmodelout.h \
    ct_view/inModel/ctg_inresultmodelpossibilities.h \
    ct_view/inModel/ctg_inresultmodelconfiguration.h \
    ct_view/inModel/ctg_inresultmodelturnchoice.h \
    ct_view/inModel/ctg_inturnmanager.h \
    ct_view/inModel/ctg_inmodelpossibilitieschoice.h \
    ct_itemdrawable/model/inModel/abstract/def_ct_abstractgroupmodelin.h \
    ct_result/model/inModel/tools/ct_inresultsbymodel.h \
    ct_result/model/inModel/ct_inresultmodelgrouptocopy.h \
    ct_result/model/outModel/ct_outresultmodelgroupcopy.h \
    ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibility.h \
    ct_result/model/outModel/tools/ct_iteratoritemsmodelforresultmodelgroup.h \
    ct_tools/model/ct_autorenamemodels.h \
    ct_shapedata/ct_boxdata.h \
    ct_itemdrawable/model/inModel/ct_inzeroormoregroupmodel.h \
    ct_tools/model/ct_generateoutmodelname.h \
    ct_itemdrawable/tools/ct_standardcontext.h \
    ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h \
    interfacessettings.h \
    ct_itemdrawable/ct_referencepoint.h \
    ct_shapedata/ct_linedata.hpp \
    ct_view/tools/ct_textfileconfigurationdialog.h \
    ct_math/ct_math2dlines.h \
    ct_math/ct_sphericalline3d.h \
    ct_itemdrawable/tools/drawmanager/abstract/ct_abstractitemdrawabledrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithpointclouddrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardcircledrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractshapedrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardellipsedrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardscannerdrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardstandarditemgroupdrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardcontainerdrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardcylinderdrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardlinedrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardpointclusterdrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardreferencepointdrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardtriangulation2ddrawmanager.h \
    ct_tools/model/abstract/ct_abstractoutmodelcopyaction.h \
    ct_tools/model/ct_generateoutresultmodelname.h \
    ct_tools/model/ct_outmodelcopyactionaddmodelgroupingroup.h \
    ct_tools/model/ct_outmodelcopyactionaddmodelitemingroup.h \
    ct_tools/model/ct_outmodelcopyactionremovemodelgroupingroup.h \
    ct_tools/model/ct_outmodelcopyactionremovemodelitemingroup.h \
    ct_result/model/outModel/tools/ct_iteratorgroupsmodelforresultmodelgroup.h \
    ct_view/ct_lineedit.h \
    ct_math/ct_math.h \
    ct_itemdrawable/ct_grid3d.hpp \
    ct_itemdrawable/ct_grid3d.h \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid3ddrawmanager.hpp \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid3ddrawmanager.h \
    ct_itemdrawable/ct_beam.h \
    ct_itemdrawable/tools/drawmanager/ct_standardbeamdrawmanager.h \
    ct_itemdrawable/tools/gridtools/ct_grid3dwootraversalalgorithm.h \
    ct_itemdrawable/tools/gridtools/ct_abstractgrid3dbeamvisitor.h \
    ct_global/ct_context.h \
    ct_global/ct_repository.h \
    ct_itemdrawable/tools/ct_itemplateddata2darray.h \
    ct_itemdrawable/tools/ct_itemplateddata1darray.h \
    ct_tools/ct_monitoredqthread.h \
    ct_itemdrawable/tools/ct_itemplateddata3darray.h \
    ct_pointcloudindex/ct_pointcloudindexlessmemory.h \
    ct_global/ct_repositorymanager.h \
    ct_pointcloud/tools/abstract/ct_abstractundefinedsizepointcloud.h \
    ct_pointcloud/tools/ct_standardundefinedsizepointcloud.h \
    ct_reader/abstract/ct_abstractreader.h \
    ct_reader/ct_reader_xyb.h \
    ct_itemdrawable/abstract/ct_abstractpointsattributes.h \
    ct_itemdrawable/ct_pointsattributesscalartemplated.h \
    ct_itemdrawable/ct_pointsattributesscalartemplated.hpp \
    ct_colorcloud/abstract/ct_abstractcolorcloud.h \
    ct_colorcloud/ct_colorcloudstdvector.h \
    ct_colorcloud/registered/ct_standardcolorcloudregistered.h \
    ct_itemdrawable/ct_pointsattributescolor.h \
    ct_itemdrawable/ct_pointsattributesnormal.h \
    ct_itemdrawable/tools/drawmanager/ct_standardabstractpointsattributesdrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardpointsattributesnormaldrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardpointsattributescolordrawmanager.h \
    ct_normalcloud/ct_normalcloudstdvector.h \
    ct_normalcloud/abstract/ct_abstractnormalcloud.h \
    ct_normal.h \
    ct_color.h \
    ct_reader/ct_reader_ascrgb.h \
    ct_actions/abstract/ct_abstractaction.h \
    ct_actions/abstract/ct_abstractactionforgraphicsview.h \
    ct_actions/abstract/ct_abstractactionfortreeview.h \
    ct_actions/ct_actionsseparator.h \
    ct_view/actions/abstract/ct_gabstractactionoptions.h \
    ct_itemdrawable/tools/pointclustertools/ct_polylinesalgorithms.h \
    ct_normalcloud/registered/ct_standardnormalcloudregistered.h \
    ct_reader/ct_reader_larchitect_grid.h \
    ct_itemdrawable/abstract/ct_abstractgrid3d.h \
    ct_itemdrawable/abstract/ct_abstractgrid2d.h \
    ct_itemdrawable/ct_grid2dxy.hpp \
    ct_itemdrawable/ct_grid2dxy.h \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid2dxydrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid2dxydrawmanager.hpp \
    ct_itemdrawable/abstract/ct_virtualgrid2d.h \
    ct_itemdrawable/abstract/ct_virtualgrid2d.hpp \
    ct_itemdrawable/ct_grid2dyz.hpp \
    ct_itemdrawable/ct_grid2dyz.h \
    ct_itemdrawable/ct_grid2dxz.hpp \
    ct_itemdrawable/ct_grid2dxz.h \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid2dyzdrawmanager.hpp \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid2dyzdrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid2dxzdrawmanager.hpp \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid2dxzdrawmanager.h \
    ct_mesh/ct_face.h \
    ct_mesh/ct_edge.h \
    ct_mesh/ct_mesh.h \
    ct_cloud/abstract/ct_abstractcloud.h \
    ct_cloud/registered/abstract/ct_abstractcloudregistered.h \
    ct_cloud/ct_standardcloudstdvectort.h \
    ct_cloud/ct_standardcloudstdvectort.hpp \
    ct_cloudindex/abstract/ct_abstractcloudindex.h \
    ct_cloudindex/ct_cloudindexlessmemoryt.h \
    ct_cloudindex/ct_cloudindexstdvectort.h \
    ct_cloudindex/abstract/ct_abstractcloudindext.h \
    ct_cloudindex/abstract/ct_abstractmodifiablecloudindext.h \
    ct_cloudindex/ct_cloudindexlessmemoryt.hpp \
    ct_cloudindex/ct_cloudindexstdvectort.hpp \
    ct_cloudindex/abstract/ct_abstractcloudindext.hpp \
    ct_cloud/abstract/ct_abstractcloudt.h \
    ct_cloud/tools/ct_globalcloudmanagert.h \
    ct_cloud/tools/abstract/ct_abstractcloudsynctoglobalcloudmanager.h \
    ct_cloudindex/registered/abstract/ct_abstractcloudindexregistered.h \
    ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h \
    ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.hpp \
    ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregisteredt.h \
    ct_cloudindex/registered/abstract/ct_abstractnotmodifiablecloudindexregisteredt.h \
    ct_cloudindex/tools/ct_cloudindexregistrationmanagert.h \
    ct_cloudindex/registered/ct_standardmodifiablecloudindexregisteredt.h \
    ct_cloudindex/registered/ct_standardnotmodifiablecloudindexregisteredt.h \
    ct_cloudindex/registered/ct_standardnotmodifiablecloudindexregisteredt.hpp \
    ct_cloudindex/registered/ct_standardmodifiablecloudindexregisteredt.hpp \
    ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregisteredt.hpp \
    ct_cloudindex/tools/ct_cloudindexregistrationmanagert.hpp \
    ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanager.h \
    ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h \
    ct_cloudindex/registered/abstract/ct_abstractnotmodifiablecloudindexregisteredt.hpp \
    ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.hpp \
    ct_cloud/tools/ct_globalcloudmanagert.hpp \
    ct_cloud/tools/ct_globalpointcloudmanager.h \
    ct_cloud/tools/abstract/ct_abstractglobalcloudmanagert.h \
    ct_cloud/tools/abstract/ct_abstractglobalcloudmanagert.hpp \
    ct_mesh/tools/ct_meshallocatort.h \
    ct_mesh/tools/ct_meshallocatort.hpp \
    ct_cloudindex/tools/ct_modifiablecloudindexiteratort.h \
    ct_itemdrawable/ct_meshmodel.h \
    ct_itemdrawable/tools/drawmanager/ct_standardmeshmodeldrawmanager.h \
    ct_mesh/cloud/abstract/ct_abstractfacecloudindex.h \
    ct_mesh/cloud/abstract/ct_abstractedgecloudindex.h \
    ct_mesh/cloud/ct_facecloudindexlessmemory.h \
    ct_mesh/cloud/ct_facecloudindexvector.h \
    ct_mesh/cloud/ct_edgecloudindexlessmemory.h \
    ct_mesh/cloud/ct_edgecloudindexvector.h \
    ct_itemdrawable/abstract/ct_abstractattributes.h \
    ct_attributes/ct_attributescolor.h \
    ct_attributes/ct_attributesnormal.h \
    ct_attributes/ct_attributesscalart.h \
    ct_attributes/ct_attributesscalart.hpp \
    ct_attributes/abstract/ct_abstractattributesscalar.h \
    ct_itemdrawable/abstract/ct_abstractfaceattributes.h \
    ct_itemdrawable/abstract/ct_abstractedgeattributes.h \
    ct_itemdrawable/ct_faceattributescolor.h \
    ct_itemdrawable/ct_faceattributesnormal.h \
    ct_itemdrawable/ct_faceattributesscalart.h \
    ct_itemdrawable/ct_edgeattributescolor.h \
    ct_itemdrawable/ct_edgeattributesnormal.h \
    ct_itemdrawable/ct_edgeattributesscalart.h \
    ct_itemdrawable/ct_edgeattributesscalart.hpp \
    ct_itemdrawable/ct_faceattributesscalart.hpp \
    ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h \
    ct_itemdrawable/abstract/ct_abstractfaceattributesscalar.h \
    ct_itemdrawable/abstract/ct_abstractedgeattributesscalar.h \
    ct_cloud/tools/ct_globaledgecloudmanager.h \
    ct_cloud/tools/ct_globalfacecloudmanager.h \
    ct_cloud/tools/iglobalcloudlistener.h \
    ct_cloudindex/tools/ct_cloudindexiteratort.h \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid2dxzdrawmanager.h \
    ct_itemdrawable/tools/gridtools/ct_grid3dbeamvisitor_indexlist.h \
    ct_log/ct_basicostreamloglistener.h \
    ct_log/ct_fileloglistener.h \
    ct_log/abstract/ct_abstractloglistener.h \
    ct_exporter/ct_standardexporterseparator.h \
    ct_exporter/abstract/ct_abstractexporter.h \
    ct_exporter/abstract/private/ct_abstractexporter_p.h \
    ct_exporter/private/ct_standardexporterseparator_p.h \
    ct_itemdrawable/accessibility/ct_iaccesspointcloud.h \
    ct_reader/ct_standardreaderseparator.h \
    ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h \
    ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.hpp \
    ct_tools/itemdrawable/ct_itemdrawablecollectionhierarchystep.h \
    ct_tools/itemdrawable/ct_itemdrawablecollectionhierarchyresult.h \
    ct_view/itemdrawable/ct_itemdrawablehierarchycollectionwidget.h \
    ct_view/itemdrawable/ct_itemdrawablehierarchycollectionmodel.h \
    ct_view/itemdrawable/ct_itemdrawablehierarchycollectionselectionmodel.h \
    ct_tools/attributes/ct_attributestocloudworkert.h \
    ct_tools/attributes/ct_attributestocloudworkert.hpp \
    ct_tools/attributes/ct_abstractattributestocloudworker.h \
    ct_exporter/abstract/ct_abstractexporterattributesselection.h \
    ct_tools/itemdrawable/abstract/ct_abstractitemdrawablecollectionbuilder.h \
    ct_exporter/abstract/ct_abstractexporterpointattributesselection.h \
    ct_tools/ct_numerictostringconversiont.h \
    ct_tools/ct_stringtonumericconversion.h \
    ct_itemdrawable/tools/drawmanager/ct_standardpolygon2ddrawmanager.h \
    ct_itemdrawable/abstract/ct_abstractmetric.h \
    ct_itemdrawable/ct_metrict.h \
    ct_itemdrawable/ct_metrict.hpp \
    ct_itemdrawable/ct_affiliationid.h \
    ct_itemdrawable/abstract/ct_abstractprofile.h \
    ct_itemdrawable/ct_profile.h \
    ct_itemdrawable/ct_profile.hpp \
    ct_itemdrawable/tools/drawmanager/ct_standardprofiledrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardprofiledrawmanager.hpp \
    ct_itemdrawable/ct_grid4d.h \
    ct_itemdrawable/ct_grid4d.hpp \
    ct_itemdrawable/abstract/ct_abstractgrid4d.h \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid4ddrawmanager.h \
    ct_itemdrawable/tools/drawmanager/ct_standardgrid4ddrawmanager.hpp \
    ct_itemdrawable/tools/ct_itemplateddata4darray.h \
    ct_reader/ct_reader_opf.h \
    ct_itemdrawable/ct_ttreegroup.h \
    ct_itemdrawable/ct_tnodegroup.h \
    ct_itemdrawable/abstract/ct_abstractstandarditemgroup.h \
    ct_global/ct_temporarylog.h \
    ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h \
    ct_itemdrawable/ct_topfnodegroup.h \
    ct_itemdrawable/tools/iterator/ct_groupiterator.h \
    ct_itemdrawable/tools/iterator/ct_itemiterator.h \
    ct_itemdrawable/tools/iterator/ct_mutablegroupiterator.h \
    ct_itemdrawable/tools/iterator/ct_mutableitemiterator.h \
    ct_itemdrawable/tools/iterator/ct_containeriterator.h \
    ct_itemdrawable/model/outModel/ct_outopfnodegroupmodel.h \
    ct_result/tools/iterator/ct_resultgroupiterator.h \
    ct_result/tools/iterator/ct_resultitemiterator.h \
    ct_cloudindex/ct_cloudindexstdlistt.h \
    ct_cloudindex/ct_cloudindexstdlistt.hpp \
    ct_cloudindex/ct_cloudindexstdmapt.h \
    ct_cloudindex/ct_cloudindexstdmapt.hpp \
    ct_colorcloud/ct_indexcloudcolorstdmapt.h \
    ct_colorcloud/ct_indexcloudcolorstdmapt.hpp \
    ct_tools/ct_mimedata.h \
    ct_categories/abstract/ct_abstractcategory.h \
    ct_categories/ct_stdcategory.h \
    ct_categories/tools/ct_categorymanager.h \
    ct_attributes/abstract/ct_abstractitemattribute.h \
    ct_attributes/ct_stditemattributet.h \
    ct_attributes/ct_stditemattributet.hpp \
    ct_attributes/ct_stditemattributewrappert.h \
    ct_attributes/ct_stditemattributewrappert.hpp \
    ct_itemdrawable/model/inModel/abstract/def_ct_abstractitemdrawablemodelin.h \
    ct_itemdrawable/model/outModel/abstract/def_ct_abstractitemdrawablemodelout.h \
    ct_attributes/tools/ct_iacreator.h \
    ct_attributes/abstract/ct_abstractitemattributet.h \
    ct_attributes/abstract/ct_abstractitemattributet.hpp \
    ct_attributes/tools/ct_itemattributecontainer.h \
    ct_itemdrawable/model/abstract/ct_abstractgroupmodelt.h \
    ct_itemdrawable/model/abstract/ct_abstractgroupmodelt.hpp \
    ct_attributes/model/outModel/abstract/ct_outabstractitemattributemodel.h \
    ct_attributes/model/inModel/abstract/ct_inabstractitemattributemodel.h \
    ct_model/abstract/ct_abstractmodel.h \
    ct_itemdrawable/model/abstract/ct_abstractsingularitemmodelt.h \
    ct_itemdrawable/model/abstract/ct_abstractsingularitemmodelt.hpp \
    ct_itemdrawable/model/inModel/abstract/ct_inabstractsingularitemmodel.h \
    ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.h \
    ct_itemdrawable/model/inModel/ct_instdgroupmodel.h \
    ct_itemdrawable/model/outModel/abstract/ct_outabstractitemmodel.h \
    ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h \
    ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h \
    ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h \
    ct_attributes/model/inModel/abstract/def_ct_inabstractitemattributemodel.h \
    ct_attributes/model/outModel/abstract/def_ct_outabstractitemattributemodel.h \
    ct_attributes/model/abstract/ct_abstractitemattributemodelt.h \
    ct_attributes/model/abstract/ct_abstractitemattributemodelt.hpp \
    ct_attributes/model/inModel/ct_instditemattributemodel.h \
    ct_attributes/model/outModel/ct_outstditemattributemodel.h \
    ct_model/inModel/abstract/ct_inabstractmodel.h \
    ct_model/outModel/abstract/ct_outabstractmodel.h \
    ct_model/inModel/tools/ct_instdmodelpossibility.h \
    ct_model/inModel/tools/ct_instdmodelpossibilitygroup.h \
    ct_result/model/inModel/abstract/ct_inabstractresultmodel.h \
    ct_result/model/outModel/abstract/ct_outabstractresultmodelgroup.h \
    ct_attributes/tools/ct_defaultitemattributemanager.h \
    ct_itemdrawable/accessibility/ct_iaccessfacecloud.h \
    ct_itemdrawable/accessibility/ct_iaccessedgecloud.h \
    ct_cloudindex/abstract/ct_abstractmodifiablecloudindex.h \
    ct_result/model/inModel/tools/ct_instdresultmodelpossibility.h \
    ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregistered.h \
    ct_itemdrawable/abstract/ct_abstractmeshmodel.h \
    ct_result/tools/iterator/ct_resultiterator.h \
    ct_item/abstract/ct_abstractitem.h \
    ct_colorcloud/abstract/ct_abstractmodifiableindexcloudcolormap.h \
    ct_itemdrawable/model/inModel/abstract/ct_inabstractitemmodel.h \
    ct_model/tools/ct_modelsearchhelper.h \
    ct_item/tools/iterator/ct_childiterator.h \
    ct_itemdrawable/ct_polygon2d_old.h \
    ct_shapedata/ct_polygon2ddata_old.h \
    ct_shapedata/ct_planarbsplinedata.h \
    ct_itemdrawable/ct_planarbspline.h \
    ct_itemdrawable/tools/drawmanager/ct_standardplanarbsplinedrawmanager.h \
    ct_itemdrawable/ct_itemattributelist.h \
    rapidxml/rapidxml.hpp \
    rapidxml/rapidxml_iterators.hpp \
    rapidxml/rapidxml_print.hpp \
    rapidxml/rapidxml_utils.hpp \
    ct_itemdrawable/tools/drawmanager/ct_standardmeshmodelopfdrawmanager.h \
    ct_itemdrawable/ct_attributeslist.h \
    ct_coordinates/abstract/ct_abstractcoordinatesystem.h \
    ct_coordinates/ct_defaultcoordinatesystem.h \
    ct_coordinates/tools/ct_coordinatesystemmanager.h \
    ct_coordinates/view/ct_gdefaultcoordinatesystem.h \
    ct_attributes/ct_stditemattributecoordinatewrappert.h \
    ct_attributes/ct_stditemattributecoordinatewrappert.hpp \
    ct_attributes/ct_stditemattributecoordinatet.h \
    ct_attributes/ct_stditemattributecoordinatet.hpp \
    ct_reader/ct_reader_las.h \
    ct_reader/ct_reader_opf_def_models.h \
    ct_reader/ct_reader_ascrgb_def_models.h \
    ct_reader/ct_reader_xyb_def_models.h \
    ct_reader/ct_reader_larchitect_grid_def_models.h \
    ct_reader/ct_reader_las_def_models.h \
    ct_itemdrawable/tools/ct_counter.h \
    ct_itemdrawable/ct_loopcounter.h \
    ct_step/ct_stepbeginloop.h \
    ct_step/ct_stependloop.h \
    ct_cloud/tools/abstract/ct_abstractglobalcloudmanager.h \
    ct_cloud/tools/ct_cloudsynctoglobalcloudmanager.h \
    ct_cloud/registered/ct_stdcloudregisteredt.h \
    ct_cloud/registered/ct_stdcloudregisteredt.hpp \
    ct_itemdrawable/ct_pointsattributesscalarmaskt.h \
    ct_itemdrawable/ct_pointsattributesscalarmaskt.hpp \
    ct_itemdrawable/ct_stdpointsattributescontainer.h \
    ct_itemdrawable/abstract/ct_abstractpointsattributescontainer.h \
    ct_itemdrawable/ct_stdlaspointsattributescontainer.h \
    ct_itemdrawable/ct_stdlaspointsattributescontainer.h \
    ct_reader/tools/las/abstract/ct_abstractlaspointformat.h \
    ct_reader/tools/las/ct_laspointformat0.h \
    ct_reader/tools/las/ct_lasdefine.h \
    ct_reader/tools/las/ct_laspointinfo.h \
    ct_reader/tools/las/ct_laspointformat1.h \
    ct_reader/tools/las/ct_laspointformat2.h \
    ct_reader/tools/las/ct_laspointformat3.h \
    ct_reader/tools/las/ct_laspointformat4.h \
    ct_reader/tools/las/ct_laspointformat5.h \
    ct_reader/tools/las/ct_laspointformat6.h \
    ct_reader/tools/las/ct_laspointformat7.h \
    ct_reader/tools/las/ct_laspointformat8.h \
    ct_reader/tools/las/ct_laswavepacket.h \
    ct_reader/tools/las/ct_lascolorpacket.h \
    ct_reader/tools/las/ct_lasnirpacket.h \
    ct_reader/tools/las/ct_lasgpspacket.h \
    ct_reader/tools/las/ct_laspointformat9.h \
    ct_reader/tools/las/ct_laspointformat10.h \
    ct_reader/tools/las/ct_lasheader.h \
    ct_shape2ddata/ct_box2ddata.h \
    ct_shape2ddata/ct_circle2ddata.h \
    ct_shape2ddata/ct_line2ddata.h \
    ct_shape2ddata/ct_polygon2ddata.h \
    ct_shape2ddata/ct_shape2ddata.h \
    ct_shape2ddata/ct_polyline2ddata.h \
    ct_shape2ddata/ct_areashape2ddata.h \
    ct_itemdrawable/abstract/ct_abstractshape2d.h


INCLUDEPATH += .
INCLUDEPATH += ./ct_actions
INCLUDEPATH += ./ct_actions/abstract
INCLUDEPATH += ./ct_attributes
INCLUDEPATH += ./ct_attributes/abstract
INCLUDEPATH += ./ct_attributes/model
INCLUDEPATH += ./ct_attributes/model/abstract
INCLUDEPATH += ./ct_attributes/model/inModel
INCLUDEPATH += ./ct_attributes/model/inModel/abstract
INCLUDEPATH += ./ct_attributes/model/outModel
INCLUDEPATH += ./ct_attributes/model/outModel/abstract
INCLUDEPATH += ./ct_attributes/tools
INCLUDEPATH += ./ct_step
INCLUDEPATH += ./ct_step/abstract
INCLUDEPATH += ./ct_result
INCLUDEPATH += ./ct_result/abstract
INCLUDEPATH += ./ct_result/model
INCLUDEPATH += ./ct_result/model/abstract
INCLUDEPATH += ./ct_result/model/inModel
INCLUDEPATH += ./ct_result/model/outModel
INCLUDEPATH += ./ct_result/model/inModel/abstract
INCLUDEPATH += ./ct_result/model/outModel/abstract
INCLUDEPATH += ./ct_result/model/inModel/tools
INCLUDEPATH += ./ct_result/model/outModel/tools
INCLUDEPATH += ./ct_result/tools
INCLUDEPATH += ./ct_itemdrawable
INCLUDEPATH += ./ct_itemdrawable/abstract
INCLUDEPATH += ./ct_itemdrawable/tools
INCLUDEPATH += ./ct_itemdrawable/tools/gridtools
INCLUDEPATH += ./ct_itemdrawable/tools/pointclustertools
INCLUDEPATH += ./ct_itemdrawable/model/abstract
INCLUDEPATH += ./ct_itemdrawable/model/inModel
INCLUDEPATH += ./ct_itemdrawable/model/outModel
INCLUDEPATH += ./ct_itemdrawable/model/inModel/abstract
INCLUDEPATH += ./ct_itemdrawable/model/outModel/abstract
INCLUDEPATH += ./ct_itemdrawable/model/inModel/tools
INCLUDEPATH += ./ct_view
INCLUDEPATH += ./ct_view/inModel
INCLUDEPATH += ./ct_view/tools
INCLUDEPATH += ./ct_math
INCLUDEPATH += ./ct_triangulation
INCLUDEPATH += ./ct_shapedata
INCLUDEPATH += ./ct_shapedata2d
INCLUDEPATH += ./ct_tools
INCLUDEPATH += ./ct_tools/model

FORMS += \
    ct_view/inModel/ctg_inresultmodelpossibilities.ui \
    ct_view/inModel/ctg_inresultmodelconfiguration.ui \
    ct_view/inModel/ctg_inresultmodelturnchoice.ui \
    ct_view/inModel/ctg_inturnmanager.ui \
    ct_view/inModel/ctg_inmodelpossibilitieschoice.ui \
    ct_view/tools/ct_textfileconfigurationdialog.ui \
    ct_view/itemdrawable/ct_itemdrawablehierarchycollectionwidget.ui \
    ct_coordinates/view/ct_gdefaultcoordinatesystem.ui

TRANSLATIONS += languages/pluginshared_fr.ts \
                languages/pluginshared_en.ts

RESOURCES +=

macx {
    exists($${OUT_PWD}/scripts/postprocess_osx.sh) {
        QMAKE_POST_LINK = $${OUT_PWD}/scripts/postprocess_osx.sh
    } else {
        message(optionnal scripts not found)
    }
} else {
    unix{
        exists($${OUT_PWD}/scripts/postprocess_unix.sh) {
            QMAKE_POST_LINK = $${OUT_PWD}/scripts/postprocess_unix.sh
        }else {
            message(optionnal scripts not found)
        }
    }

    win32 {
        exists($${OUT_PWD}\\scripts\\postprocess_win.bat) {
            QMAKE_POST_LINK = $${OUT_PWD}\\scripts\\postprocess_win.bat
        } else {
            message(optionnal scripts not found)
        }
    }
}
