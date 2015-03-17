#ifndef CDM_STEPMANAGEROPTIONS_H
#define CDM_STEPMANAGEROPTIONS_H

#include "computreeCore_global.h"

#include <QString>

/**
  * \brief Classe representant les options du manageur d'etapes.
  */
class COMPUTREECORESHARED_EXPORT CDM_StepManagerOptions
{
public:
    CDM_StepManagerOptions();

    /*!
     *  \brief Defini le chemin d'exportation des resultats et active la serialisation automatique.
     */
    bool enableAutoSave(QString dirPath);

    /*!
     *  \brief Desactive la serialisation automatique
     */
    void disableAutoSave();

    /*!
     *  \brief Defini si il faut ou non supprimer les resultats automatiquement a la fin des traitements
     */
    void setAutoClearResultFromMemory(bool enable);

    /*!
     *  \brief Determine la QLocale utilisée
     */
    void setQLocale(QString locale);

    /*!
     *  \brief QLocale utilisée
     */
    QString getQLocale();

    /*!
     *  \brief Change le nombre de pas a sauter lors du mode debug
     */
    bool changeFastJumpInDebugMode(int nJump);

    /**
     * @brief Time to sleep in ms between two call of ack debug mode
     */
    void setTimeToSleepInAutoDebugMode(int timeInMs);

    /**
     * @brief Number of jump to next valid waitForAckIfInDebugMode()
     */
    void setNJumpInAutoDebugMode(int n);

    /*!
     *  \brief Faut-il serialiser les resultats automatiquement
     */
    bool isAutoSaveEnable() const;

    /*!
     *  \brief Le dossier de sauvegarde des resultats
     */
    QString autoSaveDirPath() const;

    /*!
     *  \brief Faut-il supprimer les resultats automatiquement a la fin des traitements
     */
    bool isAutoClearResultFromMemoryEnable() const;

    /*!
     *  \brief Valeur a utiliser lors de grand saut en mode debug
     */
    int getFastJumpValueInDebugMode() const;

    /**
     * @brief Time to sleep in ms between two call of ack debug mode
     */
    int getTimeToSleepInAutoDebugMode() const;

    /**
     * @brief Number of jump to next valid waitForAckIfInDebugMode()
     */
    int getNJumpInAutoDebugMode() const;

    /*!
     *  \brief Charge les parametres par defaut
     */
    void load();

    /*!
     *  \brief Sauvegarde les parametres
     */
    bool save() const;

private:

    int         _nJumpInFastDebugMode;
    bool        _autoSave;
    QString     _autoSaveDirPath;
    bool        _autoClearMemory;
    QString     _locale;
    int         m_timeToSleepInAutoDebugMode;   // wait in ms before call ackDebugMode
    int         m_nJumpInAutoDebugMode;         // n jump in auto debug mode
};

#endif // CDM_STEPMANAGEROPTIONS_H
