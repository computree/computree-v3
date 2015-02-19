/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/



#ifndef CT_VIRTUALABSTRACTSTEP_H
#define CT_VIRTUALABSTRACTSTEP_H

#include "interfaces.h"
#include "pluginShared_global.h"

#include "serialization.h"

#include <QDateTime>

#if defined(_MSC_VER)
#pragma warning( disable: 4250 )    // disable dominance warning
#endif

class CT_InAbstractModel;

class CT_InAbstractResultModel;
class CT_InResultModelGroup;
class CT_InResultModelGroupToCopy;

class CT_OutAbstractModel;
class CT_OutAbstractSingularItemModel;
class CT_OutAbstractGroupModel;
class CT_OutAbstractItemAttributeModel;

class CT_OutAbstractResultModel;
class CT_OutAbstractResultModelGroup;
class CT_OutResultModelGroup;
class CT_OutResultModelGroupToCopyPossibilities;

class CT_StepConfigurableDialog;
class CT_StepInitializeData;
class CT_ResultInitializeData;
class CT_ResultGroup;

class CT_InResultsByModel;

class CT_InManager;
class CT_InTurnManager;
class CT_TurnIndexManager;

class CT_OutManager;
class CT_OutTurnManager;

class CT_AbstractOutModelCopyAction;

class CT_AbstractStepPlugin;

class PLUGINSHAREDSHARED_EXPORT CT_VirtualAbstractStep : public QObject
{
    Q_OBJECT

public:
    CT_VirtualAbstractStep(CT_StepInitializeData &dataInit);
    virtual ~CT_VirtualAbstractStep();

    /**
     * @brief Inherit this method if you want to return your own tooltip.
     */
    virtual QString getToolTip() const;

    /**
     * @brief Inherit this method if you want to return your own name.
     *
     *        By default call "metaObject()->className()"
     */
    virtual QString getStepName() const;

    /**
     * @brief Inherit this method if you want to return your own extended name.
     *
     *        By default insert the "uniqueID" of the step to the "getStepName()" result
     */
    virtual QString getStepExtendedName() const;

    /**
     * @brief Inherit this method if you want to return your own description.
     *
     *        By default return a empty string
     */
    virtual QString getStepDescription() const;

    /**
     * @brief Inherit this method if you want to return your own detailled description.
     *
     *        By default return a empty string
     */
    virtual QString getStepDetailledDescription() const;

    /**
     * @brief Inherit this method if you want to return your own step URL.
     *
     *        By default return the plugin URL
     */
    virtual QString getStepURL() const;

    /**
     * @brief Inherit this method if you want to initialize something.
     *
     *        Called after you have created the step or after she has been added to the steps tree
     */
    virtual void init();

    /**
     * @brief Called from GUI before step was added to the steps tree and after method "init()"
     *
     *        This method call :
     *          - createPreConfigurationDialog() -> inherit this method to create the pre configuration dialog
     *          - preConfigure()                 -> inherit this method if you want to show a personnalized pre configuration dialog (not created with newStandardPreConfigurationDialog())
     */
    bool showPreConfigurationDialog();

    /**
     * @brief Called from GUI before step was added to the steps tree and after method "showPreConfigurationDialog()"
     *
     *        This method clear all turns and all models before call :
     *          - createInResultModelListProtected() -> inherit this method to add IN models
     */
    bool initInResultModelList();

    /**
     * @brief Called from GUI before step was added to the steps tree and after method "initInResultModelList()"
     *
     *        This method call :
     *          - configureInputResult()        -> inherit this method if you want to initialize something after models has been created and after the pre configuration
     *                                             ATTENTION : if you inherit this method call the method of the superclass first !
     *
     *        and create all IN turns and IN models
     */
    bool showInputResultConfigurationDialog();

    /**
     * @brief Called from GUI before step was added to the steps tree and after method "showInputResultConfigurationDialog()"
     *
     *        This method call :
     *          - createPostConfigurationDialog() -> inherit this method to create the post configuration dialog
     *          - postConfigure()                 -> inherit this method if you want to show a personnalized post configuration dialog (not created with newStandardPostConfigurationDialog())
     */
    bool showPostConfigurationDialog();

    /**
     * @brief Called from GUI before step was added to the steps tree and after method "showPostConfigurationDialog()"
     *
     *        This method call :
     *          - protectedInitAfterConfiguration() -> inherit this method if you want to do something before the step was added to the tree
     *
     *        and create all OUT turns and OUT models if the method "protectedInitAfterConfiguration()" return true
     */
    bool initAfterConfiguration();

    /**
     * @brief Return parameters of dialogs and other configuration to save in a script.
     *
     *  Includes settings of the pre-configuration dialog and of the post-configuration dialog.
     *
     * @warning If you inherit this method call first the method of the superclass and add your own settings to
     *          the "SettingsNodeGroup" returned by the superclass.
     *
     *          Example :
     *
     *          SettingsNodeGroup *root = CT_VirtualAbstractStep::getAllSettings();
     *          SettingsNodeGroup *myGroup = new SettingsNodeGroup("MyStep");
     *          myGroup->addValue(new SettingsNodeValue("MyFirstSetting", ....));
     *          root->addGroup(myGroup);
     *          return root;
     *
     * @return parameters to save
     */
    virtual SettingsNodeGroup* getAllSettings() const;

    /**
     * @brief Load parameters from a script
     *
     *  Includes settings of the pre-configuration dialog and of the post-configuration dialog.
     *
     * @warning If you inherit this method call first the method of the superclass
     *
     *          Example :
     *
     *          CT_VirtualAbstractStep::setAllSettings(settings);
     *          SettingsNodeGroup *myGroup = settings->groupsByTagName("MyStep").first();
     *
     *          QList<SettingsNodeValue*> values = myGroup->valuesByTagName("MyFirstSetting");
     *
     *          if(values.isEmpty())
     *          {
     *              setErrorMessage(1, "MyFirstSetting not found");
     *              setErrorCode(1);
     *              return false;
     *          }
     *
     *          QString mSettings = values.first()->toString();
     *
     *          return true;
     *
     * @return false if it was an error when loaded. The error message is recovered by a call to the method "getErrorMessage( getErrorCode() )"
     */
    virtual bool setAllSettings(const SettingsNodeGroup *settings);

