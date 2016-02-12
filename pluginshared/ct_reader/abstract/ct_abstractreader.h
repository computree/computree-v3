#ifndef CT_ABSTRACTREADER_H
#define CT_ABSTRACTREADER_H

#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/ct_fileheader.h"
#include "ct_step/tools/menu/ct_stepsmenu.h"

#define READER_COPY_FULL_IMP(argClass) virtual CT_AbstractReader *copyFull() const { return new argClass(*this); }

class PLUGINSHAREDSHARED_EXPORT CT_AbstractReader : public QObject
{
    Q_OBJECT

public:
    CT_AbstractReader();
    CT_AbstractReader(const CT_AbstractReader &other);
    virtual ~CT_AbstractReader();

    /**
      * \brief Init the reader (add readable formats, add out itemdrawable model, etc...)
      */
    void init(bool initOutItemDrawableList = true);

    /**
     * @brief Returns a displayable name of the reader
     */
    virtual QString GetReaderName() const;

    /**
     * @brief Returns the class name (you can use it to compare readers because it was unique)
     */
    virtual QString GetReaderClassName() const;

    /**
     * @brief Returns the sub menu level where we can store this reader (by default "Others")
     */
    virtual CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    /**
      * @brief Set the filepath of the file to read.
      * @warning You must NOT change parameter of your class in this method, just check if the file is valid ! Change your parameter in method
      *          "configure()" if you want.
      */
    virtual bool setFilePath(const QString &filepath);

    /**
      * @brief Return the filepath of the file
      */
    QString filepath() const;

    /**
     * @brief Enable/Disable the modification of the filepath
     */
    void setFilePathCanBeModified(bool enable);

    /**
     * @brief Return true if the filepath can be modified
     */
    bool filePathCanBeModified() const;

    /** \brief Check the validity of the file
     *
     * \return A file is valid if a header has been read (=> after setFilePath)
     */
    bool isValid();

    // By default CT_AbstractReader don't have a Bounding Box : redefine in children class of geographical files
    virtual bool hasBoundingBox() {return false;}

    /**
      * \brief Return the header object
      *
      * The returned header has to be deleted by the object taking it
      *
      */
    CT_FileHeader *takeHeaderCopy(const CT_AbstractResult *result, CT_OutAbstractItemModel *model);
    CT_FileHeader *takeHeaderCopy(const CT_AbstractResult *result, const QString &outModelName);

    /**
      * \brief Return the header object (read only)
      *
      */
    const CT_FileHeader* getHeader();


    /**
      * \brief Configure the reader.
      *
      *        Called after the method "setFilePath" and before the method "protectedCreateOutItemDrawableModelList"
      *
      * \return false if the user as canceled the configuration
      */
    virtual bool configure() { return true; }

    /**
     * @brief Backup settings
     * @return The SettingsNodeGroup to save or NULL if it was nothing to save
     * @overload Overloaded from CT_AbstractStepSerializable
     */
    virtual SettingsNodeGroup* getAllSettings() const;

    /**
     * @brief Restore settings
     * @return False if it was a problem in settings
     * @overload Overloaded from CT_AbstractStepSerializable
     */
    virtual bool setAllSettings(const SettingsNodeGroup *settings);

    /**
      * \brief Init out itemdrawable model of the reader (call this method after set the filepath)
      */
    void createOutItemDrawableModelList();

    /**
      * \brief Return the list of out itemdrawable model
      */
    const QList<CT_OutStdSingularItemModel*>& outItemDrawableModels() const;

    /**
      * \brief Return the list of out group model
      */
    const QList<CT_OutStdGroupModel*>& outGroupsModel() const;

    /**
      * \brief Return all readable formats
      */
    const QList<FileFormat> &readableFormats() const;

    /**
     * @brief Return true if it was a read error
     */
    bool isReadError() const;

    /**
     * @brief Return true if the read is stopped
     */
    bool isStopped() const;

    /**
     * @brief Returns a tooltip to show in GUI. None by default.
     */
    QString toolTip() const;

    /**
     * @brief Return the error message. You can set them by a call to the method setErrorMessage()
     */
    QString errorMessage() const;

    /**
     * @brief Clear the error message
     */
    void clearErrorMessage();

