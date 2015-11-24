#ifndef DM_CONFIGURABLETECHNIQUE_H
#define DM_CONFIGURABLETECHNIQUE_H

#include <osgFX/Technique>
#include <QList>

class DM_ConfigurableTechnique : public osgFX::Technique
{
public:
    DM_ConfigurableTechnique();

    void addPassToUse(osg::StateSet *ss);
    void addPassToUse(QList< osg::ref_ptr<osg::StateSet> > l);

private:
    typedef std::list< osg::ref_ptr<osg::StateSet> > MPass_list;
    MPass_list    m_passes;

protected:
    void define_passes();
};

#endif // DM_CONFIGURABLETECHNIQUE_H
