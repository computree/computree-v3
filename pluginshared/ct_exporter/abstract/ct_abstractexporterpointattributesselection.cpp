#include "ct_abstractexporterpointattributesselection.h"

CT_AbstractExporterPointAttributesSelection::CT_AbstractExporterPointAttributesSelection() : CT_AbstractExporterAttributesSelection()
{
}

bool CT_AbstractExporterPointAttributesSelection::configureExport()
{
    clearWorker();

    return selectAttributes();
}

CT_ItemDrawableHierarchyCollectionWidget::CloudType CT_AbstractExporterPointAttributesSelection::cloudType() const
{
    return CT_ItemDrawableHierarchyCollectionWidget::POINTS;
}

QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > CT_AbstractExporterPointAttributesSelection::getBuilders() const
{
    QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > l;
    l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(QObject::tr("Attributs"), (CT_AbstractItemDrawableCollectionBuilder*)&m_attributsBuilder);

    return l;
}

void CT_AbstractExporterPointAttributesSelection::postConfigureAttributesBuilder(CT_AbstractItemDrawableCollectionBuilder *builder)
{
    if((&m_attributsBuilder) == builder)
        m_attributsBuilder.onlyKeepsTheseTypesInCollection2<CT_AttributesColor, CT_AbstractAttributesScalar>();
}

void CT_AbstractExporterPointAttributesSelection::setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder *> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const
{
    // attributes
    if((&m_attributsBuilder) == pair.second)
    {
        model->addExcludeModel(0); // exclude me
        model->addExcludeModel(-1); // exclude documents colors
    }
}

bool CT_AbstractExporterPointAttributesSelection::useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget)
{
    m_attributsWorker.setColorCloud(selectorWidget->colorCloudSelected());

    QList<CT_AbstractPointsAttributes*> attributes;

    QList<CT_AbstractSingularItemDrawable*> list = selectorWidget->itemDrawableSelected();
    QListIterator<CT_AbstractSingularItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractSingularItemDrawable *item = it.next();
        CT_AbstractPointsAttributes *pa = dynamic_cast<CT_AbstractPointsAttributes*>(item);

        if(pa != NULL)
            attributes.append(dynamic_cast<CT_AbstractPointsAttributes*>(item));
    }

    m_attributsWorker.setAttributes(attributes);

    return true;
}

CT_AbstractColorCloud* CT_AbstractExporterPointAttributesSelection::createColorCloudBeforeExportToFile()
{
    CT_AbstractColorCloud *cc = NULL;

    if((m_attributsWorker.colorCloud().data() == NULL)
            && (!m_attributsWorker.attributes().isEmpty()))
    {
        m_attributsWorker.setColorCloud(PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud));
        m_attributsWorker.apply();
        cc = m_attributsWorker.colorCloud()->abstractColorCloud();
    }
    else if(m_attributsWorker.colorCloud().data() != NULL)
    {
        cc = m_attributsWorker.colorCloud()->abstractColorCloud();
    }

    return cc;
}

void CT_AbstractExporterPointAttributesSelection::clearWorker()
{
    m_attributsWorker.setAttributes(QList<CT_AbstractPointsAttributes*>());
    m_attributsWorker.setColorCloud(QSharedPointer<CT_StandardColorCloudRegistered>(NULL));
}