    /**
     * @brief Called from GUI before step was deleted
     *
     *        This method delete the configuration dialog. It is the only place to delete your dialog because this method
     *        was called in the GUI thread.
     *
     * @warning If you inherit this method don't forget to call the method of the superclass
     */
    virtual void aboutToBeDeleted();

    /**
     * @brief Return the gui context in manual mode or debug mode. See method "setGuiContext" for more information.
     */
    GuiContextInterface* getGuiContext() const;

    /*!
     *  \brief Create a copy of this step. You must inherit this method and return a empty copy of your step.
     */
    virtual CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit) = 0;

protected:

    friend class CT_OutManager;
    friend class CT_OutModelCopyActionAddModelItemInGroup;
    friend class CT_OutModelCopyActionAddModelGroupInGroup;
    friend class CT_OutModelCopyActionAddModelItemAttributeInItem;
    friend class CT_OutModelCopyActionRemoveModelItemInGroup;
    friend class CT_OutModelCopyActionRemoveModelGroupInGroup;
    friend class CT_OutModelCopyActionRemoveModelItemAttributeInItem;
    friend class CT_InTurnManager;
    friend class CT_AbstractItem;
    friend class CT_AbstractItemAttribute;
    friend class CT_AbstractItemDrawable;
    friend class CT_AbstractItemGroup;
    friend class CT_ResultItemIterator;
    friend class CT_ResultGroupIterator;
    friend class CT_GroupIterator;
    friend class CT_ItemIterator;
    friend class CT_ModelSearchHelper;

    CT_StepInitializeData       *_data;             /*!< les données d'initialisation */

    /**
     * @brief Redefine this method if you want to init something after all configuration dialog has been called
     * @return true if your initialisation is a success
     */
    virtual bool protectedInitAfterConfiguration() { return true; }

    /************ RESULTAT ET ITEMDRAWABLE EN ENTREE ************/

    /**
     * @brief Added the list of your INPUT models that represent elements that you want to search in your "compute" method. Use the
     *        method "createNewInResultModel(...)" or "createNewInResultModelForCopy(...)" to create a new (standard) INPUT result to
     *        search. Use method "setXXXX" or "addXXX" of the INPUT result model created to add other elements to search.
     */
    virtual void createInResultModelListProtected() = 0;

    /**
     * @brief Add a new model that represent a result to the list of results to search. Call this method in "createInResultModelListProtected"
     * @param model : the model that represent the result to add.
     * @return true if the model is added. qfatal error if the model can not be added !
     * @deprecated use the method "createNewInResultModel" or "createNewInResultModelForCopy"
     */
    Q_DECL_DEPRECATED bool addInResultModel(CT_InAbstractResultModel *model);

    /**
     * @brief Create a new INPUT model that represent a result to search and add it to the list of INPUT models.
     *
     * @param modelName : the unique name of the model (typically a DEF_....). Cannot be empty !
     * @param modelDisplayableName : displayable name of the model (showed in gui dialog/widget). (if empty it takes the resultName parameter).
     * @param description : a description
     * @param recursive : true if you want that the result search possibilities in parent step recursively
     *
     * @return returns the model created. NULL if the unique name already exist. Use method "setXXXX" or "addXXX" to add other elements to search.
     */
    CT_InResultModelGroup* createNewInResultModel(const QString &uniqueName,
                                                  const QString &displayableName = "",
                                                  const QString &description = "",
                                                  bool recursive = false);

    /**
     * @brief Create a new INPUT model that represent a result to search and copy (if you want to copy a result of a step) and add it to the list of INPUT models.
     *
     * @param modelName : the unique name of the model (typically a DEF_....). Cannot be empty !
     * @param modelDisplayableName : displayable name of the model (showed in gui dialog/widget). (if empty it takes the resultName parameter).
     * @param description : a description
     * @param recursive : true if you want that the result search possibilities in parent step recursively
     *
     * @return returns the model created. NULL if the unique name already exist.
     */
    CT_InResultModelGroupToCopy* createNewInResultModelForCopy(const QString &uniqueName,
                                                               const QString &displayableName = "",
                                                               const QString &description = "",
                                                               bool recursive = false);

    /**
     * @brief Set that the step not need input results. If you use this method you can not use methods to add other INPUT models !
     */
    bool setNotNeedInputResult();

    /**
     * @brief Define the maximum number of turn that a user can do. By default 9999. Called this method in your constructor.
     *
     * When you search a result they can be multiple possibilities and one possibility represent one result. You must call the method compute
     * with the first possibility then with the second possibility, etc... One call to the method compute is called a "turn". The user can define
     * in the configuration dialog of INPUT elements that the first turn use the third possibility and a second turn use the first possibility, etc...
     *
     * You can call the method "setUseAllInputResultInOneCompute(true)" if you want that all results of all turn is computed in one time (only one
     * compute). The method must be called in method "createInResultModelListProtected" to be accepted.
     *
     * @param n : number of maximum turn
     * @return true if the number has been set. 0 will return false.
     */
    bool setMaximumTurn(int n);

    /**
     * @brief Returns the maximum turns allowed. See "setMaximumTurn" method for more information.
     */
    int maximumTurn() const;

    /**
     * @brief Returns the index of the current turn. Use this method in your "compute" method if you want to know what is the current turn.
     */
    int currentTurnIndex() const;

    /**
     * @brief Returns the INPUT turns manager
     */
    CT_InTurnManager* getInTurnManager() const;

    /**
     * @brief Returns the INPUT model that represent a result to search that correspond to the unique name passed in parameter.
     * @param uniqueName : the unique name of the model you search. Typically a DEF_.... or result of the method "completeName()"
     *        of a CT_AutoRenameModels object.
     * @return NULL if the model was not found.
     */
    CT_InAbstractResultModel* getInResultModel(const QString &uniqueName) const;

    /**
     * @brief Returns the INPUT model that represent a item to search that correspond to the unique name passed in parameter.
     * @param inResultModel : the model that represent a result in which you want to search the model represented by the unique
     *                        name passed in parameter. Example :
     *
     *                        QList<CT_ResultGroup*> inResultList = getInputResults();
     *                        CT_InAbstractModel *inItemDrawableModel = getInModelForResearch(inResultList.at(X).model(), "InModelName");
     *
     * @param uniqueName : the unique name of the model you search. Typically a DEF_.... or result of the method "completeName()"
     *        of a CT_AutoRenameModels object.
     * @return NULL if the model was not found.
     * @deprecated use the method "PS_MODELS->searchModel(....)"
     */
    Q_DECL_DEPRECATED CT_InAbstractModel* getInModelForResearch(const CT_OutAbstractResultModel *inResultModel, const QString &uniqueName) const;

    /**
     * @brief Returns the INPUT model that represent a item to search that correspond to the unique name passed in parameter.
     * @param inResult : the result in which you want to search the model represented by the unique
     *                   name passed in parameter. Example :
     *
     *                   QList<CT_ResultGroup*> inResultList = getInputResults();
     *                   CT_InAbstractModel *inItemDrawableModel = getInModelForResearch(inResultList.at(X), DEF_InScene);
     *
     * @param uniqueName : the unique name of the model you search. Typically a DEF_.... or result of the method "completeName()"
     *                     of a CT_AutoRenameModels object.
     * @return NULL if the model was not found.
     * @deprecated use the method "PS_MODELS->searchModel(....)"
     */
    Q_DECL_DEPRECATED CT_InAbstractModel* getInModelForResearch(const CT_ResultGroup *inResult, const QString &uniqueName) const;

    /**
     * @brief Returns the INPUT model that represent a item to search that correspond to the unique name passed in parameter.
     * @param inResultUniqueName : the unique name of the model that represent a result in which you want to search the model
     *                             represented by the unique name passed in parameter. Typically a DEF_.... or result of the
     *                             method "completeName()" of a CT_AutoRenameModels object. Example :
     *
     *                             CT_InAbstractModel *inItemDrawableModel = getInModelForResearch(DEF_InResult, 0, DEF_InScene);
     *
     * @param possibilitySelectedIndex : the index of the possibility of the INPUT result that correspond to the INPUT result 'i' (set 0 by default if you don't know)
     * @param uniqueName : the unique name of the model you search. Typically a DEF_.... or result of the method "completeName()"
     *                     of a CT_AutoRenameModels object.
     * @return NULL if the model was not found.
     */
    CT_InAbstractModel* getInModelForResearch(const QString &inResultUniqueName,
                                              const int &possibilitySelectedIndex,
                                              const QString &uniqueName) const;

    /**
     * @brief Returns the list of results for the current turn. Use this method in your "compute" method.
     */
    QList<CT_ResultGroup *> getInputResults() const;

    /**
     * @brief Returns the list of results for the current turn sorted by INPUT models. Use this method in your "compute" method.
     */
    QList<CT_InResultsByModel*> getInputResultsSortedByModels() const;

    /**
     * @brief Returns the list of results for the current turn that correspond to the INPUT model passed in
     *        parameter. Use this method in your "compute" method.
     * @param resultModel : the model that represent a result to search. Example :
     *
     *        CT_InAbstractResultModel *model = getInResultModel(DEF_SearchInResult);
     *        QList<CT_ResultGroup*> inResultList = getInputResultsForModel(model);
     * @deprecated use the method "getInputResultsForModel(QString)"
     */
    Q_DECL_DEPRECATED QList<CT_ResultGroup*> getInputResultsForModel(const CT_InAbstractResultModel *resultModel) const;

    /**
     * @brief Returns the list of results for the current turn that correspond to the INPUT model passed in
     *        parameter. Use this method in your "compute" method.
     * @param inResultUniqueName : the unique name of the model that represent a result to search. Example :
     *
     *        QList<CT_ResultGroup*> inResultList = getInputResultsForModel(DEF_InResult);
     */
    QList<CT_ResultGroup*> getInputResultsForModel(const QString &inResultUniqueName) const;

    /************ RESULTAT ET ITEM EN SORTIE ************/

    /**
     * @brief Return true if the step create a default OUTPUT models list with an invalid INPUT models list. The default OUTPUT
     *        models list is used to add capability to the gui to show a list of results that will create a step.
     *
     *        You can check the result of this method if you have segfault in your "createOutResultModelListProtected()" method
     *        when the plugin is loaded.
     */
    bool isCreateDefaultOutModelActive() const;

    /**
     * @brief Called only one time before we will call the method "createOutResultModelListProtected". This method is not
     *        called for each turn and you can redefine them if you want to make something before (re)create the list of
     *        OUTPUT models (per example init a counter or other elements)
     */
    virtual void preProcessCreateOutResultModelListProtected() {}

    /**
     * @brief Called for each turn to allow the step to create OUPUT models.
     */
    virtual void createOutResultModelListProtected() = 0;

    /**
     * @brief Add a new model that represent a result to the list of results you want to create in your "compute" method.
     *        Call this method in "createOutResultModelListProtected".
     *
     * @param model : the model that represent the result to add.
     * @return true if the model is added. qfatal error if the model can not be added !
     * @deprecated use the method "createNewOutResultModel" or "createNewOutResultModelToCopy"
     */
    Q_DECL_DEPRECATED bool addOutResultModel(CT_OutAbstractResultModelGroup *model);

    /**
     * @deprecated use the method "createNewOutResultModel" or "createNewOutResultModelToCopy"
     */
    Q_DECL_DEPRECATED bool addOutResultModelCopy(CT_InResultModelGroupToCopy *inModel,
                                                 const QList<CT_AbstractOutModelCopyAction*> &actions = QList<CT_AbstractOutModelCopyAction*>());

    /**
     * @brief Create a new OUPUT result model and add it to the list of out model.
     *
     * @param uniqueName : the unique name of the model (typically a DEF_....). If empty it will be generate automatically.
     * @param resultName : result name (optionnal, if empty it takes the uniqueName parameter)
     * @param displayableName : displayable name of the model (showed in gui dialog/widget). (if empty it takes the resultName parameter).
     * @param description : a description
     *
     * @return returns the model created. NULL if the unique name already exist.
     */
    CT_OutResultModelGroup* createNewOutResultModel(const QString &uniqueName = "",
                                                    const QString &resultName = "",
                                                    const QString &displayableName = "",
                                                    const QString &description = "");

    /**
     * @brief Create a new OUPUT result model that represent a copy of a result of another step (a possibility
     *        of an INPUT model defined in your "createInResultModelListProtected" method) and add it to the list of results.
     *
     * @param inModelName : the unique name of the INPUT model (typically a DEF_....) of type CT_InResultModelGroupToCopy. Cannot be empty !
     *
     * @return returns the model created. NULL if the INPUT model was not found.
     */
    CT_OutResultModelGroupToCopyPossibilities* createNewOutResultModelToCopy(const QString &inUniqueName);

    /**
     * @brief Returns the OUTPUT turns manager
     */
    CT_OutTurnManager* getOutTurnManager() const;

    /**
     * @brief Returns the model that represent a result that correspond to the unique name passed in parameter
     * @param uniqueName : the unique name of the model
     * @return NULL if the model was not found
     */
    CT_OutAbstractResultModel* getOutResultModel(const QString &uniqueName) const;

    /**
     * @brief When you want to create an item or an item attribute in your method "compute" you must to add a OUTPUT model to pass it to the
     *        constructor. Use the model returned by this method to create your item :
     *
     *        QList<CT_ResultGroup*> outResultList = getOutResultList();
     *        CT_OutAbstractModel *outModelForCreateCircleInResultX = getOutModelForCreation(outResultList.at(X), DEF_OutCircle);
     *
     *        CT_Circle *circle = new CT_Circle(outModelForCreateCircleInResultX, ...);
     *
     * @param outResult : the result where you want to search the model
     * @param uniqueName : the unique name of the model to search
     * @return NULL if the model was not found
     */
    CT_OutAbstractModel *getOutModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const;
    CT_OutAbstractSingularItemModel* getOutSingularItemModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const;
    CT_OutAbstractGroupModel* getOutGroupModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const;
    CT_OutAbstractItemAttributeModel* getOutItemAttributeModelForCreation(const CT_ResultGroup *outResult, const QString &uniqueName) const;

    /**
     * @brief Returns the list of result that was created for the current turn. Use this method in your "compute" method.
     */
    QList<CT_ResultGroup*> getOutResultList() const;

    /************ CONFIGURATION ************/

    /**
     * @brief Redefine this method if you want to create a pre configuration dialog.
     *
     * The pre configuration dialog will be displayed before the call to the method "createInResultModelListProtected". If you want to create
     * a standard pre configuration dialog use the method "newStandardPreConfigurationDialog(...)" and add elements to the dialog. Settings of a
     * standard dialog will be automatically backuped and restored. If you use your own dialog you must overload method "getAllSettings" and
     * "setAllSettings" to backup/restore settings of your own configuration.
     */
    virtual void createPreConfigurationDialog() {}

    /**
     * @brief Create a new standard pre configuration dialog. Call this method only once ! If you call this method multiple times the last
     *        standard pre configuration dialog will be deleted from memory.
     * @return The standard pre configuration dialog.
     */
    CT_StepConfigurableDialog* newStandardPreConfigurationDialog();

    /**
     * @brief Show the pre configuration dialog.
     *
     * If you want to show your own configuration dialog your must overload this method and show your dialog when this method is called. Don't forget
     * to call the method "setSettingsModified(true)" if your settings is modified (if user accept your dialog).
     *
     * @return true if the settings was modified.
     */
    virtual bool preConfigure();

    /**
     * @brief Show the INPUT elements configuration dialog. This dialog allow the user to select elements that must be used in "compute" method
     *        for searched elements. The dialog is showed in read only mode if the step has childrens or if we can not configure something.
     * @return true if settings is modified.
     */
    virtual bool configureInputResult();

    /**
     * @brief Redefine this method if you want to create a post configuration dialog.
     *
     * The post configuration dialog will be displayed before the call to the method "createOutResultModelListProtected". If you want to create
     * a standard post configuration dialog use the method "newStandardPostConfigurationDialog(...)" and add elements to the dialog. Settings of a
     * standard dialog will be automatically backuped and restored. If you use your own dialog you must overload method "getAllSettings" and
     * "setAllSettings" to backup/restore settings of your own configuration.
     */
    virtual void createPostConfigurationDialog() {}

    /**
     * @brief Create a new standard post configuration dialog. Call this method only once ! If you call this method multiple times the last
     *        standard post configuration dialog will be deleted from memory.
     * @return The standard post configuration dialog.
     */
    CT_StepConfigurableDialog* newStandardPostConfigurationDialog();

    /**
     * @brief Show the post configuration dialog.
     *
     * If you want to show your own configuration dialog your must overload this method and show your dialog when this method is called. Don't forget
     * to call the method "setSettingsModified(true)" if your settings is modified (if user accept your dialog).
     *
     * @return true if the settings was modified.
     */
    virtual bool postConfigure();

    /**
     * @brief Call this method when settings of your step (typically after a configuration dialog is accepted) is modified.
     *
     * A signal is emitted when this variable changed. The method "isSettingsModified()" is used to know if the compute of the
     * step must be restarted.
     *
     * @param modified : true if you want to inform that settings is modified
     */
    virtual void setSettingsModified(bool modified);

    /************ TRAITEMENT ************/

    /**
     * @brief Redefine this method to make your process
     *
     * This method is called for each turn (see "setMaximumTurn" method for more information). If you want to compute all results of all turns in
     * one time you can call method "setUseAllInputResultInOneCompute(true)" in method "createInResultModelListProtected()". In this case the method
     * "compute" will be called only once and you must get results sorted by models with the method "getInputResultsSortedByModels()".
     *
     * @warning Don't forget to call the method "isStopped()" in all loops to allow the user to stop the process.
     *          Don't forget to call the method "setProgress(...)" to inform the user the progression of your process.
     *          Don't forget to call the method "setErrorMessage(...)" if you have an error in your step. Or use the LogInterface "PS_LOG->add....".
     */
    virtual void compute() = 0;

    /**
     * @brief Redefine this method if your step allow modification of elements after the process is completed. The user can make a right click
     *        to your step and click "modify" so this method is called (not in GUI thread but in another thread). It is useful if you want to allow
     *        a user to modify elements manually (show "setManual" method for more information)
     *
     * This method is called for each turn (see "setMaximumTurn" method for more information). If you want to compute all results of all turns in
     * one time you can call method "setUseAllInputResultInOneCompute(true)" in method "createInResultModelListProtected()". In this case the method
     * "modify" will be called only once and you must get results sorted by models with the method "getInputResultsSortedByModels()".
     *
     * @warning Use the method "setModifiable(true)" in your constructor to inform that this method can be called.
     *          Use the method "setManual(true)" in your constructor to make your step manual if you want that user can modify manually elements.
     */
    virtual void modify() {}

    /**
     * @brief Called this method in your "compute" method to inform the progress
     */
    void setProgress(float progress);

    /**
     * @brief Define an error message for an error code (map). Call this method in your constructor if you want.
     * @warning Don't use 0 because it will signify no error.
     */
    void setErrorMessage(int errorCode, const QString &errorMessage);

    /**
     * @brief Set the error code. 0 if it was no error, other if in your "compute" method there was and error.
     */
    void setErrorCode(int errorCode);

    /**
     * @brief Clear all error message
     */
    void clearErrorMessage();

    /**
     * @deprecated This method was not used
     */
    Q_DECL_DEPRECATED void pauseProcessing();

    /************ MODE DEBUG ************/

    /**
     * @brief Define if your step is debuggable. Call this method in your constructor.
     * @param debuggable : true if you want to make your step debuggable
     */
    void setDebuggable(bool debuggable);

    /*!
     *  \brief Set a debug break point ! Attend que l'utilisateur demande de continuer si le mode debug de l'étape est activé et qu'un point
     *         d'arrêt à été défini à ce moment là.
     *
     *  Le mode debug fonctionne ainsi :
     *
     *  - Un compteur (_index_step_step_to_go) est initialisé à 0 lors du lancement du traitement.
     *
     *  - Si l'utilisateur a activé le mode debug de l'étape et a défini un point d'arrêt représenté par un chiffre (int) (par exemple 150).
     *
     *  - A chaque appel à la méthode 'waitForAckIfInDebugMode' le compteur (_index_step_step_to_go) est comparé
     *    au point d'arrêt (_step_by_step_size, dans l'exemple 150). Seulement SI le mode debug est actif.
     *
     *  - Si le compteur et le point d'arrêt sont éguaux alors il y a un appel à la méthode 'preWaitForAckIfInDebugMode',
     *    puis attend que l'utilisateur veuille continuer le traitement, puis appel la méthode 'postWaitForAckIfInDebugMode'.
     *
     *  Pour utiliser le mode debug dans une étape il suffit d'appeler cette méthode et un compteur s'incrémentera pour mettre l'étape en pause
     *  lorsque le compteur et le point d'arrêt (chiffre) défini par l'utilisateur sont égaux. Il vous suffit d'afficher un message
     *  pour informer l'utilisateur de ce qui se passe en redéfinissant la méthode "preWaitForAckIfInDebugMode()".
     */
    void waitForAckIfInDebugMode();

