#ifndef CT_ABSTRACTMODEL_H
#define CT_ABSTRACTMODEL_H

#include "pluginShared_global.h"
#include "interfaces.h"

#include <QString>
#include <QList>

class CT_AutoRenameModels;
class CT_VirtualAbstractStep;
class CT_ResultGroup;

/**
 * @brief Represent a model for all elements that must have a model.
 *
 * A model can have zero or multiple childrens (other CT_AbstractModel). So the structure is a tree. All models in the tree
 * must have a unique name. This can be made by two ways :
 *
 * - You create all the tree and you define a unique name for all elements manually. You must be sure to have a unique name for
 *   all elements.
 *
 * - You get a unknown tree and you want to add other elements but you don't know the name of existing elements in the tree. So you must use
 *   a CT_AutoRenameModels that can rename automatically a model to get a unique name not used in the tree by other elements. Create a model with
 *   an empty name and pass a CT_AutoRenameModels object when you add the model to another. Keep the CT_AutoRenameModels to get the unique name of
 *   the model if you want to find it later in the tree.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractModel : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Construct a model
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_AbstractModel(const QString &uniqueName,
                     const QString &description,
                     const QString &displayableName);

    virtual ~CT_AbstractModel();

    /**
     * @brief Returns the model type in a displayable QString (for gui)
     */
    virtual QString modelTypeDisplayable() const = 0;

    /**
     * @brief Returns the unique name of the model
     */
    QString uniqueName() const;

    /**
     * @brief Returns the unique name of the model + the turn. If it was only one turn the uniqueNamePlusTurn() == uniqueName().
     *        If you want to compare a model in a turn use the method uniqueName().
     *        If you want to save and restore a model under a turn use this method. (Used by CT_InResultModelGroup)
     * @warning Only model that represent a "Result" use the turn information. Item, Attributes and other type of model don't use the turn information.
     */
    QString uniqueNamePlusTurn() const;

    /**
     * @brief Set a turn index for the uniqueNamePlusTurn()
     */
    void setTurnIndex(int turnIndex);

    /**
     * @brief Returns the turn index
     */
    int turnIndex() const;

    /**
     * @brief Returns the description of the model
     */
    QString description() const;

    /**
     * @brief Returns the displayable name of the model
     */
    QString displayableName() const;

    /**
     * @brief Set a new displayable name
     */
    void setDisplayableName(const QString &displayableName);

    /**
     * @brief Change the unique name of this model
     * @return false if the unique name is already used in the hierarchy
     */
    bool setUniqueName(const QString &uniqueName);

    /**
     * @brief Check if you can change the uniqueName() to the value passed in parameter.
     * @return true if ok
     */
    virtual bool canSetUniqueName(const QString &uniqueName) const;

    /**
     * @brief Returns all children of this model
     */
    virtual QList<CT_AbstractModel*> childrens() const = 0;

    /**
     * @brief Returns all children of this model casted to the template parameter
     */
    template<typename T> QList<T*> childrensStaticCast() const
    {
        QList<T*> childrens;
        QList<CT_AbstractModel*> child = this->childrens();
        QListIterator<CT_AbstractModel*> it(child);

        while(it.hasNext())
            childrens.append((T*)it.next());

        return childrens;
    }

    /**
     * @brief Returns all models of this model and its childrens
     */
    QList<CT_AbstractModel*> recursiveGetAllModels() const;

    /**
     * @brief Returns true if the model don't have childrens
     */
    virtual bool isEmpty() const = 0;

    /**
     * @brief Returns the parent model of this model
     */
    CT_AbstractModel* parentModel() const;

    /**
     * @brief Returns the root model of the hierarchy
     */
    CT_AbstractModel* rootModel() const;

    /**
     * @brief Returns true if the unique name passed in parameter exist in hierarchy (begin from the root model and go down recursively)
     */
    bool existInTree(const QString &uniqueName) const;

    /**
     * @brief Returns true if the unique name of the model passed in parameter or one of its children (recursively) exist
     *        in hierarchy (begin from the root model and go down recursively)
     */
    bool existModelOrChildrensInTree(CT_AbstractModel *modelToSearch) const;

    /**
     * @brief Search the model in the entire hierarchy that unique name equals the unique name passed
     *        in parameter (begin from the root model and go down recursively)
     * @return NULL if no model was found
     */
    CT_AbstractModel* findModelInTree(const QString &uniqueName) const;

    /**
     * @brief Returns the step who created this model
     */
    CT_VirtualAbstractStep* step() const;

    /**
     * @brief Returns the model of the result were we can find this model
     */
    //virtual CT_AbstractModel* resultModel() const = 0;

protected:

    /**
     * @brief Set the parent model of this model
     */
    void setParentModel(const CT_AbstractModel *parent);

    /**
     * @brief Set a parent model to another model
     */
    static void staticSetParentModelToModel(CT_AbstractModel *model, const CT_AbstractModel *parent);

    /**
     * @brief Search the model (search) and its children in hierarchy (go down in the structure)
     * @param rootGroup : for the first call you must pass the root model if you want to search in all hierarchy, use the rootModel() method
     * @param search : the model and its children to search
     * @return true if the model or one of its children were found in hierarchy
     */
    bool internalExistModelOrChildrensInTree(CT_AbstractModel *rootModel,
                                             CT_AbstractModel *search) const;


    /**
     * @brief Search the unique name in hierarchy (go down in the structure)
     * @param parent : for the first call you must pass the root model if you want to search in all hierarchy, use the rootModel() method
     * @param uniqueName : the unique name to search
     * @return true if unique name exist in hierarchy
     */
    bool internalExistInTree(CT_AbstractModel *parent,
                             const QString &uniqueName) const;

    /**
     * @brief Search the model that have the unique name in hierarchy (go down in the structure)
     * @param parent : for the first call you must pass the root model if you want to search in all hierarchy, use the rootModel() method
     * @param uniqueName : the unique name to search
     * @return NULL if model was not found
     */
    CT_AbstractModel* internalFindModelInTree(CT_AbstractModel *parent,
                                              const QString &uniqueName) const;

    friend class CT_AutoRenameModels;
    /**
     * @brief Used by class CT_AutoRenameModels to set it when the model was added in a auto rename object.
     *        If a previous auto rename object was set, the method call the removeModel(this) method to remove
     *        this model from this object.
     */
    void setAutoRenameObject(const CT_AutoRenameModels *autoRename);

    /**
     * @brief Returns the auto rename object of this model
     */
    CT_AutoRenameModels* autoRenameObject() const;

    friend class CT_VirtualAbstractStep;
    /**
     * @brief Used by class CT_VirtualAbstractStep to set it when the model was created or by childrens
     *        when the model was added to it.
     * @warning Set step to this model and to childrens (recursively)
     */
    void setStep(const CT_VirtualAbstractStep *step);

private:
    QString                 m_uName;
    QString                 m_uNamePlusTurn;
    int                     m_turnIndex;
    QString                 m_description;
    QString                 m_dName;
    CT_AutoRenameModels     *m_autoRename;
    CT_VirtualAbstractStep  *m_step;
    CT_AbstractModel        *m_parent;
};

#endif // CT_ABSTRACTMODEL_H
