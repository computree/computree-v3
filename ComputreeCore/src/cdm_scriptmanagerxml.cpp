#include "cdm_scriptmanagerxml.h"

#include "ct_abstractstepplugin.h"

#include <QFileInfo>
#include <QDomDocument>
#include <QTextStream>
#include <QDir>

#include <QCoreApplication>

CDM_ScriptManagerXML::CDM_ScriptManagerXML(CDM_PluginManager &pluginManager) : CDM_ScriptManagerAbstract(pluginManager)
{
    setPluginManager(pluginManager);
    _valueCount = 0;
}

CDM_ScriptManagerXML::~CDM_ScriptManagerXML()
{
}

QList<QString> CDM_ScriptManagerXML::getFileExtensionAccepted() const
{
    QList<QString> list;
    list << ".xsct2";

    return list;
}

bool CDM_ScriptManagerXML::acceptFile(const QString &filePath) const
{
    QList<QString> extList = getFileExtensionAccepted();
    QListIterator<QString> it(extList);

    while(it.hasNext())
    {
        QString ext = it.next();

        if(filePath.lastIndexOf(ext) == (filePath.size()-ext.size()))
        {
            return true;
        }
    }

    return false;
}

QString CDM_ScriptManagerXML::verifyScript(const QString &filePath)
{
    QString error = "";

    if(QFile::exists(filePath))
    {
        QFile f(filePath);

        if(f.open(QIODevice::ReadOnly))
        {
            QDomDocument doc;

            if(doc.setContent(&f))
            {
                QDomElement root = doc.documentElement();

                QFileInfo info = QFileInfo(filePath);
                QString fileDirectory = info.absoluteDir().path() + "/" + info.completeBaseName();

                error = recursiveLoadScript(root, NULL, fileDirectory, NULL, getPluginManager(), true);
            }

            f.close();
        }
        else
        {
            error = QCoreApplication::translate("verifyScript", "Le fichier script n'a pas pu etre ouvert.");
        }
    }
    else
    {
        error = QCoreApplication::translate("verifyScript", "Le chemin vers le fichier script est incorrect.");
    }

    return error;
}

QString CDM_ScriptManagerXML::verifyScriptText(const QString &text)
{
    QDomDocument doc;

    if(doc.setContent(text))
    {
        QDomElement root = doc.documentElement();

        return recursiveLoadScript(root, NULL, "", NULL, getPluginManager(), true);
    }

    return QCoreApplication::translate("loadScriptText", "Texte non XML.");
}

QString CDM_ScriptManagerXML::loadScript(const QString &filePath,
                                         CDM_StepManager &stepManager)
{
    QString error = "";

    if(QFile::exists(filePath))
    {
        QFile f(filePath);

        if(f.open(QIODevice::ReadOnly))
        {
            QDomDocument doc;

            QString errMsg;
            int errLine;
            int errCol;

            if(doc.setContent(&f, &errMsg, &errLine, &errCol))
            {
                QDomElement root = doc.documentElement();

                QFileInfo info = QFileInfo(filePath);
                QString fileDirectory = info.absoluteDir().path() + "/" + info.completeBaseName();

                error = recursiveLoadScript(root, NULL, fileDirectory, &stepManager, getPluginManager());
            }
            else
            {
                error = QCoreApplication::translate("verifyScript", "Le fichier script n'a pas pu etre ouvert en tant que document XML :") +
                        "\r\n\r\n" +
                        errMsg +
                        "\r\n\r\n" +
                        QCoreApplication::translate("verifyScript", "Ligne : %1").arg(errLine) +
                        "\r\n" +
                        QCoreApplication::translate("verifyScript", "Colonne : %1").arg(errCol);
            }

            f.close();
        }
        else
        {
            error = QCoreApplication::translate("verifyScript", "Le fichier script n'a pas pu etre ouvert.");
        }
    }
    else
    {
        error = QCoreApplication::translate("verifyScript", "Le chemin vers le fichier script est incorrect.");
    }

    return error;
}

QString CDM_ScriptManagerXML::loadScriptText(const QString &text,
                                             CDM_StepManager &stepManager)
{
    QDomDocument doc;

    if(doc.setContent(text))
    {
        QDomElement root = doc.documentElement();

        return recursiveLoadScript(root, NULL, "", &stepManager, getPluginManager());
    }

    return QCoreApplication::translate("loadScriptText", "Texte non XML.");
}

bool CDM_ScriptManagerXML::writeScript(const QString &filePath,
                                       bool saveResult,
                                       CDM_StepManager &stepManager)
{
    QFile f(filePath);

    if(f.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&f);

        _valueCount = 0;

        stream << "<COMPUTREE_SCRIPT>\r\n";

        QList<CT_VirtualAbstractStep*> rootList = stepManager.getStepRootList();
        QListIterator<CT_VirtualAbstractStep*> it(rootList);

        while(it.hasNext())
        {
            recursiveWriteScript(stream, *getPluginManager(), *(it.next()), "\t", saveResult);
        }

        stream << "</COMPUTREE_SCRIPT>\r\n";

        f.close();

        return true;
    }

    return false;
}