public:
    /**
     * @brief Called from gui before the step will be stopped to go in debug mode (after "waitForAckInDebugMode"). By default do nothing.
     * @warning This method is in the GUI thread !
     */
    virtual void preWaitForAckIfInDebugMode() {}

    /**
     * @brief Called from gui after the user click to go to next debug step. By default do nothing.
     * @warning This method is in the GUI thread !
     */
    virtual void postWaitForAckIfInDebugMode() {}

protected:
    /**
     * @deprecated Use the method currentDebugBreakPointNumber
     */
    Q_DECL_DEPRECATED int stepByStepSize() const;

    /**
     * @brief Returns the number of the break point. In other words the number of turn that the debug mode has made.
     */
    int currentDebugBreakPointNumber() const;

    /************ MODE MANUEL ************/

    /*!
     *  \brief Set if we can call the method "modify" of the step (call this method in your constructor with true if you plan to use the method "modify")
     */
    void setModifiable(bool modifiableOn);

    /*!
     *  \brief Set if that step use the manual mode (call this method in your constructor with true if you plan to use the method "requestManualMode()")
     */
    void setManual(bool manualModeOn);

    /**
     * @brief Call this method if you want to go in manual mode.
     * @warning You must, in the constructor of the step, set that you use the manual mode with the method "setManualMode()" otherwise
     *          this method do nothing !
     */
    void requestManualMode();

    /**
     * @brief Called when manual mode is activate for the first time
     *
     *        Inherit this method if you want to create documents and add actions (you are in GUI thread)
     *
     *        Example :
     *
     *        PMO_MyAction *action = new PMO_MyAction();
     *
     *        GuiContextInterface *context = getGuiContext();
     *        context->actionsManager()->addAction(action);
     *
     *        DocumentInterface *doc = context->documentManager()->new3DDocument();
     *        doc->setCurrentAction(action);
     *
     *          -------
     *
     *        Do nothing by default
     *
     * @warning Only called if a GuiContext exist.
     * @warning Called for each turn
     */
    virtual void initManualMode();

    /**
     * @brief Called every time the manual mode is activated (you are in GUI thread) or quitted
     *
     *        Do nothing by default
     *
     * @warning Only called if a GuiContext exist
     * @param quit : true if it called when quit manual mode, false when enter in manual mode
     */
    virtual void useManualMode(bool quit = false);

    /*!
     *  \brief Retourne le nombre de tour a executer
     */
    int getNTurnToCompute() const;

    /*!
     *  \brief Retourne le tour courant
     */
    int getCurrentTurn() const;

    /*!
     *  \brief Progression lors de la suppression des résultats en mémoire
     *
     *  Defini la progression. Si la progression a changé, met à jour le temps passé et envoie
     *  les signaux de mise à jour (cleanMemoryInProgress, etc...)
     *
     *  Vous devez appeler cette méthode lorsque vous
     */
    void setClearMemoryProgress(float progress);

