#include "dm_configurableeffect.h"

DM_ConfigurableEffect::DM_ConfigurableEffect() : osgFX::Effect()
{
}

const char *DM_ConfigurableEffect::effectName() const
{
    return "Configurable effect";
}

const char *DM_ConfigurableEffect::effectDescription() const
{
    return "Configurable effect";
}

const char *DM_ConfigurableEffect::effectAuthor() const
{
    return "MK";
}

void DM_ConfigurableEffect::setTechnique(osgFX::Technique *tech)
{
    m_technique = tech;
}

bool DM_ConfigurableEffect::define_techniques()
{
    addTechnique(m_technique.get());

    return true;
}
