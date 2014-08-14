#include "cdm_scriptmanagerabstract.h"

CDM_ScriptManagerAbstract::CDM_ScriptManagerAbstract(CDM_PluginManager &pluginManager)
{
    setPluginManager(pluginManager);
}

void CDM_ScriptManagerAbstract::setPluginManager(CDM_PluginManager &pluginManager)
{
    _pluginManager = &pluginManager;
}

CDM_PluginManager* CDM_ScriptManagerAbstract::getPluginManager() const
{
    return _pluginManager;
}
