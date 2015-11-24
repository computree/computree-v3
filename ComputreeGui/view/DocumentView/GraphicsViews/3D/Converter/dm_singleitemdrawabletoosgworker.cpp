#include "dm_singleitemdrawabletoosgworker.h"

#include "view/DocumentView/GraphicsViews/3D/Painting/dm_configurableeffect.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_configurabletechnique.h"

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
    if(color.alpha() == KEEP_ALPHA_COLOR)
        m_painter.setColor(QColor(color.red(), color.green(), color.blue()));
    else
        m_painter.setColor(color);
}

void DM_SingleItemDrawableToOsgWorker::setGeometriesConfiguration(const DM_GeometriesConfiguration &config)
{
    m_geometriesConfiguration = config;
    m_geometriesConfiguration.setFunctionToCreateNewEffectForGlobal(staticCreateGlobalEffect, this);
    m_geometriesConfiguration.setFunctionToCreateNewEffectForLocal(staticCreateLocalEffect, this);

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

osgFX::Effect* DM_SingleItemDrawableToOsgWorker::staticCreateGlobalEffect(osg::PrimitiveSet::Mode mode, void *context)
{
    DM_SingleItemDrawableToOsgWorker *ptr = (DM_SingleItemDrawableToOsgWorker*)context;

    QList<osg::ref_ptr<osg::StateSet> > l = ptr->m_geometriesConfiguration.globalStateSet(mode);

    DM_ConfigurableEffect *eff = NULL;

    if(!l.empty()) {
        eff = new DM_ConfigurableEffect();
        DM_ConfigurableTechnique *tech = new DM_ConfigurableTechnique();

        eff->setTechnique(tech);
        tech->addPassToUse(l);
    }

    return eff;
}

osgFX::Effect* DM_SingleItemDrawableToOsgWorker::staticCreateLocalEffect(osg::PrimitiveSet::Mode mode, void *context)
{
    DM_SingleItemDrawableToOsgWorker *ptr = (DM_SingleItemDrawableToOsgWorker*)context;

    QList<osg::ref_ptr<osg::StateSet> > l = ptr->m_geometriesConfiguration.localStateSet(mode);

    DM_ConfigurableEffect *eff = NULL;

    if(!l.empty()) {
        eff = new DM_ConfigurableEffect();
        DM_ConfigurableTechnique *tech = new DM_ConfigurableTechnique();

        eff->setTechnique(tech);
        tech->addPassToUse(l);
    }

    return eff;
}