public:

    /********************************************************************************/
    /************ Not intended for direct use by plugin developper ******************/
    /********************************************************************************/

    /*!
     * @brief Return a unique id for the step
     * @warning Not intended for direct use by plugin developper
     */
    int uniqueID() const;

    /**
     * @brief Return IN models of the step (not used in turns but just for information)
     * @warning Not intended for direct use by plugin developper
     */
    QList<const CT_InAbstractResultModel*> getInResultsModel() const;

    /**
     * @brief Return OUT models of the step (not used in turns but just for information)
     * @warning Not intended for direct use by plugin developper
     */
    QList<const CT_OutAbstractResultModel*> getOutResultsModel() const;

    /**
     * @brief Return true if step must choose between multiple input models
     * @warning Not intended for direct use by plugin developper
     */
    bool hasChoiceBetweenMultipleInputResults();

    /**
     * @brief Return the plugin
     * @warning Not intended for direct use by plugin developper
     */
    CT_AbstractStepPlugin* getPlugin() const;

    /**
     * @brief Method to add, insert, remove, etc... steps
     * @warning Not intended for direct use by plugin developper
     */
    bool appendStep(CT_VirtualAbstractStep *step);
    bool insertStep(int n, CT_VirtualAbstractStep *step);
    bool removeStep(CT_VirtualAbstractStep *step);

    /**
     * @brief Return the childs
     * @warning Not intended for direct use by plugin developper
     */
    QList<CT_VirtualAbstractStep*> getStepChildList() const;

    /**
     * @brief Return the parent
     * @warning Not intended for direct use by plugin developper
     */
    CT_VirtualAbstractStep* parentStep() const;

    /**
     * @brief Return the step root of the hierarchy
     */
    CT_VirtualAbstractStep* rootStep() const;

    /**
     * @brief Return true if this step can be added after the step passed in parameter
     * @warning Not intended for direct use by plugin developper
     */
    bool acceptAddAfterThisStep(const CT_VirtualAbstractStep *step);

    /**
     * @brief Return true if this step must have at least on input result
     * @warning Not intended for direct use by plugin developper
     */
    bool needInputResults() const;

    /**
     * @brief Return true if this step is running, stopped, etc...
     * @warning Not intended for direct use by plugin developper
     */
    bool isRunning() const;
    bool isStopped() const;
    bool isInManualMode() const;

    /**
     * @deprecated This method must not be used
     */
    Q_DECL_DEPRECATED bool isPaused() const;

    /**
     * @brief Returns true if this step want that the core restart this step. By default return true if setting is modified or if
     *        result was cleared from memory.
     * @warning Not intended for direct use by plugin developper
     */
    virtual bool mustBeRestarted() const;

    /**
     * @brief Returns true if this step want that the core recheck all step of the tree. By default return false.
     * @warning Not intended for direct use by plugin developper
     */
    virtual bool mustRecheckTree() const;

    /**
     * @brief Stop the step. Call the method "isStopped()" in loop of your compute to know if you must stop your process !
     * @warning Not intended for direct use by plugin developper
     */
    void stop();

    /**
     * @brief Quit the manual mode
     * @warning Not intended for direct use by plugin developper but you can use this method if you want to quit manual mode.
     */
    void quitManualMode();

    /**
     * @brief Quit the pause mode
     * @warning Not intended for direct use by plugin developper
     * @deprecated This method must not be used
     */
    Q_DECL_DEPRECATED void unPause();

    /**
     * @brief Set the debug mode on/off. Called from gui or core to set the debug mode ON/OFF. If ON the method "waitForAckIfInDebugMode" is active in
     *        your step.
     * @warning Not intended for direct use by plugin developper
     */
    void setDebugModeOn(bool enable);

    /**
     * @brief Return true if this step is debuggable. Use method "setDebuggable()" in the constructor to set the mode.
     * @warning Not intended for direct use by plugin developper
     */
    bool isDebuggable() const;

    /**
     * @brief Return true if this step use the manual mode. Use method "setManual()" in the constructor to set the mode.
     * @warning Not intended for direct use by plugin developper
     */
    bool isManual() const;

    /**
     * @brief Return true if this step use the manual mode and it's modifiable and use only one turn. Use method "setManual()" and "setModifiable" in the
     *        constructor of your step to set the mode ON.
     * @warning Not intended for direct use by plugin developper
     */
    bool isModifiable() const;

    /**
     * @brief Return the number of milliseconds that have elapsed since the method "compute" was called
     * @warning Not intended for direct use by plugin developper
     */
    int getExecuteTime() const;

    /**
     * @brief Return the progress of the method "compute". Use method "setProgress()" to change the progress.
     * @warning Not intended for direct use by plugin developper
     */
    int getProgress() const;

    /**
     * @brief Return the current error code. 0 if no error.
     * @warning Not intended for direct use by plugin developper
     */
    int getErrorCode() const;

    /**
     * @brief Return the error message for the error code passed in parameter
     * @warning Not intended for direct use by plugin developper
     */
    virtual QString getErrorMessage(int error_code) const;

    /**
     * @brief Return the custom name
     * @warning Not intended for direct use by plugin developper
     */
    QString getStepCustomName() const;

    /**
     * @brief Called from GUI to set a custom name
     * @warning Not intended for direct use by plugin developper
     */
    void setStepCustomName(QString customName);

    /**
     * @brief Return the number of results
     * @warning Not intended for direct use by plugin developper
     */
    int nResult() const;

    /**
     * @brief Return all results. Used by GUI !
     * @warning If you want out results in the "compute()" method use "getOutResultList()" ! the method
     *          "getOutResultList()" return results for one turn instead of this method return results for all turns and
     *          only after the method "compute()" was finished.
     * @warning Not intended for direct use by plugin developper
     */
    QList<CT_ResultGroup*> getResults() const;

    /**
     * @brief Return the result at index 'n'. Used by GUI !
     * @warning If you want out results in the "compute()" method use "getOutResultList()" ! the method
     *          "getOutResultList()" return results for one turn instead of this method return results for all turns and
     *          only after the method "compute()" was finished.
     * @warning Not intended for direct use by plugin developper
     */
    CT_ResultGroup* getResult(int n) const;

    /**
     * @brief Clear out result from memory and from the step. Used before the method compute is called or if the step is stopped.
     * @warning Not intended for direct use by plugin developper
     */
    virtual void clearOutResult();

    /**
     * @brief Only clear result only from memory. The results list is not modified. Called from core or gui.
     * @warning Not intended for direct use by plugin developper
     */
    virtual void clearOutResultFromMemory();

