#ifndef CDM_SCRIPTMANAGERXML_H
#define CDM_SCRIPTMANAGERXML_H

#include "cdm_scriptmanagerabstract.h"

class QDomElement;
class QTextStream;

/**
  * \brief Classe permettant d'ecrire/lire un script d'etapes afin de creer les etapes et les ajouter
  *        a un manageur d'etape.
  */
class COMPUTREECORESHARED_EXPORT CDM_ScriptManagerXML : public CDM_ScriptManagerAbstract
{
public:

    CDM_ScriptManagerXML(CDM_PluginManager &pluginManager);

    virtual ~CDM_ScriptManagerXML();

    /*!
     *  \brief Liste les extensions des fichiers de script acceptes
     */
    virtual QList<QString> getFileExtensionAccepted() const;

    /*!
     *  \brief Accepte ce fichier ?
     *
     *  \return Retourne vrai si le script pourra etre lu (si l'extension du fichier est bonne).
     */
    virtual bool acceptFile(const QString &filePath) const;

    /*!
     *  \brief Verifie le script et renvoie un message d'erreur si il y en a une, sinon renvoie ""
     */
    virtual QString verifyScript(const QString &filePath);
    virtual QString verifyScriptText(const QString &text);

    /*!
     *  \brief Charge le script à partir d'un fichier (cree les etapes a l'aide du PluginManager et les ajoutes au StepManager)
     */
    virtual QString loadScript(const QString &filePath,
                               CDM_StepManager &stepManager);

    /*!
     *  \brief Charge le script à partir d'une chaine de caractères (cree les etapes a l'aide du PluginManager et les ajoutes au StepManager)
     */
    virtual QString loadScriptText(const QString &text,
                                   CDM_StepManager &stepManager);

    /*!
     *  \brief Ecrit les identifiants et options de chaque etapes contenu dans le StepManager, a l'aide du PluginManager
     */
    virtual bool writeScript(const QString &filePath,
                             bool saveResult,
                             CDM_StepManager &stepManager);
private:

    int    _valueCount;

    QString recursiveLoadScript(QDomElement &e,
                                CT_VirtualAbstractStep *parent,
                                const QString &fileDirectory,
                                CDM_StepManager *stepManager,
                                CDM_PluginManager *pluginManager,
                                bool verify = false);

    void recursiveWriteScript(QTextStream &stream,
                              CDM_PluginManager &pluginManager,
                              CT_VirtualAbstractStep &step,
                              QString first,
                              bool &saveResult);

    SettingsNodeGroup* recursiveReadSettingsGroup(QDomElement &element);

    void recursiveWriteSettingsGroup(QTextStream &stream,
                                     SettingsNodeGroup *group,
                                     const QString &tab);

    void writeSettingsValue(QTextStream &stream,
                            SettingsNodeValue *value,
                            const QString &tab);
};

#endif // CDM_SCRIPTMANAGERXML_H
