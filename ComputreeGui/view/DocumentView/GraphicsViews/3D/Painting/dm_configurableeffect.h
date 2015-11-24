#ifndef DM_CONFIGURABLEEFFECT_H
#define DM_CONFIGURABLEEFFECT_H

#include <osgFX/Effect>

class DM_ConfigurableEffect : public osgFX::Effect
{
public:
    DM_ConfigurableEffect();

    /** get the name of this Effect */
    const char *effectName() const;

    /** get a brief description of this Effect*/
    const char *effectDescription() const;

    /** get the effect author's name */
    const char *effectAuthor() const;

    void setTechnique(osgFX::Technique* tech);

private:
    osg::ref_ptr<osgFX::Technique>  m_technique;

protected:
    bool define_techniques();
};

#endif // DM_CONFIGURABLEEFFECT_H
