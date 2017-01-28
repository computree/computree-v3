#include "cdm_citationinfo.h"
#include "ct_abstractstepplugin.h"

#include <QDebug>


CDM_CitationInfo::CDM_CitationInfo(CDM_StepManager *stepManager, CDM_PluginManager *pluginManager) : QObject(NULL)
{
    _computreeCitationRIS = "TY  - COMP\n"
                            "TI  - Computree platform\n"
                            "AU  - Computree Core Team\n"
                            "PB  - ONF RDI Department\n"
                            "PY  - 2017\n"
                            "UR  - http://rdinnovation.onf.fr/computree\n"
                            "ER  - \n";

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


QList<CDM_CitationInfo::StepCitationInfo> CDM_CitationInfo::getScriptTable()
{
    QList<CDM_CitationInfo::StepCitationInfo> list;

    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);

        CT_AbstractStepPlugin* plugin = currentStep->getPlugin();
        QString pluginOfficialName = plugin->getPluginOfficialName();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));

        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }
        if (pluginOfficialName != "")
        {
            pluginName = pluginOfficialName;
        }

        list.append(CDM_CitationInfo::StepCitationInfo(currentStep->uniqueID(), currentStep->getStepName(), currentStep->getStepDescription(), pluginName));
    }
    return list;
}

QString CDM_CitationInfo::getPluginAndStepCitations()
{
    QList<CT_AbstractStepPlugin*> pluginDone;

    QString str;

    str.append(parseRIS(_computreeCitationRIS));
    str.append("<br>");

    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);

        CT_AbstractStepPlugin* plugin = currentStep->getPlugin();
        QString pluginOfficialName = plugin->getPluginOfficialName();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));
        QString stepName = currentStep->getStepName();

        if (pluginName.left(5) == "plug_")
        {
            pluginName.remove(0, 5);
        }
        if (pluginOfficialName != "")
        {
            pluginName = pluginOfficialName;
        }

        if (!pluginDone.contains(plugin))
        {
            pluginDone.append(plugin);

            str.append(QString("[<b>Plugin %1</b>]<br>").arg(pluginName));
            QString pluginRIS = plugin->getPluginRISCitation();
            if (pluginRIS == "")
            {
                str.append(tr("<em>No official citation was provided</em><br>"));
            } else {
                str.append(parseRIS(pluginRIS));
            }
            str.append("<br>");
        }

        QStringList stepsRis = currentStep->getStepRISCitations();
        if (stepsRis.size() > 0)
        {
            str.append(QString("[<b>%1 - in plugin %2</b>]<br>").arg(stepName).arg(pluginName));
        }

        for (int i = 0 ; i < stepsRis.size() ; i++)
        {
            str.append(parseRIS(stepsRis.at(i)));
            str.append("<br>");
        }
    }

    return str;
}

QString CDM_CitationInfo::getPluginRIS()
{
    QString str;

    str.append(_computreeCitationRIS);

    QList<QString> stepCitations;

    QMap<QString, CT_AbstractStepPlugin*> plugins;
    for (int i = 0 ; i < _stepList.size() ; i++)
    {
        CT_VirtualAbstractStep* currentStep = _stepList.at(i);
        CT_AbstractStepPlugin* plugin = currentStep->getPlugin();
        QString pluginName = _pluginManager->getPluginName(_pluginManager->getPluginIndex(plugin));

        plugins.insert(pluginName, plugin);

        QStringList stepsRis = currentStep->getStepRISCitations();
        for (int i = 0 ; i < stepsRis.size() ; i++)
        {
            QString ris = stepsRis.at(i);
            if (!stepCitations.contains(ris))
            {
                stepCitations.append(ris);
            }
        }

    }

    QMapIterator<QString, CT_AbstractStepPlugin*> it(plugins);
    while (it.hasNext())
    {
        it.next();
        str.append(it.value()->getPluginRISCitation());
    }

    for (int i = 0 ; i < stepCitations.size() ; i++)
    {
        str.append(stepCitations.at(i));
    }

    return str;
}

QString CDM_CitationInfo::parseRIS(QString ris)
{    
    QMultiMap<QString, QString> fields;
    QStringList lines = ris.split("\n", QString::SkipEmptyParts);

    for (int i = 0 ; i < lines.size() ; i++)
    {
        QString line = lines.at(i);

        QString fieldValue = line.mid(0,2);
        int cpt = 2;
        while (line.mid(cpt,1) == " " || line.mid(cpt,1) == "-")
        {
            ++cpt;
        }

        QString value = line.mid(cpt);

        if (fieldValue.size() == 2)
        {
            fields.insert(fieldValue.toUpper(), value);
        }
    }

    QString str;

    if (fields.contains("AU") || fields.contains("A1") || fields.contains("PY"))
    {
        str.append("<b>");
    }

    QList<QString> authors = fields.values("AU");
    authors.append(fields.values("A1"));
    for (int i = 0 ; i < authors.size() ; i++)
    {

        QString author = authors.at(i);
        author.replace(", ", " ");
        author.replace(",", " ");
        str.append(author);
        if (i < authors.size() - 1)
        {
            str.append(", ");
        }
    }

    if (str.size() > 0)
    {
        str.append(". ");
    }

    if (fields.contains("PY"))
    {
        str.append(fields.values("PY").first());
        str.append(". ");
    }else if (fields.contains("Y1"))
    {
        str.append(fields.values("Y1").first());
        str.append(". ");
    }

    if (str.size() > 0)
    {
        str.append("</b>");
    }

    if (fields.contains("TI"))
    {
        str.append("<em>");
        str.append(fields.values("TI").first());
        str.append("</em>. ");
    } else if (fields.contains("T1"))
    {
        str.append("<em>");
        str.append(fields.values("T1").first());
        str.append("</em>. ");
    }

    if (fields.contains("PB"))
    {
        str.append(fields.values("PB").first());
        str.append(". ");
    }
    if (fields.contains("UR"))
    {
        str.append("<br><a href=\"");
        str.append(fields.values("UR").first());
        str.append("\">");
        str.append(fields.values("UR").first());
        str.append("</a>. ");
    }
    str.append("<br>");

    return str;
}


