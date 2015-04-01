#include "ct_abstractexporterpointattributesselection.h"

#include "ct_itemdrawable/ct_pointsattributescolor.h"
#include "ct_itemdrawable/ct_pointsattributesnormal.h"
#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"

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
    l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(QObject::tr("Colors"), (CT_AbstractItemDrawableCollectionBuilder*)&m_attributsBuilderColors);
    l << QPair<QString, CT_AbstractItemDrawableCollectionBuilder*>(QObject::tr("Normals"), (CT_AbstractItemDrawableCollectionBuilder*)&m_attributsBuilderNormals);

    return l;
}

void CT_AbstractExporterPointAttributesSelection::postConfigureAttributesBuilder(CT_AbstractItemDrawableCollectionBuilder *builder)
{
    if((&m_attributsBuilderColors) == builder) {

        if(canExportWithColors())
            m_attributsBuilderColors.onlyKeepsTheseTypesInCollection2<CT_AttributesColor, CT_AbstractAttributesScalar>();
        else
            m_attributsBuilderColors.onlyKeepsTheseTypesInCollection1<CT_Nothing1>();

    } else if((&m_attributsBuilderNormals) == builder) {

        if(canExportWithNormals())
            m_attributsBuilderNormals.onlyKeepsTheseTypesInCollection1<CT_AttributesNormal>();
        else
            m_attributsBuilderNormals.onlyKeepsTheseTypesInCollection1<CT_Nothing1>();

    }
}

void CT_AbstractExporterPointAttributesSelection::setExcludeConfiguration(const QPair<QString, CT_AbstractItemDrawableCollectionBuilder *> &pair, CT_ItemDrawableHierarchyCollectionSelectionModel *model) const
{
    // if result is generated by the builder "m_attributsBuilderColors"
    if((&m_attributsBuilderColors) == pair.second) {
        //model->addExcludeModel(0); // exclude the selection of other colors founded by this builder. The index is 0 because it was the index of the builder in the "QList" of the method getBuilders()

        // if we can export with colors information
        if(canExportWithColors())
            model->addExcludeModel(-1); // exclude the selection of colors of the document
    }

    // if result is generated by the builder "m_attributsBuilderNormals"
    if((&m_attributsBuilderNormals) == pair.second) {
        //model->addExcludeModel(1); // exclude the selection of other normals founded by this builder. The index is 1 because it was the index of the builder in the "QList" of the method getBuilders()

        // if we can export with normals information
        if(canExportWithNormals())
            model->addExcludeModel(-2); // exclude the selection of normals of the document
    }
}

bool CT_AbstractExporterPointAttributesSelection::useSelection(const CT_ItemDrawableHierarchyCollectionWidget *selectorWidget)
{
    clearWorker();

    m_attributsColorPointWorker.setColorCloud(selectorWidget->colorCloudSelected());
    m_attributsNormalPointWorker.setNormalCloud(selectorWidget->normalCloudSelected());

    QList<CT_AbstractPointsAttributes*> attributesColor;
    QList<CT_AbstractPointsAttributes*> attributesNormal;

    QList<CT_AbstractSingularItemDrawable*> list = selectorWidget->itemDrawableSelected();
    QListIterator<CT_AbstractSingularItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractSingularItemDrawable *item = it.next();
        CT_PointsAttributesColor *pac = dynamic_cast<CT_PointsAttributesColor*>(item);

        if(pac != NULL) {
            attributesColor.append(pac);
        } else {
            CT_AbstractPointAttributesScalar *pas = dynamic_cast<CT_AbstractPointAttributesScalar*>(item);

            if(pas != NULL) {
                attributesColor.append(pas);
            } else {

                CT_PointsAttributesNormal *pan = dynamic_cast<CT_PointsAttributesNormal*>(item);

                if(pan != NULL)
                    attributesNormal.append(pan);
            }
        }
    }

    m_attributsColorPointWorker.setAttributes(attributesColor);
    m_attributsNormalPointWorker.setAttributes(attributesNormal);

    return true;
}

CT_AbstractColorCloud* CT_AbstractExporterPointAttributesSelection::createColorCloudBeforeExportToFile()
{
    CT_AbstractColorCloud *cc = NULL;

    if((m_attributsColorPointWorker.colorCloud().data() == NULL)
            && (!m_attributsColorPointWorker.attributes().isEmpty()))
    {
        m_attributsColorPointWorker.setColorCloud(PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud));
        m_attributsColorPointWorker.apply();
        cc = m_attributsColorPointWorker.colorCloud()->abstractColorCloud();
    }
    else if(m_attributsColorPointWorker.colorCloud().data() != NULL)
    {
        cc = m_attributsColorPointWorker.colorCloud()->abstractColorCloud();
    }

    return cc;
}

CT_AbstractNormalCloud* CT_AbstractExporterPointAttributesSelection::createNormalCloudBeforeExportToFile()
{
    CT_AbstractNormalCloud *nc = NULL;

    if((m_attributsNormalPointWorker.normalCloud().data() == NULL)
            && (!m_attributsNormalPointWorker.attributes().isEmpty()))
    {
        m_attributsNormalPointWorker.setNormalCloud(PS_REPOSITORY->createNewNormalCloud(CT_Repository::SyncWithPointCloud));
        m_attributsNormalPointWorker.apply();
        nc = m_attributsNormalPointWorker.normalCloud()->abstractNormalCloud();
    }
    else if(m_attributsNormalPointWorker.normalCloud().data() != NULL)
    {
        nc = m_attributsNormalPointWorker.normalCloud()->abstractNormalCloud();
    }

    return nc;
}

void CT_AbstractExporterPointAttributesSelection::clearWorker()
{
    m_attributsColorPointWorker.setAttributes(QList<CT_AbstractPointsAttributes*>());
    m_attributsColorPointWorker.setColorCloud(QSharedPointer<CT_StandardColorCloudRegistered>(NULL));

    m_attributsNormalPointWorker.setAttributes(QList<CT_AbstractPointsAttributes*>());
    m_attributsNormalPointWorker.setColorCloud(QSharedPointer<CT_StandardColorCloudRegistered>(NULL));
}