private:
    void recursiveClearOutResult();
    void recursiveClearChildOutResult();
    void recursiveClearOutResultFromMemory();

public:

    /**
     * @brief Return true if settings of the step was modified.
     * @warning Not intended for direct use by plugin developper
     */
    virtual bool isSettingsModified() const;

    /**
     * @brief Only clear result from memory. The results list is not modified.
     * @warning Not intended for direct use by plugin developper
     */
    bool isDebugModeOn() const;

    /**
     * @brief Called from CORE to launch the compute of the step.
     *
     *       This method :
     *          - clear out results
     *          - retrieves the in results
     *          - for each turn
     *              - create out results
     *              - call compute
     *
     * @warning Not intended for direct use by plugin developper
     */
    void execute();

    /**
     * @brief Called from CORE to launch the compute of the step but in modification mode.
     *
     *       This method :
     *          - retrieves the in results
     *          - only for the first turn (a modifiable step can only have a maximum of one turn)
     *              - call modify
     *
     * @warning Not intended for direct use by plugin developper
     */
    void executeModify();

    /**
     * @brief Called from GUI when the user click on the button to restart the step after she was in debug mode.
     * @warning Not intended for direct use by plugin developper
     */
    void ackDebugMode(int jump_n_step = 1);

    /**
     * @brief Called from GUI to set a special context when the step go in debug mode or manual mode.
     *
     *  3 possibilities :
     *
     *  - If debug mode is active this method is called before the step compute. After she
     *    finished the method is called with context = NULL.
     *
     *  - Every time the step requires the manual mode this method is called if the GuiContextInterface
     *    returned by the method "getGuiContext()" is different from the current context. After the step
     *    quit the manual mode this method is called with context = NULL.
     *
     *  - If debug mode is active this method is called before the step compute. Every time the step
     *    requires the manual mode this method is called if the GuiContextInterface returned by the
     *    method "getGuiContext()" is different from the current context. After she finished
     *    calculating, the method is called with context = NULL.
     *
     * @warning Not intended for direct use by plugin developper
     */
    void setGuiContext(const GuiContextInterface *context);

    /**
     * @brief Called from GUI after the step request the manual mode. This method is called in the GUI thread.
     * @warning Not intended for direct use by plugin developper
     */
    void ackManualMode();

    /******************* SERIALISATION (DEPRECATED) *********************/

    Q_DECL_DEPRECATED virtual SettingsNodeGroup* getSerializedInformation() const;

    Q_DECL_DEPRECATED virtual bool setSerializedInformation(const SettingsNodeGroup *settings);

    Q_DECL_DEPRECATED int getNumberResultSerialized() const;

    Q_DECL_DEPRECATED QString getRelativeFilepathOfSerializedResults() const;

    Q_DECL_DEPRECATED virtual bool recursiveCanBeDeserialized(QString &dirPath);

    Q_DECL_DEPRECATED void setUserData (uint id, QObjectUserData *data) { Q_UNUSED(id) Q_UNUSED(data) }

    Q_DECL_DEPRECATED QObjectUserData* userData(uint id) const { Q_UNUSED(id) return NULL; }