    /**
     * @brief Return the list of out ItemDrawable created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. Set the model and change the result of the ItemDrawable returned if result != NULL or model != NULL.
     *
     * @example QList<CT_AbstractSingularItemDrawable*> scenes = takeItemDrawableOfType(CT_Scene::staticGetType()); // if you want to change the model and the result yourself
     * @example QList<CT_AbstractSingularItemDrawable*> scenes = takeItemDrawableOfType(CT_Scene::staticGetType(), outResult, outSceneModel); // if you want to change the model and the result in this method
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    QList<CT_AbstractSingularItemDrawable*> takeItemDrawableOfType(const QString &type, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the list of out ItemDrawable created in the method "readFile" that have the modelName passed in parameter and remove
     *        them from this reader. Set the model and change the result of the ItemDrawable returned by this method if result != NULL or model != NULL.
     *
     * @example QList<CT_AbstractSingularItemDrawable*> scenes = takeItemDrawableOfModel(DEF_Reader_XXX_OutScene); // if you want to change the model and the result yourself
     * @example QList<CT_AbstractSingularItemDrawable*> scenes = takeItemDrawableOfModel(DEF_Reader_XXX_OutScene, outResult, outSceneModel); // if you want to change the model and the result in this method
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    QList<CT_AbstractSingularItemDrawable*> takeItemDrawableOfModel(const QString &modelName, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the list of out ItemDrawable created in the method "readFile" that have the modelName passed in parameter and remove
     *        them from this reader. The outModelName is used to search your model in your step and set it (with the result) to all itemdrawable returned.
     *
     * @example QList<CT_AbstractSingularItemDrawable*> scenes = takeItemDrawableOfModel(DEF_Reader_XXX_OutScene, outResult, DEF_MyOutScene);
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    QList<CT_AbstractSingularItemDrawable*> takeItemDrawableOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName);

    /**
     * @brief Return the first ItemDrawable in the list that was created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. Set the model and change the result of the ItemDrawable returned if result != NULL or model != NULL.
     *
     * @example CT_AbstractSingularItemDrawable *scene = takeFirstItemDrawableOfType(CT_Scene::staticGetType()); // if you want to change the model and the result yourself
     * @example CT_AbstractSingularItemDrawable *scene = takeFirstItemDrawableOfType(CT_Scene::staticGetType(), outResult, outSceneModel); // if you want to change the model and the result in this method
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    CT_AbstractSingularItemDrawable* takeFirstItemDrawableOfType(const QString &type, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the first ItemDrawable in the list that was created in the method "readFile" that have the modelName passed in parameter and remove
     *        them from this reader. Set the model and change the result of the ItemDrawable returned if result != NULL or model != NULL.
     *
     * @example CT_AbstractSingularItemDrawable *scene = takeFirstItemDrawableOfModel(DEF_Reader_XXX_OutScene); // if you want to change the model and the result yourself
     * @example CT_AbstractSingularItemDrawable *scene = takeFirstItemDrawableOfModel(DEF_Reader_XXX_OutScene, outResult, outSceneModel); // if you want to change the model and the result in this method
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    CT_AbstractSingularItemDrawable* takeFirstItemDrawableOfModel(const QString &modelName, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the first ItemDrawable in the list that was created in the method "readFile" that have the modelName passed in parameter and remove
     *        them from this reader. The outModelName is used to search your model in your step and set it (with the result) to the itemdrawable returned.
     *
     * @example CT_AbstractSingularItemDrawable *scene = takeFirstItemDrawableOfModel(DEF_Reader_XXX_OutScene, outResult, DEF_MyOutScene); // if you want to change the model and the result in this method
     *
     * @warning this method remove ItemDrawable from the reader !
     */
    CT_AbstractSingularItemDrawable* takeFirstItemDrawableOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName);

    /**
     * @brief Return the list of ItemDrawable created in the method "readFile" that have the type passed in parameter.
     *
     *        example : itemDrawableOfType( CT_Scene::staticGetType() )
     *
     * @warning this method DON'T remove ItemDrawable from the reader, it will be deleted by the reader in the destructor !
     */
    QList<CT_AbstractSingularItemDrawable*> itemDrawableOfType(const QString &type) const;

    /**
     * @brief Return the list of ItemDrawable created in the method "readFile" that have the modelName passed in parameter.
     *
     *        example : itemDrawableOfModel( DEF_Reader_XXX_OutScene )
     *
     * @warning this method DON'T remove ItemDrawable from the reader, it will be deleted by the reader in the destructor !
     */
    QList<CT_AbstractSingularItemDrawable*> itemDrawableOfModel(const QString &modelName) const;

    /**
     * @brief Return the first ItemDrawable created in the method "readFile" that have the modelName passed in parameter.
     *
     *        example : firstItemDrawableOfModel( DEF_Reader_XXX_OutScene )
     *
     * @warning this method DON'T remove ItemDrawable from the reader, it will be deleted by the reader in the destructor !
     */
    CT_AbstractSingularItemDrawable* firstItemDrawableOfModel(const QString &modelName) const;

    /**
     * @brief Return the list of out Group created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. Set the model and change the result of the Group returned if result != NULL or model != NULL.
     *
     * @warning this method remove Group from the reader !
     */
    QList<CT_AbstractItemGroup*> takeGroupOfModel(const QString &modelName, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the list of out Group created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. The outModelName is used to search your model in your step and set it (with the result) to all groups returned.
     *
     * @warning this method remove Group from the reader !
     */
    QList<CT_AbstractItemGroup*> takeGroupOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName);

    /**
     * @brief Return the first Group in the list that was created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. Set the model and change the result of the Group returned if result != NULL or model != NULL.
     *
     * @warning this method remove Group from the reader !
     */
    CT_AbstractItemGroup* takeFirstGroupOfModel(const QString &modelName, const CT_AbstractResult *result = NULL, CT_OutAbstractItemModel *model = NULL);

    /**
     * @brief Return the first Group in the list that was created in the method "readFile" that have the type passed in parameter and remove
     *        them from this reader. The outModelName is used to search your model in your step and set it (with the result) to the group returned.
     *
     * @warning this method remove Group from the reader !
     */
    CT_AbstractItemGroup* takeFirstGroupOfModel(const QString &modelName, const CT_AbstractResult *result, const QString &outModelName);

    /**
     * @brief Return the list of Group created in the method "readFile" that have the type passed in parameter.
     *
     * @warning this method DON'T remove Group from the reader, it will be deleted by the reader in the destructor !
     */
    QList<CT_AbstractItemGroup*> groupOfModel(const QString &modelName) const;

    /**
     * @brief Return a empty copy of the reader
     */
    virtual CT_AbstractReader *copy() const = 0;

    /**
     * @brief Returns a copy with all parameters set
     */
    virtual CT_AbstractReader *copyFull() const = 0;

