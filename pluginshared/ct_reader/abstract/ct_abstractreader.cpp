#include "ct_abstractreader.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_global/ct_context.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

CT_AbstractReader::CT_AbstractReader()
{
}

CT_AbstractReader::~CT_AbstractReader()
{
    clearOutItemDrawableModel();
    clearOutItemDrawable();

    clearOutGroupsModel();
    clearOutGroups();
}

void CT_AbstractReader::init(bool initOutItemDrawableList)
{
    protectedInit();

    if(initOutItemDrawableList)
        protectedCreateOutItemDrawableModelList();
}

bool CT_AbstractReader::setFilePath(const QString &filepath)
{
    m_filePath = filepath;
    return true;
}

QString CT_AbstractReader::filepath() const
{
    return m_filePath;
}

void CT_AbstractReader::createOutItemDrawableModelList()
{
    clearOutItemDrawableModel();
    clearOutGroupsModel();
    protectedCreateOutItemDrawableModelList();
}

const QList<CT_OutStdSingularItemModel*>& CT_AbstractReader::outItemDrawableModels() const
{
    return m_outItemsModel;
}

const QList<CT_OutStdGroupModel*>& CT_AbstractReader::outGroupsModel() const
{
    return m_outGroupsModel;
}

const QList<FileFormat>& CT_AbstractReader::readableFormats() const
{
    return m_formats;
}

bool CT_AbstractReader::isReadError() const
{
    return m_error;
}

bool CT_AbstractReader::isStopped() const
{
    return m_stop;
}

QString CT_AbstractReader::toolTip() const
{
    return m_tooltip;
}

bool CT_AbstractReader::readFile()
{
    emit started();

    clearErrorMessage();
    setProgress(0);

    clearOutItemDrawable();

    m_error = false;
    m_stop = false;

    PS_COORDINATES_SYS_MANAGER->initUsedOfAllCoordinateSystem();

    m_error = protectedReadFile();

    if(!m_error && !PS_COORDINATES_SYS->wasUsed())
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Reader error ! The reader has not used the coordinate system !"));

    emit finished();

    return m_error;
}

void CT_AbstractReader::cancel()
{
    m_stop = true;
}

QString CT_AbstractReader::errorMessage() const
{
    return m_errorMess;
}

void CT_AbstractReader::clearErrorMessage()
{
    m_errorMess = "";
    m_error = false;
}

QList<CT_AbstractSingularItemDrawable*> CT_AbstractReader::takeItemDrawableOfType(const QString &type,
                                                                                  const CT_AbstractResult *result,
                                                                                  CT_OutAbstractItemModel *model)
{
    QList<CT_AbstractSingularItemDrawable*> l;

    QMutableMapIterator<QString, CT_AbstractSingularItemDrawable*> it(m_outItems);

    while(it.hasNext())
    {
        it.next();

        CT_AbstractSingularItemDrawable *item = it.value();

        if(item->getType() == type)
        {
            if(result != NULL)
                item->changeResult((CT_AbstractResult*)result);

            if(model != NULL)
                item->setModel(model);

            l.append(item);
            it.remove();
        }
    }

    return l;
}

QList<CT_AbstractSingularItemDrawable *> CT_AbstractReader::takeItemDrawableOfModel(const QString &modelName,
                                                                                    const CT_AbstractResult *result,
                                                                                    CT_OutAbstractItemModel *model)
{
    QList<CT_AbstractSingularItemDrawable*> l;

    QMutableMapIterator<QString, CT_AbstractSingularItemDrawable*> it(m_outItems);

    while(it.hasNext())
    {
        it.next();

        if(it.key() == modelName)
        {
            CT_AbstractSingularItemDrawable *item = it.value();

            if(result != NULL)
                item->changeResult((CT_AbstractResult*)result);

            if(model != NULL)
                item->setModel(model);

            l.append(item);
            it.remove();
        }
    }

    return l;
}

QList<CT_AbstractSingularItemDrawable *> CT_AbstractReader::takeItemDrawableOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName)
{
    CT_OutAbstractItemModel *model = dynamic_cast<CT_OutAbstractItemModel*>(PS_MODELS->searchModelForCreation(outModelName, result));

    Q_ASSERT_X(model != NULL, "CT_AbstractReader::takeItemDrawableOfModel", "You want to take items and set it a new model searched by modelName but this model was not found or is not a itemdrawable model");

    return takeItemDrawableOfModel(modelName, result, model);
}