private:

    friend class CT_InResultModelManager;
    friend class CT_AbstractStepPlugin;
    friend class CT_InResultModelGroup;

    int                             _uniqueId;

    QString                         _customName;

    CT_ResultInitializeData         *_resultInit;

    CT_StepConfigurableDialog       *_preConfigDialog;              /*!< la fenêtre de pré-configuration (avant l'appel à la méthode "createInResultModelListProtected()") */
    CT_StepConfigurableDialog       *_postConfigDialog;             /*!< la fenêtre de post-configuration (après l'appel à la méthode "createInResultModelListProtected()" et avant l'appel à la méthode "createOutResultModelListProtected()") */


    QList<CT_VirtualAbstractStep*>  _stepChildList;                 /*!< la liste des étapes filles */

    CT_InManager                    *_inManager;
    CT_OutManager                   *_outManager;
    CT_TurnIndexManager             *_turnIndexManager;

    bool                            _defaultOutModelCreating;   /*!< true if we create a default out model wit ha invalid in model list */

    bool                            _canChangeSetOneCompute;        /*!< peut-on modifier le nombre de tour maximum ? */

    bool                            _running;               /*!< A true lorque l'étape est en cours de traitement */
    bool                            _stop;                  /*!< Stop le traitement */
    int                             _error_code;            /*!< code d'erreur (0 si pas d'erreur) */
    QMap<int, QString>              _errorMessageMap;       /*!< les messages d'erreurs en fonction du code d'erreur */

    bool                            _paused;                /*!< A true lorsque l'étape est en pause */

    QDateTime                       _launch_time;           /*!< Date et heure de lancement du traitement */
    QTime                           _execute_time;          /*!< Calcul de la durée du traitement */
    int                             _elapsed;               /*!< Durée effective du traitement */

    int                             _progress;              /*!< La progression */
    int                             _progressClearMem;      /*!< La progression lors de la suppression des résultats en mémoire */

    bool                            _isSettingsModified;    /*!< les paramètres ont-ils été modifiées */

    bool                            _debuggable;            /*!< si l'étape est débogable (false par défaut) */
    bool                            _debugMode;             /*!< debug mode on/off */
    bool                            _ackOfDebugMode;        /*!< acquitement en mode debug */

    bool                            m_modifiable;                       /*!< is that step modifiable ? */
    bool                            m_manual;                           /*!< is that step use the manual mode ? */
    bool                            m_firstCallToManualMode;            /*!< is the first call to the method ackManualMode() ? */
    bool                            m_mustQuitManualMode;               /*!< true if we must qui the manual mode */
    GuiContextInterface             *m_guiContext;                      /*!< gui context in manual or debug mode */

    QString                         _result_absolute_path_saved_dir;    /*!< le chemin du fichier ou sont sauvegarde les resultats (absolue) */
    QString                         _result_saved_dir;                  /*!< le chemin du fichier ou sont sauvegarde les resultats (relatif) */
    int                             _n_result_saved;

    qint64                          _step_by_step_size;                 /*!< nombre de pas a pas (utile si on veut se reperer lorsqu'il y a un probleme dans l'algo) */
    qint64                          _index_step_step_to_go;             /*!< on va ignorer l'attente d'un ack du mode pas a pas tant que _step_by_step_size est inferieur a _index_step_step_to_go */

    QString                         _serializedResultsRelativeFilename; /*! le fichier ou sont stocké les résultats (chemin relatif) */
    int                             _nResultSerialized;                 /*! le nombre de résultat sérialisé */

    static int CURRENT_ID;

    /*!
     *  \brief Recherche les possibilités de résultats à partir de l'étape parente si il n'y a pas de tour défini
     */
    void initInputResultConfiguration();

    /*!
     *  \brief Crée la liste des résultats modèles nécessaires en entrée.
     */
    void createInResultModelList();

    /*!
     *  \brief Défini l'étape parente à celle-ci
     */
    void setParentStep(CT_VirtualAbstractStep *parent);

    /*!
     *  \brief Défini la date et l'heure de départ du traitement
     */
    void setLaunchTime(QDateTime launchTime);

    /*!
     *  \brief Lance le traitement
     */
    void runProcessing(bool modificationMode = false);

    /*!
     *  \brief Défini le chemin relatif vers les résultats sérialisés.
     */
    void setRelativeFilepathOfSerializedResults(QString path);

    /*!
     *  \brief Défini le nombre de résultats sérialisés.
     */
    void setNumberResultSerialized(int n);

    /*!
     *  \brief Supprime le resultat 'n' de la mémoire et de la liste
     *
     *  \return true si le résultat a été supprimé (n compris entre 0 et la fin de la liste), false sinon
     */
    bool removeResult(int n);

    /*!
     *  \brief Retourne la liste des résultats modèles de sortie de tous les tours
     */
    QList< QList<CT_OutAbstractResultModelGroup*> > getAllOutResultModels() const;

    /**
     * @brief Called by CT_AbstractStepPlugin to set if he create a default out model with no valid in model list
     */
    void setCreateDefaultOutModelActive(bool active);

    /**
     * @brief return the out manager (used by CT_AbstractStepPlugin)
     */
    CT_OutManager* getOutManager() const;

