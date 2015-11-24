#include "dm_configurabletechnique.h"

DM_ConfigurableTechnique::DM_ConfigurableTechnique() : osgFX::Technique()
{
}

void DM_ConfigurableTechnique::addPassToUse(osg::StateSet *ss)
{
    m_passes.push_back(ss);
}

void DM_ConfigurableTechnique::addPassToUse(QList< osg::ref_ptr<osg::StateSet> > l)
{
    QListIterator< osg::ref_ptr<osg::StateSet> > it(l);
    while(it.hasNext()) {
        m_passes.push_back(it.next().get());
    }
}

void DM_ConfigurableTechnique::define_passes()
{
    for(MPass_list::const_iterator it = m_passes.begin(); it != m_passes.end(); it++) {
        addPass((*it).get());
    }
}