CT_AbstractSingularItemDrawable* CT_AbstractReader::takeFirstItemDrawableOfType(const QString &type,
                                                                                const CT_AbstractResult *result,
                                                                                CT_OutAbstractItemModel *model)
{
    QMutableMapIterator<QString, CT_AbstractSingularItemDrawable*> it(m_outItems);

    while(it.hasNext())
    {
        it.next();

        CT_AbstractSingularItemDrawable *item = it.value();

        if(item->getType() == type)
        {
            it.remove();

            if(result != NULL)
                item->changeResult(result);

            if(model != NULL)
                item->setModel(model);

            return item;
        }
    }

    return NULL;
}

CT_AbstractSingularItemDrawable* CT_AbstractReader::takeFirstItemDrawableOfModel(const QString &modelName,
                                                                                 const CT_AbstractResult *result,
                                                                                 CT_OutAbstractItemModel *model)
{
    QMutableMapIterator<QString, CT_AbstractSingularItemDrawable*> it(m_outItems);

    while(it.hasNext())
    {
        it.next();

        if(it.key() == modelName)
        {
            CT_AbstractSingularItemDrawable *item = it.value();

            it.remove();

            if(result != NULL)
                item->changeResult(result);

            if(model != NULL)
                item->setModel(model);

            return item;
        }
    }

    return NULL;
}

CT_AbstractSingularItemDrawable* CT_AbstractReader::takeFirstItemDrawableOfModel(const QString &modelName,
                                                                                 const CT_AbstractResult *result,
                                                                                 const QString &outModelName)
{
    CT_OutAbstractItemModel *model = dynamic_cast<CT_OutAbstractItemModel*>(PS_MODELS->searchModelForCreation(outModelName, result));

    Q_ASSERT_X(model != NULL, "CT_AbstractReader::takeFirstItemDrawableOfModel", "You want to take the first item and set it a new model searched by modelName but this model was not found or is not a itemdrawable model");

    return takeFirstItemDrawableOfModel(modelName, result, model);
}

QList<CT_AbstractSingularItemDrawable*> CT_AbstractReader::itemDrawableOfType(const QString &type) const
{
    QList<CT_AbstractSingularItemDrawable*> l;

    QMapIterator<QString, CT_AbstractSingularItemDrawable*> it(m_outItems);

    while(it.hasNext())
    {
        it.next();

        CT_AbstractSingularItemDrawable *item = it.value();

        if(item->getType() == type)
            l.append(item);
    }

    return l;
}

QList<CT_AbstractSingularItemDrawable *> CT_AbstractReader::itemDrawableOfModel(const QString &modelName) const
{
    QList<CT_AbstractSingularItemDrawable*> l;

    QMapIterator<QString, CT_AbstractSingularItemDrawable*> it(m_outItems);

    while(it.hasNext())
    {
        it.next();

        if(it.key() == modelName)
            l.append(it.value());
    }

    return l;
}

CT_AbstractSingularItemDrawable *CT_AbstractReader::firstItemDrawableOfModel(const QString &modelName) const
{
    return m_outItems.value(modelName, NULL);
}

QList<CT_AbstractItemGroup *> CT_AbstractReader::takeGroupOfModel(const QString &modelName, const CT_AbstractResult *result, CT_OutAbstractItemModel *model)
{
    QList<CT_AbstractItemGroup*> l;

    QMutableMapIterator<QString, CT_AbstractItemGroup*> it(m_outGroups);

    while(it.hasNext())
    {
        it.next();

        if(it.key() == modelName)
        {
            CT_AbstractItemGroup *gr = it.value();

            if(result != NULL)
                gr->changeResult((CT_AbstractResult*)result);

            if(model != NULL)
                gr->setModel(model);

            l.append(gr);
            it.remove();
        }
    }

    return l;
}