////////////// PRIVATE ////////////////

QString CDM_ScriptManagerXML::recursiveLoadScript(QDomElement &e,
                                                  CT_VirtualAbstractStep *parent,
                                                  const QString &fileDirectory,
                                                  CDM_StepManager *stepManager,
                                                  CDM_PluginManager *pluginManager,
                                                  bool verify)
{
    QString error = "";

    if(pluginManager->isAPluginLoaded())
    {
        CT_VirtualAbstractStep *newParent = NULL;

        if(!e.isNull()
                && (e.tagName() == "Step"))
        {
            QString pluginName = e.attribute("plugin", "");
            QString key = e.attribute("type", "");

            if(pluginName.isEmpty())
            {
                return QCoreApplication::translate("recursiveLoadScript", QString("Le plugin n'est pas renseigne pour l'etape %1. Vous utilisez surement une ancienne version de script.").arg(key).toLatin1());
            }

            CT_AbstractStepPlugin *stepPluginManager = pluginManager->getPlugin(pluginName);

            if(stepPluginManager == NULL)
            {
                return QCoreApplication::translate("recursiveLoadScript", QString("Le plugin %1 pour l'etape %2 ne semble pas etre charge.").arg(pluginName).arg(key).toLatin1());
            }

            CT_VirtualAbstractStep *step = stepPluginManager->getStepFromKey(key);

            if(step == NULL)
            {
                return QCoreApplication::translate("recursiveLoadScript", QString("L'etape %1 n'a pas ete trouve dans le plugin %2.").arg(key).arg(pluginName).toLatin1());
            }

            QDomNodeList childList = e.childNodes();

            if(childList.size() < 2)
            {
                return QCoreApplication::translate("recursiveLoadScript", QString("Erreur de la lecture des paramètres").toLatin1());
            }

            if(!verify)
            {
                CT_VirtualAbstractStep *copyStep = stepPluginManager->createNewInstanceOfStep(*step, parent);

                if(copyStep == NULL)
                {
                    return QCoreApplication::translate("recursiveLoadScript", QString("Erreur lors de la copie de l'etape %1 du plugin %2.").arg(key).arg(pluginName).toLatin1());
                }

                QDomElement childE = childList.at(0).toElement();

                if(childE.tagName() != "S1")
                    return "ERROR S1";

                childE = childE.childNodes().at(0).toElement();

                // lecture des parametres de l'etape
                SettingsNodeGroup *rootSettingsGroup = recursiveReadSettingsGroup(childE);

                if(!copyStep->setAllSettings(rootSettingsGroup))
                {
                    return QCoreApplication::translate("recursiveLoadScript", QString("Erreur lors de l'affectation des parametres a l'etape %1 du plugin %2.").arg(key).arg(pluginName).toLatin1());
                }

                delete rootSettingsGroup;

                CT_AbstractStepSerializable *copyStepLF = dynamic_cast<CT_AbstractStepSerializable*>(copyStep);

                if(copyStepLF != NULL)
                {
                    copyStepLF->setUserData(0, new CDM_ScriptStepObjectUserData(fileDirectory));
                }

                childE = childList.at(1).toElement();

                if(childE.tagName() != "S2")
                    return "ERROR S2";

                childE = childE.childNodes().at(0).toElement();

                // lecture des parametres de serialisation de l'etape
                SettingsNodeGroup *rootSerialisationGroup = recursiveReadSettingsGroup(childE);

                if(!copyStep->setSerializedInformation(rootSerialisationGroup))
                {
                    return QCoreApplication::translate("recursiveLoadScript", QString("Erreur lors de l'affectation des parametres de serialisation a l'etape %1 du plugin %2.").arg(key).arg(pluginName).toLatin1());
                }

                delete rootSerialisationGroup;

                bool debugOn = (e.attribute("debug", "off") == "on");

                copyStep->setDebugModeOn(debugOn);
                copyStep->initAfterConfiguration();

                // ajout a l'arbre
                if(!stepManager->addStep(copyStep, parent))
                {
                    delete copyStep;

                    return QCoreApplication::translate("recursiveLoadScript", QString("Erreur lors de l'ajout de l'etape %1 du plugin %2 au gestionnaire d'etape.").arg(key).arg(pluginName).toLatin1());
                }
                else
                {
                    newParent = copyStep;
                }
            }

            int size = childList.size();
            int i = 2;

            while(error.isEmpty()
                  && (i<size))
            {
                QDomElement StepE = childList.at(i).toElement();
                error = recursiveLoadScript(StepE, newParent, fileDirectory, stepManager, pluginManager, verify);

                ++i;
            }

            return error;
        }

        if(!e.isNull())
        {
            QDomNodeList childList = e.childNodes();

            int size = childList.size();
            int i = 0;

            while(error.isEmpty()
                  && (i<size))
            {
                QDomElement StepE = childList.at(i).toElement();
                error = recursiveLoadScript(StepE, newParent, fileDirectory, stepManager, pluginManager, verify);

                ++i;
            }
        }
    }
    else
    {
        return QCoreApplication::translate("recursiveLoadScript", "Aucun plugin n'est charge");
    }

    return error;
}

