#include "pb_stepgenericloadfile.h"

#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h"
#include "ct_result/model/inModel/ct_inresultmodelnotneedinputresult.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

#define DEF_SearchResult                "PB_SGLF_result"
#define DEF_SearchGroup                 "PB_SGLF_group"

PB_StepGenericLoadFile::PB_StepGenericLoadFile(CT_StepInitializeData &dataInit, CT_AbstractReader *reader) : CT_AbstractStepLoadFile(dataInit)
{
    m_reader = reader;

    connect(m_reader, SIGNAL(progressChanged(int)), this, SLOT(readerProgressChanged(int)), Qt::DirectConnection);
}

PB_StepGenericLoadFile::~PB_StepGenericLoadFile()
{
    delete m_reader;
}

void PB_StepGenericLoadFile::init()
{
    m_reader->init(false);

    CT_AbstractStepLoadFile::init();
}

QString PB_StepGenericLoadFile::getStepDescription() const
{
    QString ext = createAcceptedExtensionString(" ");

    return tr("Charge un fichier du type") + ext;
}

QList<QString> PB_StepGenericLoadFile::getFileExtensionAccepted() const
{
    QList<QString> l;

    QListIterator<FileFormat> it(m_reader->readableFormats());

    while(it.hasNext())
    {
        const FileFormat &ff = it.next();

        QListIterator<QString> itS(ff.suffixes());

        while(itS.hasNext())
            l.append("." + itS.next());
    }

    return l;
}

bool PB_StepGenericLoadFile::setFilePath(QString filePath)
{
    QString fp = getFilePath();

    if(CT_AbstractStepLoadFile::setFilePath(filePath))
    {
        if(!m_reader->setFilePath(getFilePath()))
        {
            setFilePath(fp);
            setSettingsModified(false);

            return false;
        }

        return true;
    }

    return false;
}

CT_VirtualAbstractStep* PB_StepGenericLoadFile::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepGenericLoadFile(dataInit, m_reader->copy());
}

bool PB_StepGenericLoadFile::postConfigure()
{
    if(CT_AbstractStepLoadFile::postConfigure())
    {
        return m_reader->configure();
    }

    return false;
}

bool PB_StepGenericLoadFile::protectedInitAfterConfiguration()
{
    if(getStepChildList().isEmpty())
    {
        m_reader->createOutItemDrawableModelList();
    }

    return true;
}

void PB_StepGenericLoadFile::createInResultModelListProtected()
{
    // No in result is needed
    CT_InResultModelNotNeedInputResult *resultModel = new CT_InResultModelNotNeedInputResult();
    addInResultModel(resultModel);
}

// Creation and affiliation of OUT models
void PB_StepGenericLoadFile::createOutResultModelListProtected()
{
    CT_OutStdGroupModel *root = new CT_OutStdGroupModel(DEF_SearchGroup);

    QListIterator<CT_OutStdSingularItemModel*> it(m_reader->outItemDrawableModels());

    while(it.hasNext())
    {
        CT_OutStdSingularItemModel *model = it.next();
        root->addItem((CT_OutStdSingularItemModel*)model->copy());
    }

    QListIterator<CT_OutStdGroupModel*> itG(m_reader->outGroupsModel());

    while(itG.hasNext())
    {
        CT_OutStdGroupModel *model = itG.next();
        root->addGroup((CT_OutStdGroupModel*)model->copy());
    }

    addOutResultModel(new CT_OutResultModelGroup(DEF_SearchResult, root, "Result"));
}

void PB_StepGenericLoadFile::compute()
{
    if(m_reader->readFile())
    {
        CT_ResultGroup *out_res = getOutResultList().first();
        CT_StandardItemGroup *group = new CT_StandardItemGroup((DEF_CT_AbstractGroupModelOut*)getOutModelForCreation(out_res, DEF_SearchGroup), out_res);

        QListIterator<CT_OutStdSingularItemModel*> it(m_reader->outItemDrawableModels());

        while(it.hasNext())
        {
            CT_OutStdSingularItemModel *model = it.next();
            CT_OutAbstractItemModel *modelCreation = getOutModelForCreation(out_res, model->name());

            QList<CT_AbstractSingularItemDrawable*> items = m_reader->takeItemDrawableOfModel(model->name(), out_res, modelCreation);
            QListIterator<CT_AbstractSingularItemDrawable*> itI(items);

            while(itI.hasNext())
                group->addItemDrawable(itI.next());
        }

        QListIterator<CT_OutStdGroupModel*> itG(m_reader->outGroupsModel());

        while(itG.hasNext())
        {
            CT_OutStdGroupModel *model = itG.next();
            CT_OutAbstractItemModel *modelCreation = getOutModelForCreation(out_res, model->name());

            QList<CT_AbstractItemGroup*> groups = m_reader->takeGroupOfModel(model->name(), out_res, modelCreation);
            QListIterator<CT_AbstractItemGroup*> itI(groups);

            while(itI.hasNext())
                group->addGroup(itI.next());
        }

        out_res->addGroup(group);
    }
}

void PB_StepGenericLoadFile::readerProgressChanged(int progress)
{
    setProgress(progress);
}