protected:

#ifdef USE_BOOST_OLD

    /*!
     *  \brief Sérialise les résultats de l'étape puis apelle la méthode pour les
     *         étapes filles.
     */
#ifdef USE_BOOST_BINARY
    bool recursiveSerializeResult(QString &dirPath, QString &relativeFilePath, boost::archive::binary_oarchive &ar, bool clearResultAfter);
#else
    bool recursiveSerializeResult(QString &dirPath, QString &relativeFilePath, boost::archive::text_oarchive &ar, bool clearResultAfter);
#endif

    /*!
     *  \brief Charge les résultats sérialisé puis apelle la méthode pour les
     *         étapes filles.
     */
#ifdef USE_BOOST_BINARY
    bool recursiveLoadSerializedResult(boost::archive::binary_iarchive &ar);
#else
    bool recursiveLoadSerializedResult(boost::archive::text_iarchive &ar);
#endif

#endif

signals:

    /**
     * @brief Emitted when at least one setting was modified
     */
    void settingsModified();

    /**
     * @brief Emitted when the name of the step was modified
     */
    void nameChanged();

    /**
     * @brief Signal the progress of the compute method
     * @param n : progress between 0-100
     */
    void inProgress(int n);

    /**
     * @brief Time elapsed changed since the step has begin the compute. Emitted in compute method.
     * @param elaps : time in ms
     */
    void elapsed(int elaps);

    /**
     * @brief Emitted whenever the step enter in debug mode. The step wait the gui call the method "ackDebugMode" to continue the process.
     */
    void waitForAckInDebugMode();

    /**
     * @brief Manual mode start. This mode provides the ability to edit items manually in documents via actions (see ActionInterface).
     *
     *  Signal emmited when the step enter in manual mode and required a manual mode context from GUI. To finish this mode
     *  the GUI must call the method "quitManualMode" or "stop" of the step. The step can call the method "quitManualMode" and the signal
     *  "manualModeCompleted" is emitted
     */
    void manualModeRequired();

    /**
     * @brief Manual mode completed
     *
     *  Signal emitted when the step quit the manual mode or was stopped in manual mode.
     */
    void manualModeCompleted();

    /**
     * @brief Emitted when the step was in pause mode
     */
    void paused();

    /**
     * @brief Emitted when the process is complete
     */
    void isCompleted();

    /**
     * @brief Emitted when the step want to show a message in gui
     */
    void showMessage(QString mess);

    /**
     * @brief Emitted to inform the progress of clear memory process
     * @param n : progress between 0-100
     */
    void cleanMemoryInProgress(int n);

    /**
     * @brief Emitted when a result is added
     * @param res : the result
     */
    void resultAdded(const CT_AbstractResult *res);

    /**
     * @brief Emitted when the result will cleared its elements from memory
     * @param res : the resut
     * @warning if you want to use method of the result you must connect this signal in Qt::DirectConnection because
     *          the result can be removed from memory when this signal is catched in Qt::QueuedConnection
     */
    void resultToBeClearedFromMemory(const CT_AbstractResult *res);

    /**
     * @brief Emitted when the result will be removed from memory
     * @param res : the resut
     * @warning if you want to use method of the result you must connect this signal in Qt::DirectConnection because
     *          the result will be removed from memory if this signal is catched in Qt::QueuedConnection
     */
    void resultToBeRemoved(const CT_AbstractResult *res);

    /**
     * @brief Emitted when the result will be cleared serialized
     * @param res : the resut
     * @warning if you want to use method of the result you must connect this signal in Qt::DirectConnection because
     *          the result can be removed from memory when this signal is catched in Qt::QueuedConnection
     */
    void resultToBeSerialized(const CT_AbstractResult *res);
};

#endif // CT_VIRTUALABSTRACTSTEP_H