void CDM_ScriptManagerXML::recursiveWriteScript(QTextStream &stream,
                                                CDM_PluginManager &pluginManager,
                                                CT_VirtualAbstractStep &step,
                                                QString first,
                                                bool &saveResult)
{
    if(pluginManager.isAPluginLoaded())
    {
        stream << first << "<Step plugin=\"" << pluginManager.getPluginName(step.getPlugin()) << "\" type=\"" << step.getPlugin()->getKeyForStep(step) << "\"";

        stream << " debug=\"" << (step.isDebugModeOn() ? "on" : "off") << "\"";

        stream << " id=\"" << step.uniqueID() << "\">\r\n";

        // ecriture des settings de l'étape
        SettingsNodeGroup *s1Root = new SettingsNodeGroup("S1");
        SettingsNodeGroup *rootSettingsGroup = step.getAllSettings();

        if(rootSettingsGroup != NULL)
            s1Root->addGroup(rootSettingsGroup);

        recursiveWriteSettingsGroup(stream, s1Root, first + "\t");

        delete s1Root;

        SettingsNodeGroup *s2Root = new SettingsNodeGroup("S2");

        // ecriture des settings de sérialisation de l'étape
        if(saveResult)
        {
            SettingsNodeGroup *rootSerialisationGroup = step.getSerializedInformation();

            if(rootSerialisationGroup != NULL)
                s2Root->addGroup(rootSerialisationGroup);
        }

        recursiveWriteSettingsGroup(stream, s2Root, first + "\t");

        delete s2Root;

        // ecriture des etapes filles
        QList<CT_VirtualAbstractStep *> stepChildList = step.getStepChildList();
        QListIterator<CT_VirtualAbstractStep *> it(stepChildList);

        while(it.hasNext())
        {
            recursiveWriteScript(stream, pluginManager, *(it.next()), first + "\t", saveResult);
        }

        stream << first << "</Step>\r\n";
    }
}

SettingsNodeGroup* CDM_ScriptManagerXML::recursiveReadSettingsGroup(QDomElement &element)
{
    if(element.isNull())
        return NULL;

    SettingsNodeGroup *root = new SettingsNodeGroup(element.tagName());

    QDomNodeList list = element.childNodes();
    int size = list.size();

    for(int i=0; i<size; ++i)
    {
        QDomElement node = list.at(i).toElement();

        if(!node.hasAttribute("value"))
        {
            SettingsNodeGroup *group = recursiveReadSettingsGroup(node);

            if(group != NULL)
                root->addGroup(group);
        }
        else
        {
            QString value = node.attribute("value");
            SettingsNodeValue *val = new SettingsNodeValue(node.tagName(), value);
            root->addValue(val);
        }
    }

    return root;
}

void CDM_ScriptManagerXML::recursiveWriteSettingsGroup(QTextStream &stream,
                                                       SettingsNodeGroup *group,
                                                       const QString &tab)
{
    if(group->isValid())
    {
        stream << tab << "<" << group->name() << ">\r\n";

        QString tmpTab = tab + "\t";

        QListIterator<SettingsNodeGroup*> itG(group->groups());

        while(itG.hasNext())
        {
            recursiveWriteSettingsGroup(stream,
                                        itG.next(),
                                        tmpTab);
        }

        QListIterator<SettingsNodeValue*> itV(group->values());

        while(itV.hasNext())
        {
            writeSettingsValue(stream,
                               itV.next(),
                               tmpTab);
        }

        stream << tab << "</" << group->name() << ">\r\n";
    }
}

void CDM_ScriptManagerXML::writeSettingsValue(QTextStream &stream,
                                              SettingsNodeValue *value,
                                              const QString &tab)
{
    if(value->isValid())
    {
        QString additionnalFields = "";
        if (value->name()!="Version")
        {
            additionnalFields = QString(" id=\"%1\"").arg(_valueCount++);
            additionnalFields += QString(" description=\"%1\"").arg(value->description());
        }

        stream << tab << "<" << value->name() << " value=\"" << value->value().toString().replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;") << "\"" << additionnalFields << "></" << value->name() << ">\r\n";
    }
}
