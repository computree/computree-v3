#include "dm_singleitemdrawabletoosgworker.h"

DM_SingleItemDrawableToOsgWorker::DM_SingleItemDrawableToOsgWorker(GOsgGraphicsView &view, QObject *parent) : m_view(view),
    QObject(parent), QRunnable()
{
    m_item = NULL;
    m_result.m_rootGroup = NULL;
    setColor(Qt::white);
}

void DM_SingleItemDrawableToOsgWorker::setItemDrawable(CT_AbstractItemDrawable *item)
{
    m_item = item;
}

void DM_SingleItemDrawableToOsgWorker::setColor(const QColor &color)
{
    m_painter.setColor(color);
}

void DM_SingleItemDrawableToOsgWorker::setGeometriesConfiguration(const DM_GeometriesConfiguration &config)
{
    m_geometriesConfiguration = config;
    m_painter.setGeometriesConfiguration(&m_geometriesConfiguration);
}

CT_AbstractItemDrawable *DM_SingleItemDrawableToOsgWorker::itemDrawable() const
{
    return m_item;
}

DM_PainterToOsgElementsResult DM_SingleItemDrawableToOsgWorker::result() const
{
    return m_result;
}

void DM_SingleItemDrawableToOsgWorker::compute()
{
    run();
}

void DM_SingleItemDrawableToOsgWorker::run()
{
    m_item->draw(m_view, m_painter);

    m_painter.finish();
    m_result = m_painter.results();

    emit finished(this);
}
