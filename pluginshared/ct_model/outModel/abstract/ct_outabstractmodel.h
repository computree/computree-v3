#ifndef CT_OUTABSTRACTMODEL_H
#define CT_OUTABSTRACTMODEL_H

#include "ct_model/abstract/ct_abstractmodel.h"
#include "ct_item/abstract/ct_abstractitem.h"

class QMutex;

/**
 * @brief Represent a OUPUT model. An CT_OutAbstractModel is just a class to know that is a OUPUT model.
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutAbstractModel : public CT_AbstractModel
{
    Q_OBJECT

public:
    /**
     * @brief Construct a OUTPUT model
     * @param uniqueName : a unique name in all the hierarchy
     * @param description : a description
     * @param displayableName : a displayable name
     */
    CT_OutAbstractModel(const QString &uniqueName,
                        const QString &description,
                        const QString &displayableName);
    ~CT_OutAbstractModel();

    /**
     * @brief Returns the original model (recursively) if this model is a copy otherwise return 'this'
     */
    CT_OutAbstractModel* originalModel() const;

    /**
     * @brief Returns the original model (not recursively) if this model is a copy otherwise return 'this'
     */
    CT_OutAbstractModel* currentOriginalModel() const;

    /**
     * @brief Returns the original model (recursively) with a result not null. If this model
     *        is not a copy and had a result the method return 'this'. If this model
     *        is not a copy and don't had a result the method return NULL.
     */
    CT_OutAbstractModel* lastOriginalModelWithAResult() const;

    /**
     * @brief Returns true if at least one element that use this model is visible in a document
     */
    bool isVisible() const;

    /**
     * @brief Returns true if at least one element that use this model is visible in the document passed in parameter
     */
    bool isVisibleInDocument(const DocumentInterface *doc) const;

    /**
     * @brief Copy this model
     */
    virtual CT_OutAbstractModel* copy() const = 0;

    /**
     * @brief Returns the item contained in this model
     */
    CT_AbstractItem* item() const;

    /**
     * @brief Returns the result where item is stocked. If it was a copy of a model the result will be found to the originalModel
     * @warning This will returns a no NULL element only if this model was set to an Item and the Item is added to a result
     *          or something in the tree structure of the result
     */
    CT_AbstractResult* result() const;

protected:

    /**
     * @brief Used by model to set the original model when a copy is made
     */
    void setOriginalModel(const CT_OutAbstractModel *model);

    /**
     * @brief Used by other model to set the original model on childrens per example
     */
    static void staticSetOriginalModel(CT_OutAbstractModel *model, const CT_OutAbstractModel *originalModel)
    {
        model->setOriginalModel(originalModel);
    }

    /**
     * @brief Used by model to set the item
     */
    void setItem(CT_AbstractItem *item);

    /**
     * @brief Delete the item from this model
     */
    void clearItem();

    friend class CT_AbstractItem;
    friend class CT_AbstractItemDrawable;
    friend class CT_AbstractResult;

    /**
     * @brief Used by CT_AbstractItemDrawable or CT_AbstractItemAttribute to set the result of this model
     */
    void setResult(const CT_AbstractResult *res);

    /**
     * @brief Increment the counter that count the number of element (that use this model ) visible in the document passed in parameter
     *        Used by CT_AbstractItemDrawable
     */
    void incrementVisibleInDocument(const DocumentInterface *doc);

    /**
     * @brief Decrement the counter that count the number of element (that use this model ) visible in the document passed in parameter
     *        Used by CT_AbstractItemDrawable
     */
    void decrementVisibleInDocument(const DocumentInterface *doc);

    friend class CT_OutTurnManager;
    friend class ModelTest;

    /**
     * @brief Called from CT_OutTurnManager or parent model to make some
     *        change in models to complete it.
     */
    virtual bool recursiveSetComplete();

private:
    CT_OutAbstractModel             *m_originalModel;
    QHash<DocumentInterface*, int>   m_visibleInDocuments;
    QMutex                          *m_originalModelMutex;
    CT_AbstractResult               *m_realResult;
    CT_AbstractItem                 *m_item;

private slots:
    void originalModelDestroyed();

signals:
    void visibilityChanged(bool v);
    void visibilityInDocumentChanged(const DocumentInterface *doc, bool v);
};

#endif // CT_OUTABSTRACTMODEL_H
