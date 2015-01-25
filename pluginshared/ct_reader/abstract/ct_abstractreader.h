#ifndef CT_ABSTRACTREADER_H
#define CT_ABSTRACTREADER_H

#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/ct_fileheader.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractReader : public QObject
{
    Q_OBJECT

public:
    CT_AbstractReader();
    virtual ~CT_AbstractReader();

    /**
      * \brief Init the reader (add readable formats, add out itemdrawable model, etc...)
      */
    void init(bool initOutItemDrawableList = true);

    /**
     * \brief Inherit this method if you want to return your own name.
     *
     *        By default call "metaObject()->className()"
     */
    QString GetReaderName();

    /**
      * \brief Set the filepath of the file to read
      */
    virtual bool setFilePath(const QString &filepath);

    /**
      * \brief Return the filepath of the file
      */
    QString filepath() const;

    /**
      * \brief Return the header object
      *
      * \param deleteHeader If set true, the reader will delete the header object in destructor
      *
      */
    CT_FileHeader* getHeader(bool deleteHeader);

    const CT_FileHeader* getHeader();

    /**
      * \brief Return a const reference on the header
      *
      */
    const CT_FileHeader& getConstHeader() const;

    /**
      * \brief Configure the reader.
      *
      *        Called after the method "setFilePath" and before the method "protectedCreateOutItemDrawableModelList"
      *
      * \return false if the user as canceled the configuration
      */
    virtual bool configure() { return true; }

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
      * \brief Return the out itemdrawable model for header
      */
    const CT_OutStdSingularItemModel* outHeaderModel() const;

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
     * @brief Called by derived class in "protectedReadFile()" to inform that this reader
     *        don't used the coordinate system
     */
    void setNotNeedToUseCoordinateSystem();

    /**
     * @brief Set the tooltip to show in gui
     */
    void setToolTip(const QString &t);

    /**
      * \brief Set the out itemdrawable model for header
      */
    const void setOutHeaderModel(CT_OutStdSingularItemModel *headerModel);

    /**
     * @brief Add the ItemDrawable you will create in result (it's a model). If you create multiple times the
     *        same ItemDrawable you must add multiple model with different name.
     */
    void addOutItemDrawableModel(CT_OutStdSingularItemModel *item);

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
    virtual void protectedCreateOutItemDrawableModelList() = 0;

    /**
     * @brief Inherit this method to read the file
     */
    virtual bool protectedReadFile() = 0;



    CT_FileHeader*                                          m_header;

private:
    QString                                                 m_filePath;
    QList<FileFormat>                                       m_formats;
    QString                                                 m_tooltip;
    QString                                                 m_errorMess;
    QList<CT_OutStdSingularItemModel*>                      m_outItemsModel;
    QList<CT_OutStdGroupModel*>                             m_outGroupsModel;
    CT_OutStdSingularItemModel*                             m_outHeaderModel;
    QMultiMap<QString, CT_AbstractSingularItemDrawable*>    m_outItems;
    QMultiMap<QString, CT_AbstractItemGroup*>               m_outGroups;
    int                                                     m_progress;
    bool                                                    m_error;
    bool                                                    m_stop;
    bool                                                    m_deleteHeader;

    void clearOutItemDrawableModel();
    void clearOutItemDrawable();

    void clearOutGroupsModel();
    void clearOutGroups();

signals:
    void started();
    void progressChanged(int progress);
    void finished();
};

#endif // CT_ABSTRACTREADER_H
