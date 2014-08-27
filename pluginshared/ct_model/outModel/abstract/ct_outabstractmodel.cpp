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
    return m_originalModel;
}

bool CT_OutAbstractModel::isVisible() const
{
    QMutexLocker locker(m_originalModelMutex);

    if(originalModel() != NULL)
        return originalModel()->isVisible();

    return !m_visibleInDocuments.isEmpty();
}

bool CT_OutAbstractModel::isVisibleInDocument(const DocumentInterface *doc) const
{
    bool val = (m_visibleInDocuments.value((DocumentInterface*)doc, 0) > 0);

    QMutexLocker locker(m_originalModelMutex);

    if(!val && (originalModel() != NULL))
        return originalModel()->isVisibleInDocument(doc);

    return val;
}

CT_AbstractItem* CT_OutAbstractModel::item() const
{
    return m_item;
}

CT_AbstractResult* CT_OutAbstractModel::result() const
{
    if((m_realResult == NULL)
            && (originalModel() != NULL))
        return originalModel()->result();

    return m_realResult;
}

void CT_OutAbstractModel::setResult(const CT_AbstractResult *res)
{
    m_realResult = (CT_AbstractResult*)res;
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
    QList<CT_AbstractModel*> c = childrens();
    QListIterator<CT_AbstractModel*> it(c);

    while(it.hasNext())
    {
        if(!((CT_OutAbstractModel*)it.next())->recursiveSetComplete())
            return false;
    }

    return true;
}

void CT_OutAbstractModel::originalModelDestroyed()
{
    m_originalModel = NULL;
}
