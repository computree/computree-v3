#include "cdm_scriptmanagerabstract.h"

CDM_ScriptManagerAbstract::CDM_ScriptManagerAbstract(CDM_PluginManager &pluginManager)
{
    m_callBack = NULL;
    setPluginManager(pluginManager);
}

CDM_ScriptManagerAbstract::IScriptLoadCallBack *CDM_ScriptManagerAbstract::scriptLoadCallBack() const
{
    return m_callBack;
}

void CDM_ScriptManagerAbstract::setScriptLoadCallBack(CDM_ScriptManagerAbstract::IScriptLoadCallBack *c)
{
    m_callBack = c;
}

void CDM_ScriptManagerAbstract::setPluginManager(CDM_PluginManager &pluginManager)
{
    _pluginManager = &pluginManager;
}

CDM_PluginManager* CDM_ScriptManagerAbstract::getPluginManager() const
{
    return _pluginManager;
}
