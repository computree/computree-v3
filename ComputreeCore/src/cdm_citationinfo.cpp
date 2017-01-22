#include "cdm_citationinfo.h"
#include "ct_abstractstepplugin.h"

#include <QDebug>


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
    QString str = "@misc{plugONF,"
                  "title={Plugin ONF for Computree},"
                  "author={Alexandre Piboule},"
                  "organization = {ONF RDI Department},"
                  "year={2017},"
                  "url = {http://rdinnovation.onf.fr/projects/plugin-onf}}";
    parseBibTex(str);
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


QString CDM_CitationInfo::getStepByPluginList()
{
    QString str;
    QMultiMap<QString, CT_VirtualAbstractStep*> stepsByPlugins;
    QMap<QString, QString> officialNames;
    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);

        CT_AbstractStepPlugin* plugin = currentStep->getPlugin();
        QString pluginOfficialName = plugin->getPluginOfficialName();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));
        stepsByPlugins.insert(pluginName, currentStep);
        officialNames.insert(pluginName, pluginOfficialName);
    }

    QList<QString> plugins = stepsByPlugins.uniqueKeys();
    for (int i = 0 ; i < plugins.size() ; i++)
    {
        QString pluginName = plugins.at(i);
        QString pluginOfficialName = officialNames.value(pluginName);
        QList<CT_VirtualAbstractStep*> steps = stepsByPlugins.values(pluginName);

        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }

        if (pluginOfficialName != "")
        {
            pluginName = pluginOfficialName;
        }

        str += "<b>Plugin: " + pluginName + "</b><br>";

        for (int j = 0 ; j < steps.size() ; j++)
        {
            CT_VirtualAbstractStep* step = steps.at(j);
            str += step->getStepName() + "&emsp;" + step->getStepDescription() + "<br>";
        }
        str += "<br>";
    }
    return str;
}

QString CDM_CitationInfo::getPluginBibTex()
{
    QString str;
    QMap<QString, CT_AbstractStepPlugin*> plugins;
    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);
        CT_AbstractStepPlugin* plugin = currentStep->getPlugin();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));

        plugins.insert(pluginName, plugin);
    }

    QMapIterator<QString, CT_AbstractStepPlugin*> it(plugins);
    while (it.hasNext())
    {
        it.next();
        str.append(it.value()->getPluginBibTexCitation());
    }

    return str;
}

QString CDM_CitationInfo::parseBibTex(QString bibTex)
{

    bibTex.remove("{");
    bibTex.remove("}");
    bibTex.remove("@");
    bibTex.replace(" = ", "=");
    bibTex.replace("= ", "=");
    bibTex.replace(" =", "=");
    QStringList fields = bibTex.split(",", QString::SkipEmptyParts);
    qDebug() << fields;
    return "";
}


