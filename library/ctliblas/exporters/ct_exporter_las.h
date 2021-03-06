#ifndef CT_EXPORTER_LAS_H
#define CT_EXPORTER_LAS_H

#include "ctliblas/ctliblas_global.h"
#include "ctliblas/itemdrawable/las/ct_stdlaspointsattributescontainer.h"
#include "ctliblas/tools/las/abstract/ct_abstractlaspointformat.h"

// from pluginshared
#include "ct_exporter/abstract/ct_abstractexporterattributesselection.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"

class CTLIBLAS_EXPORT CT_Exporter_LAS : public CT_AbstractExporterAttributesSelection
{
    Q_OBJECT

public:
    CT_Exporter_LAS();

    virtual QString getExporterCustomName() const;

    CT_StepsMenu::LevelPredefined getExporterSubMenuName() const;

    void init();

    bool setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list);
    bool setPointsToExport(const QList<CT_AbstractCloudIndex*> &list);

    SettingsNodeGroup* saveExportConfiguration() const;
    bool loadExportConfiguration(const SettingsNodeGroup *root);

    virtual CT_ItemDrawableHierarchyCollectionWidget::CloudType cloudType() const;
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > getBuilders() const;
    void setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const;
    bool useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget);

    virtual CT_AbstractExporter* copy() const;

protected:

    bool protectedExportToFile();
    void clearWorker();

    /**
     * @brief Write the header of the file
     * @param stream : the data stream of the file (it will be seek to 0 automatically and set to LittleEndian)
     * @param rn : the points attributes that represent for each point the "return number"
     * @param indexes : the list of all indexes that will be exported
     * @param nPoints : will be filled with the total number of points to write (sum of size of index in indexes)
     * @return NULL if it was an error otherwiser the header created (don't forget to delete it from memory)
     */
    CT_LASHeader* writeHeader(QDataStream &stream,
                              const CT_AbstractPointAttributesScalar *rn,
                              const QList<CT_AbstractPointCloudIndex *> &indexes,
                              size_t &nPoints);

    /**
     * @brief Return the point data format to use (check m_lasContainer to know where attributes is in the container and select the appropriate format)
     */
    int getPointDataFormat() const;

    /**
     * @brief Return the length of a point in bytes (use the method "getPointDataFormat" if optFormat == -1, otherwise pass the format to this method)
     */
    size_t getPointDataLength(const int &optFormat = -1) const;

    /**
     * @brief Create a point data format (tools to export)
     */
    CT_AbstractLASPointFormat* createPointDataFormat(const int &optFormat = -1) const;

private:
    CT_ItemDrawableCollectionBuilderT<CT_StdLASPointsAttributesContainer>   m_lasAttributesContainerBuilder;
    CT_StdLASPointsAttributesContainer                                      *m_lasContainer;
    CT_OutAbstractSingularItemModel                                         *m_lasContainerModel;
};

#endif // CT_EXPORTER_LAS_H
