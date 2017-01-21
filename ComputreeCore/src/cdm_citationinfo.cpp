#include "cdm_citationinfo.h"
#include "ct_abstractstepplugin.h"


CDM_CitationInfo::CDM_CitationInfo(CDM_StepManager *stepManager, CDM_PluginManager *pluginManager) : QObject(NULL)
{
    _stepManager = stepManager;
    _pluginManager = pluginManager;
    QList<CT_VirtualAbstractStep*> rootList = stepManager->getStepRootList();
    QListIterator<CT_VirtualAbstractStep*> it(rootList);

    while(it.hasNext())
    {
        CT_VirtualAbstractStep* currentStep = it.next();
        _stepList.append(currentStep);
        recursiveGetStepList(currentStep, _stepList);
    }
}

QString CDM_CitationInfo::getStepByPluginList()
{
    QString str;
    QMultiMap<QString, CT_VirtualAbstractStep*> stepsByPlugins;
    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);

        CT_AbstractStepPlugin* plugin = currentStep->getPlugin();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));
        stepsByPlugins.insert(pluginName, currentStep);
    }

    QList<QString> plugins = stepsByPlugins.uniqueKeys();
    for (int i = 0 ; i < plugins.size() ; i++)
    {
        QString pluginName = plugins.at(i);
        QList<CT_VirtualAbstractStep*> steps = stepsByPlugins.values(pluginName);


        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }

        str += "Plugin: " + pluginName + "\n";

        for (int j = 0 ; j < steps.size() ; j++)
        {
            CT_VirtualAbstractStep* step = steps.at(j);
            str += "\t" + step->getStepName() + "\t" + step->getStepDescription() + "\n";
        }
        str += "\n";
    }
    return str;
}

void CDM_CitationInfo::recursiveGetStepList(CT_VirtualAbstractStep *step, QList<CT_VirtualAbstractStep*> &stepList)
{
    QList<CT_VirtualAbstractStep *> stepChildList = step->getStepChildList();
    QListIterator<CT_VirtualAbstractStep *> it(stepChildList);

    while(it.hasNext())
    {
        CT_VirtualAbstractStep* currentStep = it.next();
        stepList.append(currentStep);
        recursiveGetStepList(currentStep, stepList);
    }
}