public slots:

    /**
     * @brief Read the file
     * @return Return false if it was a problem when read the file or you can check later with the method isReadError()
     */
    bool readFile();

    /**
     * @brief Cancel the reading
     */
    void cancel();

protected:

    /**
      * \brief Add a new readable format
      */
    void addNewReadableFormat(const FileFormat &format);

    /**
     * @brief Set the tooltip to show in gui
     */
    void setToolTip(const QString &t);

    /**
     * @brief Add the ItemDrawable you will create in result (it's a model). If you create multiple times the
     *        same ItemDrawable you must add multiple model with different name.
     */
    void addOutItemDrawableModel(CT_OutStdSingularItemModel *item);
    void addOutItemDrawableModel(const QString &modelName,
                                 CT_AbstractSingularItemDrawable *item,
                                 const QString &displayableName = "",
                                 const QString &description = "");

    /**
     * @brief Add a new out ItemDrawable you have created in the method "protectedReadFile" for the model name "modelName"
     */
    void addOutItemDrawable(const QString &modelName, CT_AbstractSingularItemDrawable *item);

    /**
     * @brief Add the Group you will create in result (it's a model). If you create multiple times the
     *        same Group you must add multiple model with different name.
     */
    void addOutGroupModel(CT_OutStdGroupModel *gr);

    /**
     * @brief Add a new out Group you have created in the method "protectedReadFile" for the model name "modelName"
     */
    void addOutGroup(const QString &modelName, CT_AbstractItemGroup *gr);

    /**
     * @brief Returns true if the model with the unique name is contained in this reader
     */
    bool containsItemDrawableModel(const QString &uniqueName) const;

    /**
      * \brief Set the progression when you read the file
      */
    void setProgress(int progress);

    /**
      * \brief Set a error message
      */
    void setErrorMessage(const QString &err);

    /**
     * @brief Init your reader (add readable formats, etc...)
     */
    virtual void protectedInit() = 0;

    /*!
     *  \brief In this method you must add all ItemDrawable you create in result. Use the method "addOutItemDrawableModel". Called
     */
    virtual void protectedCreateOutItemDrawableModelList();

    /**
     * @brief Inherit this method to read the file
     */
    virtual bool protectedReadFile() = 0;

    CT_FileHeader*                                          m_header;

private:
    QList<FileFormat>                                       m_formats;
    QString                                                 m_tooltip;
    QString                                                 m_errorMess;
    QList<CT_OutStdSingularItemModel*>                      m_outItemsModel;
    QList<CT_OutStdGroupModel*>                             m_outGroupsModel;
    QMultiMap<QString, CT_AbstractSingularItemDrawable*>    m_outItems;
    QMultiMap<QString, CT_AbstractItemGroup*>               m_outGroups;
    int                                                     m_progress;
    bool                                                    m_error;
    bool                                                    m_stop;
    QString                                                 m_filePath;
    bool                                                    m_filepathCanBeModified;

    void clearOutItemDrawableModel();
    void clearOutItemDrawable();

    void clearOutGroupsModel();
    void clearOutGroups();

signals:
    void started();
    void progressChanged(int progress);
    void finished();

    void filePathModified();
};

#endif // CT_ABSTRACTREADER_H
