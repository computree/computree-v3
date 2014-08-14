#ifndef CDM_STEPMANAGER_H
#define CDM_STEPMANAGER_H

#include "computreeCore_global.h"

#include "interfaces.h"

#include "cdm_stepmanageroptions.h"

#include <QThread>
#include <QMutex>
#include <QObjectUserData>

class CDM_ScriptManagerAbstract;
class CDM_ActionsManager;

/**
  * \brief Classe permettant d'ajouter le chemine de serialisation des resultats a chaque
  *        etapes serialise. Ainsi on peut verifier si un resultat a deja ete serialiser
  *        et dans quelle dossier. Utiliser par le ScriptManager aussi.
  */
class COMPUTREECORESHARED_EXPORT CDM_ScriptStepObjectUserData : public QObjectUserData
{
public:
    CDM_ScriptStepObjectUserData(QString path = ".");

    void setSerializedResultDirPath(QString path);
    QString getSerializedResultDirPath() const;

private:

    QString _stepLoadSerializedResultDirPath;
};

/**
  * \brief Classe permettant de gerer l'arbre des etapes, l'execution des etapes et la serialisation des resultats.
  */
class COMPUTREECORESHARED_EXPORT CDM_StepManager : public QThread
{
    Q_OBJECT

public:

    enum ActionType
    {
        ExecuteStep,
        ExecuteModifyStep,
        LoadSerializeResult,
        None
    };

    CDM_StepManager(const CDM_ScriptManagerAbstract *scriptManager,
                    const CDM_ActionsManager *actionManager);
    ~CDM_StepManager();

    void setGuiContext(const GuiContextInterface *guiContext);

    /*!
     *  \brief Definir le manageur de script qui sera utilise pour sauvegarder le script
     *         lors de la serialisation des resultats.
     *
     * Info : Le stepManager ne supprime pas le scriptManager de la mémoire.
     */
    void setScriptManager(const CDM_ScriptManagerAbstract *scriptManager);

    /*!
     *  \brief Retourne le manageur de script utilise.
     */
    CDM_ScriptManagerAbstract* getScriptManager() const;

    /*!
     *  \brief Ajout d'une etape
     *
     *  \param step : L'etape a ajouter.
     *  \param parent : L'etape parente a laquelle ajouter cette etape, ou NULL si l'ajout de l'etape doit se faire a la racine
     *
     *  \return true si l'ajout est effectif, false sinon.
     */
    bool addStep(Step *step, Step *parent = NULL);

    /*!
     *  \brief Insertion d'une etape
     *
     *  \param step : L'etape  insrer.
     *  \param parent : L'etape parente & laquelle ajouter cette etape
     *
     *  \return true si l'ajout est effectif, false sinon.
     */
    bool insertStep(Step *step, Step &parent);

    /*!
     *  \brief Suppression d'une etape
     *
     *  \param step : L'etape a supprimer.
     *
     *  \return true si la suppression est effective, false sinon.
     */
    bool removeStep(Step *step);

    /**
     * @brief Activate or not the debug mode of the step
     */
    void setStepDebugModeOn(Step *step, bool debugModeOn);

    /*!
     *  \brief Retourne la liste des etapes du tronc de l'arbre
     */
    QList<Step*> getStepRootList() const;

    /*!
     *  \brief Changer les options
     */
    bool setOptions(CDM_StepManagerOptions options);

    /*!
     *  \brief Retourne les options
     */
    CDM_StepManagerOptions getOptions() const;

    /*!
     *  \brief Retourne le type d'action que le gestionnaire est en train d'effectuer (execution, chargement des rsultats serialises, etc...)
     */
    CDM_StepManager::ActionType getAction() const;

    /*!
     *  \brief Retourne vrai si lorsque l'on va executer les traitements  partir de l'etape passe en parametre
     *         l'auto sauvegarde va etre desactive.
     */
    bool checkAutoSaveDisabledIfExecuteFromStep(Step &step) const;

    /*!
     *  \brief Retourne vrai si au moins une etape fille de l'etape passe en paramtre est en mode debug (recursif). L'etape en parametre
     *         peut etre NULL, alors le test s'effectura a partir des etapes du tronc de l'arbre.
     */
    bool checkOneStepIsInDebugModeFromStep(Step *step) const;

    /**
     * @brief Return true if the current step is in manual mode
     */
    bool isInManualMode() const;

public slots:

    /*!
     *  \brief Lancement des traitements
     *
     *  \param beginStep : L'etape a partir de laquelle lancer les traitements. NULL si on desire lancer les traitements
     *                     a partir de la premiere etape qui a subit des modifications.
     *
     *  \return false si des traitements sont deja en cours, true si le lancement c'est bien passe.
     */
    bool executeStep(Step *beginStep = NULL);

