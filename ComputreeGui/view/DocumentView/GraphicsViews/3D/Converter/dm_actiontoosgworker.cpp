#include "dm_actiontoosgworker.h"

#include "view/DocumentView/GraphicsViews/3D/Painting/dm_configurableeffect.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_configurabletechnique.h"

DM_ActionToOsgWorker::DM_ActionToOsgWorker() : DM_AbstractWorker()
{
    m_action = NULL;
    m_gv = NULL;

    m_painter.setUseDisplayList(false);
    m_painter.setGeometriesConfiguration(&m_geoConfig);

    connect(this, SIGNAL(canceled()), this, SLOT(cancelEvent()), Qt::DirectConnection);
}

void DM_ActionToOsgWorker::setActionToConvert(CT_AbstractActionForGraphicsView *action)
{
    m_action = action;
}

void DM_ActionToOsgWorker::setGraphicsView(GraphicsViewInterface *gv)
{
    m_gv = gv;
}

DM_PainterToOsgElementsResult DM_ActionToOsgWorker::results() const
{
    return m_painter.results();
}

void DM_ActionToOsgWorker::setGeometriesConfiguration(const DM_GeometriesConfiguration &config)
{
    m_geoConfig = config;
    m_geoConfig.setFunctionToCreateNewEffectForGlobal(staticCreateGlobalEffect, this);
    m_geoConfig.setFunctionToCreateNewEffectForLocal(staticCreateLocalEffect, this);
}

void DM_ActionToOsgWorker::apply()
{
    setProgress(0);

    if((m_action != NULL) && (m_gv != NULL)){

        m_action->draw(*m_gv, m_painter);

        m_painter.finish();
    }

    setProgress(100);

    setFinished();
}

void DM_ActionToOsgWorker::cancelEvent()
{
    m_painter.cancel();
}

osgFX::Effect* DM_ActionToOsgWorker::staticCreateGlobalEffect(osg::PrimitiveSet::Mode mode, void *context)
{
    DM_ActionToOsgWorker *ptr = (DM_ActionToOsgWorker*)context;

    QList<osg::ref_ptr<osg::StateSet> > l = ptr->m_geoConfig.globalStateSet(mode);

    DM_ConfigurableEffect *eff = NULL;

    if(!l.empty()) {
        eff = new DM_ConfigurableEffect();
        DM_ConfigurableTechnique *tech = new DM_ConfigurableTechnique();

        eff->setTechnique(tech);
        tech->addPassToUse(l);
    }

    return eff;
}

osgFX::Effect* DM_ActionToOsgWorker::staticCreateLocalEffect(osg::PrimitiveSet::Mode mode, void *context)
{
    DM_ActionToOsgWorker *ptr = (DM_ActionToOsgWorker*)context;

    QList<osg::ref_ptr<osg::StateSet> > l = ptr->m_geoConfig.localStateSet(mode);

    DM_ConfigurableEffect *eff = NULL;

    if(!l.empty()) {
        eff = new DM_ConfigurableEffect();
        DM_ConfigurableTechnique *tech = new DM_ConfigurableTechnique();

        eff->setTechnique(tech);
        tech->addPassToUse(l);
    }

    return eff;
}
