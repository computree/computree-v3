#include "ct_abstractexporterattributesselection.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QMessageBox>

CT_AbstractExporterAttributesSelection::CT_AbstractExporterAttributesSelection() : CT_AbstractExporter()
{
}

bool CT_AbstractExporterAttributesSelection::configureExport()
{
    return selectAttributes();
}

bool CT_AbstractExporterAttributesSelection::selectAttributes()
{
    m_buildersResults.clear();

    if(!errorMessage().isEmpty())
    {
        QMessageBox::critical(NULL, QObject::tr("Erreur"), errorMessage(), QMessageBox::Ok);
        return false;
    }

    QDialog dialog;
    dialog.setWindowTitle(QObject::tr("Configuration des autres éléments de l'export"));
    dialog.resize(700,500);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                                     Qt::Vertical);
    CT_ItemDrawableHierarchyCollectionWidget *selectorWidget = new CT_ItemDrawableHierarchyCollectionWidget(cloudType(), &dialog);
    selectorWidget->setDocumentManager(documentManager());

    QHBoxLayout *layout = new QHBoxLayout(&dialog);
    layout->addWidget(selectorWidget);
    layout->addWidget(buttons);

    connect(buttons, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), &dialog, SLOT(reject()));

    buildAttributesCollection();

    selectorWidget->setModel(buildModel());

    if(selectorWidget->hasChoice())
    {
        if(dialog.exec() == QDialog::Rejected)
            return false;

        return useSelection(selectorWidget);
    }

    return true;
}

CT_VirtualAbstractStep *CT_AbstractExporterAttributesSelection::rootStep() const
{
    CT_VirtualAbstractStep *parent = myStep();

    if(parent == NULL)
    {
        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext()
              && (parent == NULL))
        {
            CT_AbstractItemDrawable *id = it.next();

            if(id->result() != NULL)
                parent = id->result()->parentStep();
        }
    }

    if(parent != NULL)
    {
        parent = dynamic_cast<CT_VirtualAbstractStep*>(parent)->rootStep();
    }

    return dynamic_cast<CT_VirtualAbstractStep*>(parent);
}

void CT_AbstractExporterAttributesSelection::buildAttributesCollection()
{
    CT_VirtualAbstractStep *parent = rootStep();

    if(parent != NULL)
    {
        // get builders from the derivate class
        QList< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > builders = getBuilders();
        QListIterator< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > it(builders);

        while(it.hasNext())
        {
            const QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> &pair = it.next();
            CT_AbstractItemDrawableCollectionBuilder *builder = pair.second;

            if(builder->buildFrom(dynamic_cast<CT_VirtualAbstractStep*>(parent)))
            {
                // let the derivate class post configure the builder (per example to remove certains elements)
                postConfigureAttributesBuilder(builder);

                m_buildersResults.append(pair);
            }
        }
    }
}

CT_ItemDrawableHierarchyCollectionModel* CT_AbstractExporterAttributesSelection::buildModel() const
{
    CT_ItemDrawableHierarchyCollectionModel *model = new CT_ItemDrawableHierarchyCollectionModel();

    CT_ItemDrawableHierarchyCollectionSelectionModel *sModel;

    QListIterator< QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> > it(m_buildersResults);

    while(it.hasNext())
    {
        const QPair<QString, CT_AbstractItemDrawableCollectionBuilder*> &pair = it.next();

        sModel = new CT_ItemDrawableHierarchyCollectionSelectionModel(pair.first);

        if(!pair.second->collection().isEmpty())
            sModel->setCollection(pair.second->collection());

        // let the derivate class add the exclude information for this selection model
        setExcludeConfiguration(pair, sModel);

        model->addModel(sModel);
    }

    if(model->models().isEmpty())
    {
        delete model;
        model = NULL;
    }
    else
    {
        int currentIndex = 0;

        // add excluded model to other models
        QListIterator<CT_ItemDrawableHierarchyCollectionSelectionModel*> itM(model->models());

        while(itM.hasNext())
        {
            CT_ItemDrawableHierarchyCollectionSelectionModel *sModel = itM.next();

            // excluded modes of this selection model
            QList<int> excluded = sModel->excludedModels();

            foreach (int index, excluded)
            {
                if((index != currentIndex) && (index >= 0))
                {
                    // model excluded from this selection model
                    CT_ItemDrawableHierarchyCollectionSelectionModel *sModelExcluded = model->models().at(index);

                    // if this model has not already the current selection model in excluded list
                    if(!sModelExcluded->mustExcludeModel(currentIndex))
                        sModelExcluded->addExcludeModel(currentIndex);  // we add it
                }
            }

            ++currentIndex;
        }
    }

    return model;
}
