#ifndef CT_ABSTRACTEXPORTERATTRIBUTESSELECTION_H
#define CT_ABSTRACTEXPORTERATTRIBUTESSELECTION_H

#include "ct_exporter/abstract/ct_abstractexporter.h"
#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionmodel.h"
#include "ct_tools/itemdrawable/abstract/ct_abstractitemdrawablecollectionbuilder.h"
#include "ct_view/itemdrawable/ct_itemdrawablehierarchycollectionwidget.h"

/**
 * @brief Exporter that have predefined method to search other ItemDrawable ot Attributes like CT_AbstractPointsAttributes, CT_AbstractFaceAttributes, etc...
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractExporterAttributesSelection : public CT_AbstractExporter
{
public:
    CT_AbstractExporterAttributesSelection();

    /**
     * @brief By default call selectAttributes()
     */
    virtual bool configureExport();

    /**
     * @brief Call this method to show the dialog where the user can select attributes
     */
    bool selectAttributes();

    /**
     * @brief Returns the cloud type to use (points, faces, edges or none)
     */
    virtual CT_ItemDrawableHierarchyCollectionWidget::CloudType cloudType() const = 0;

protected:

    /**
     * @brief Return the step where the research for attributes must begin
     */
    virtual CT_VirtualAbstractStep* rootStep() const;

    /**
     * @brief Build the collection of attributes for the selection widget
     */
    void buildAttributesCollection();

    /**
     * @brief Get the builder who must search attributes or ItemDrawable. Per example a CT_ItemDrawableCollectionBuilderT<CT_AbstractPointsAttributes>
     *        and a CT_ItemDrawableCollectionBuilderT<CT_Scanner>
     *
     *        The first element of the QPair is the name that you want to show in the tree view for the results of the builder (second element of the QPair)
     */
    virtual QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > getBuilders() const = 0;

    /**
     * @brief Configure the builder after the call of "buildFrom" method. Per example if you want only keeps
     *        special types of attributes.
     */
    virtual void postConfigureAttributesBuilder(CT_AbstractItemDrawableCollectionBuilder *builder) { Q_UNUSED(builder) }

    /**
     * @brief Return the model to pass to the CT_ItemDrawableHierarchyCollectionWidget
     */
    CT_ItemDrawableHierarchyCollectionModel* buildModel() const;

    /**
     * @brief You must call method addExcludeModel of the model to configure items that must be excluded
     */
    virtual void setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const = 0;

    /**
     * @brief Use elements selected. Return false if you want to cancel the configuration.
     */
    virtual bool useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget) = 0;

    /**
     * @brief Call this method in your constructor to enable/disable the use of document's colors in the selector (dialog)
     */
    void setCanExportWithColors(bool enable);

    /**
     * @brief Call this method in your constructor to enable/disable the use of document's normals in the selector (dialog)
     */
    void setCanExportWithNormals(bool enable);

    /**
     * @brief Return true if this exporter can export colors information
     */
    bool canExportWithColors() const;

    /**
     * @brief Return true if this exporter can export normals information
     */
    bool canExportWithNormals() const;

private:
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> >  m_buildersResults;
    bool                                                                m_canSelectColors;
    bool                                                                m_canSelectNormals;
};

#endif // CT_ABSTRACTEXPORTERATTRIBUTESSELECTION_H