QList<CT_AbstractItemGroup *> CT_AbstractReader::takeGroupOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName)
{
    CT_OutAbstractGroupModel *model = dynamic_cast<CT_OutAbstractGroupModel*>(PS_MODELS->searchModelForCreation(outModelName, result));

    Q_ASSERT_X(model != NULL, "CT_AbstractReader::takeGroupOfModel", "You want to take groups and set it a new model searched by modelName but this model was not found or is not a group model");

    return takeGroupOfModel(modelName, result, model);
}

CT_AbstractItemGroup* CT_AbstractReader::takeFirstGroupOfModel(const QString &modelName, const CT_AbstractResult *result, CT_OutAbstractItemModel *model)
{
    QMutableMapIterator<QString, CT_AbstractItemGroup*> it(m_outGroups);

    while(it.hasNext())
    {
        it.next();

        if(it.key() == modelName)
        {
            CT_AbstractItemGroup *gr = it.value();

            it.remove();

            if(result != NULL)
                gr->changeResult((CT_AbstractResult*)result);

            if(model != NULL)
                gr->setModel(model);

            return gr;
        }
    }

    return NULL;
}

CT_AbstractItemGroup* CT_AbstractReader::takeFirstGroupOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName)
{
    CT_OutAbstractGroupModel *model = dynamic_cast<CT_OutAbstractGroupModel*>(PS_MODELS->searchModelForCreation(outModelName, result));

    Q_ASSERT_X(model != NULL, "CT_AbstractReader::takeFirstGroupOfModel", "You want to take first group and set it a new model searched by modelName but this model was not found or is not a group model");

    return takeFirstGroupOfModel(modelName, result, model);
}

QList<CT_AbstractItemGroup *> CT_AbstractReader::groupOfModel(const QString &modelName) const
{
    QList<CT_AbstractItemGroup*> l;

    QMapIterator<QString, CT_AbstractItemGroup*> it(m_outGroups);

    while(it.hasNext())
    {
        it.next();

        if(it.key() == modelName)
            l.append(it.value());
    }

    return l;
}

void CT_AbstractReader::addNewReadableFormat(const FileFormat &format)
{
    m_formats.append(format);
}

void CT_AbstractReader::setNotNeedToUseCoordinateSystem()
{
    PS_COORDINATES_SYS->informThatUsed();
}

void CT_AbstractReader::setToolTip(const QString &t)
{
    m_tooltip = t;
}

void CT_AbstractReader::addOutItemDrawableModel(CT_OutStdSingularItemModel *item)
{
    m_outItemsModel.append(item);
}

void CT_AbstractReader::addOutItemDrawable(const QString &modelName, CT_AbstractSingularItemDrawable *item)
{
    if(item != NULL)
        m_outItems.insert(modelName, item);
}

void CT_AbstractReader::addOutGroupModel(CT_OutStdGroupModel *gr)
{
    m_outGroupsModel.append(gr);
}

void CT_AbstractReader::addOutGroup(const QString &modelName, CT_AbstractItemGroup *gr)
{
    if(gr != NULL)
        m_outGroups.insert(modelName, gr);
}

bool CT_AbstractReader::containsItemDrawableModel(const QString &uniqueName) const
{
    QListIterator<CT_OutStdSingularItemModel*> it(m_outItemsModel);

    while(it.hasNext())
    {
        if(it.next()->uniqueName() == uniqueName)
            return true;
    }

    return false;
}

void CT_AbstractReader::setProgress(int progress)
{
    if(progress != m_progress)
    {
        m_progress = progress;

        emit progressChanged(m_progress);
    }
}

void CT_AbstractReader::setErrorMessage(const QString &err)
{
    m_errorMess = err;

    if(!m_errorMess.isEmpty())
        m_error = true;
}

void CT_AbstractReader::clearOutItemDrawableModel()
{
    qDeleteAll(m_outItemsModel.begin(), m_outItemsModel.end());
    m_outItemsModel.clear();
}

void CT_AbstractReader::clearOutItemDrawable()
{
    qDeleteAll(m_outItems.begin(), m_outItems.end());
    m_outItems.clear();
}

void CT_AbstractReader::clearOutGroupsModel()
{
    qDeleteAll(m_outGroupsModel.begin(), m_outGroupsModel.end());
    m_outGroupsModel.clear();
}

void CT_AbstractReader::clearOutGroups()
{
    qDeleteAll(m_outGroups.begin(), m_outGroups.end());
    m_outGroups.clear();
}
