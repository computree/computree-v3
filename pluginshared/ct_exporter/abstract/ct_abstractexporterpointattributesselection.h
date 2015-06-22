#ifndef CT_ABSTRACTEXPORTERPOINTATTRIBUTESSELECTION_H
#define CT_ABSTRACTEXPORTERPOINTATTRIBUTESSELECTION_H

#include "ct_exporter/abstract/ct_abstractexporterattributesselection.h"
#include "ct_tools/attributes/ct_attributestocloudworkert.h"
#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"
#include "ct_itemdrawable/abstract/ct_abstractpointsattributes.h"

/**
 * @brief Inherits from this class if you want that your exporter can use attributes for points (like colors, etc...)
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractExporterPointAttributesSelection : public CT_AbstractExporterAttributesSelection
{
public:
    CT_AbstractExporterPointAttributesSelection();

    virtual bool configureExport();

    virtual CT_ItemDrawableHierarchyCollectionWidget::CloudType cloudType() const;
    virtual QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > getBuilders() const;
    virtual void postConfigureAttributesBuilder(CT_AbstractItemDrawableCollectionBuilder *builder);
    virtual void setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const;
    virtual bool useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget);

protected:
    CT_AttributesToCloudWorkerT<CT_AbstractPointsAttributes>                            m_attributsColorPointWorker;
    CT_AttributesToCloudWorkerT<CT_AbstractPointsAttributes>                            m_attributsNormalPointWorker;

    QList<CT_OutAbstractSingularItemModel*>                                             m_attributesColorModel;
    QList<CT_OutAbstractSingularItemModel*>                                             m_attributesNormalModel;

    CT_ItemDrawableCollectionBuilderT<CT_AbstractPointsAttributes>                      m_attributsBuilderColors;
    CT_ItemDrawableCollectionBuilderT<CT_AbstractPointsAttributes>                      m_attributsBuilderNormals;

    /**
     * @brief You must call this method to create the color cloud. Call this method in the beginning of your method "protectedExportToFile()". It will
     *        return the color cloud that you must use (WARNING : the color cloud returned can be NULL ! if the user don't select a point attributes)
     */
    CT_AbstractColorCloud* createColorCloudBeforeExportToFile();

    /**
     * @brief You must call this method to create the normal cloud. Call this method in the beginning of your method "protectedExportToFile()". It will
     *        return the normal cloud that you must use (WARNING : the normal cloud returned can be NULL ! if the user don't select a point attributes)
     */
    CT_AbstractNormalCloud* createNormalCloudBeforeExportToFile();

    /**
     * @brief You must call this method before you quit the method "protectedExportToFile()"
     */
    virtual void clearWorker();
};

#endif // CT_ABSTRACTEXPORTERPOINTATTRIBUTESSELECTION_H
