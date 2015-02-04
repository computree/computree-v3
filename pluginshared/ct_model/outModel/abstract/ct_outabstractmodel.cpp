#include "ct_outabstractmodel.h"

#include <QMutex>

CT_OutAbstractModel::CT_OutAbstractModel(const QString &uniqueName,
                                         const QString &description,
                                         const QString &displayableName) : CT_AbstractModel(uniqueName,
                                                                                            description,
                                                                                            displayableName)
{
    m_realResult = NULL;
    m_originalModel = NULL;
    m_originalModelMutex = new QMutex(QMutex::Recursive);
    m_item = NULL;
}

CT_OutAbstractModel::~CT_OutAbstractModel()
{
    delete m_originalModelMutex;
    clearItem();
}

CT_OutAbstractModel* CT_OutAbstractModel::originalModel() const
{
    if(m_originalModel != NULL)
        return m_originalModel->originalModel();

    return const_cast<CT_OutAbstractModel*>(this);
}

CT_OutAbstractModel* CT_OutAbstractModel::currentOriginalModel() const
{
    if(m_originalModel != NULL)
        return m_originalModel;

    return const_cast<CT_OutAbstractModel*>(this);
}

CT_OutAbstractModel* CT_OutAbstractModel::lastOriginalModelWithAResult() const
{
    CT_OutAbstractModel *lastModel = NULL;

    CT_OutAbstractModel *oModel = const_cast<CT_OutAbstractModel*>(this);

    while(oModel != NULL)
    {
        if(oModel->m_realResult != NULL)
            lastModel = oModel;

        oModel = oModel->m_originalModel;
    }

    return lastModel;
}

CT_OutAbstractModel *CT_OutAbstractModel::lastOriginalModelWithAStep() const
{
    CT_OutAbstractModel *lastModel = NULL;

    CT_OutAbstractModel *oModel = const_cast<CT_OutAbstractModel*>(this);

    while(oModel != NULL)
    {
        if(oModel->step() != NULL)
            lastModel = oModel;

        oModel = oModel->m_originalModel;
    }

    return lastModel;
}

bool CT_OutAbstractModel::isVisible() const
{
    bool val = !m_visibleInDocuments.isEmpty();

    if(!val)
    {
        QMutexLocker locker(m_originalModelMutex);

        CT_OutAbstractModel* oModel = lastOriginalModelWithAResult();

        if(oModel != this)
            val = oModel->isVisible();
    }

    return val;
}

bool CT_OutAbstractModel::isVisibleInDocument(const DocumentInterface *doc) const
{
    bool val = (m_visibleInDocuments.value((DocumentInterface*)doc, 0) > 0);

    if(!val)
    {
        QMutexLocker locker(m_originalModelMutex);

        CT_OutAbstractModel* oModel = lastOriginalModelWithAResult();

        if(oModel != this)
            val = oModel->isVisibleInDocument(doc);
    }

    return val;
}

CT_AbstractItem* CT_OutAbstractModel::item() const
{
    return m_item;
}

CT_AbstractResult* CT_OutAbstractModel::result() const
{
    CT_AbstractResult *lastResult = m_realResult;

    if(m_realResult == NULL)
    {
        CT_OutAbstractModel *oModel = m_originalModel;

        while(oModel != NULL)
        {
            if(oModel->m_realResult != NULL)
                lastResult = oModel->m_realResult;

            oModel = oModel->m_originalModel;
        }
    }

    return lastResult;
}

void CT_OutAbstractModel::setResult(const CT_AbstractResult *res)
{
    m_realResult = (CT_AbstractResult*)res;

    QList<CT_AbstractModel*> l = childrens();
    QListIterator<CT_AbstractModel*> it(l);

    while(it.hasNext())
        ((CT_OutAbstractModel*)it.next())->setResult(res);
}

void CT_OutAbstractModel::setOriginalModel(const CT_OutAbstractModel *model)
{
    if(m_originalModel != NULL)
        disconnect(m_originalModel, NULL, this, NULL);

    m_originalModel = (CT_OutAbstractModel*)model;

    if(m_originalModel != NULL)
    {
        connect(m_originalModel, SIGNAL(visibilityChanged(bool)), this, SIGNAL(visibilityChanged(bool)), Qt::DirectConnection);
        connect(m_originalModel, SIGNAL(destroyed()), this, SLOT(originalModelDestroyed()), Qt::DirectConnection);
    }
}

void CT_OutAbstractModel::setItem(CT_AbstractItem *item)
{
    clearItem();
    m_item = item;
}

void CT_OutAbstractModel::clearItem()
{
    delete m_item;
    m_item = NULL;
}

void CT_OutAbstractModel::incrementVisibleInDocument(const DocumentInterface *doc)
{
    bool empty = m_visibleInDocuments.isEmpty();

    DocumentInterface *d = (DocumentInterface*)doc;

    int val = m_visibleInDocuments.value(d, 0) + 1;

    m_visibleInDocuments.insert(d, val);

    if(val == 1)
        emit visibilityInDocumentChanged(doc, true);

    if(m_visibleInDocuments.isEmpty() != empty)
        emit visibilityChanged(true);
}

void CT_OutAbstractModel::decrementVisibleInDocument(const DocumentInterface *doc)
{
    bool empty = m_visibleInDocuments.isEmpty();

    DocumentInterface *d = (DocumentInterface*)doc;

    int val = m_visibleInDocuments.value(d, 0) - 1;

    if(val > 0)
        m_visibleInDocuments.insert(d, val);
    else
        m_visibleInDocuments.remove(d);

    if(val == 0)
        emit visibilityInDocumentChanged(doc, false);

    if(m_visibleInDocuments.isEmpty() != empty)
        emit visibilityChanged(false);
}

bool CT_OutAbstractModel::recursiveSetComplete()
{
    if(setComplete())
    {
        CT_OutAbstractModel *thisModel = this;
        CT_OutAbstractModel *oModel = currentOriginalModel();

        while((oModel != NULL) && (oModel != thisModel))
        {
            oModel->setComplete();

            thisModel = oModel;
            oModel = oModel->currentOriginalModel();
        }

        QList<CT_AbstractModel*> c = childrens();
        QListIterator<CT_AbstractModel*> it(c);

        while(it.hasNext())
        {
            oModel = (CT_OutAbstractModel*)it.next();

            if(oModel->step() == NULL)
                oModel->setStep(step());

            if(!oModel->recursiveSetComplete())
                return false;
        }
    }

    return true;
}

bool CT_OutAbstractModel::setComplete()
{
    return true;
}

void CT_OutAbstractModel::originalModelDestroyed()
{
    m_originalModel = NULL;
}