    /*!
     *  \brief Lancement des traitements manuels
     *
     *  \param beginStep : L'etape a partir de laquelle lancer les traitements. Ne peut être NULL.
     *
     *  \return false si des traitements sont deja en cours, true si le lancement c'est bien passe.
     */
    bool executeModifyStep(Step *beginStep);

    /*!
     *  \brief Lancement des traitements en mode debug OU avancer de 1 pas
     *
     *  \param beginStep : L'etape a partir de laquelle lancer les traitements. NULL si on desire lancer les traitements
     *                     a partir de la premiere etape qui a subit des modifications. EVIDEMMENT : Si une etape attend un ack de debug ce parametre
     *                     n'est pas pris en compte.
     *
     *  \return false si aucun traitements n'attend de ack ou si un lancement en mode non debug a deja ete effectue, true si le lancement c'est bien passe.
     */
    bool executeOrForwardStepInDebugMode(Step *beginStep = NULL);

    /*!
     *  \brief Lancement des traitements en mode debug ou avancer de N pas
     *
     *  \param beginStep : L'etape a partir de laquelle lancer les traitements. NULL si on desire lancer les traitements
     *                     a partir de la premiere tape qui a subit des modifications. EVIDEMMENT : Si une etape attend un ack de debug ce parametre
     *                     n'est pas pris en compte.
     *
     *  \return false si aucun traitements n'attend de ack ou si un lancement en mode non debug a deja ete effectue, true si le lancement c'est bien passe.
     */
    bool executeOrForwardStepFastInDebugMode(Step *beginStep = NULL);

    /**
     * @brief Quit the manual mode of the current step
     */
    bool quitManualMode();

    /*!
     *  \brief Lancement du chargement des resultats de chaque etapes
     *
     *  \param childBeginStep : L'etape a partir de laquelle charger les resultats. Elle ne doit pas avoir d'etape
     *                          parente sinon la methode renvoie false.
     *
     *  \return false si des traitements sont deja en cours, true si le lancement c'est bien passe.
     */
    bool loadSerializedResultFromStep(StepSerializable &childBeginStep);

    /*!
     *  \brief Stop tous les traitements
     */
    void stop();

    /*!
     *  \brief Change le nombre de pas a sauter lors de la lecture rapide en mode debug
     */
    bool setFastForwardJumpInDebugMode(int value);

private:

    bool internalExecuteStep(Step *beginStep, bool debugMode);
    bool internalExecuteModifyStep(Step *beginStep, bool debugMode);

    /*!
     * \brief Connexion/Deconnexion des signaux de l'etape aux signaux/slot de cet objet
     */
    void connectStep(Step *step);
    void connectStepBeforeRunning(Step *step);
    void disconnectStepAfterRunning(Step *step);

    void run();

    CDM_ScriptManagerAbstract   *_scripManager;
    CDM_ActionsManager          *m_actionsManager;
    GuiContextInterface         *m_guiContext;

    CDM_StepManagerOptions      _options;

    ActionType                  _action;
    Step                        *_beginStep;
    bool                        _stop;
    bool                        _debugMode;

    QList<Step*>                _stepRootList;
    Step*                       m_currentStep;

    QMutex                      _mutex;

    bool recursiveExecuteStep(QString &scriptFileAndSerializedDirName, Step &step, QList<Result*> results, bool force = false, bool &restart);

    void recursiveClearResult(Step &step);

    bool ackDebugMode(int jumpNStep);

private slots:

    void slotStepWaitForAckInDebugMode();
    void slotStepRequiredManualMode();
    void slotStepManualModeCompleted();

    void setDefaultQLocale(QString name);
    void slotRemoveActionsAfterStepExecuted();

signals:

    void stepAdded(Step *step);
    void stepInserted(int n, Step *step);
    void stepToBeRemoved(Step *step);
    void stepBeginExecuted(Step *step);
    void stepFinishExecuted(Step *step);
    void stepWaitForAckInDebugMode(bool aStepWait);
    void stepRequiredManualMode(Step *step);
    void stepInManualMode(bool val);
    void stepQuitManualMode(Step *step);
    void stepNeedShowMessage(QString message);

    void resultAdded(Result *res);
    void resultToBeClearedFromMemory(Result *res);
    void resultToBeRemoved(Result *res);
    void resultToBeSerialized(Result *res);

    void started(bool started);
    void loadResultBegin();
    void loadResultInProgress(int progress);
    void completed(bool completed = true);
};

#endif // CDM_STEPMANAGER_H
