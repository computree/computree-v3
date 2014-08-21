#ifndef CT_ABSTRACTITEM_H
#define CT_ABSTRACTITEM_H

#include "pluginShared_global.h"

#include <QObject>

class CT_OutAbstractModel;
class CT_AbstractResult;

/**
 * @brief Represent an element that can be in the hierarchy of a result
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractItem : public QObject
{
    Q_OBJECT

public:
    CT_AbstractItem();

    /**
     * @brief Create a element with a model defined in your step and the result that will contains your Element
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_AbstractItem(const CT_OutAbstractModel *model, const CT_AbstractResult *result);

    /**
     * @brief Create a CT_AbstractItemDrawable with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_AbstractItem(const QString &modelUniqueName, const CT_AbstractResult *result);

    virtual ~CT_AbstractItem();

    /**
     * @brief Returns the model of the Element
     */
    CT_OutAbstractModel* abstractModel() const;

    /**
     * @brief Returns the result that contains this Element
     */
    CT_AbstractResult* result() const;

protected:

    friend class CT_AbstractResult;

    /**
     * @brief Called from Result or item to inform that this item will be removed from the result passed in parameter
     */
    virtual void internalSetWillBeRemovedFromResult(const CT_AbstractResult *res) = 0;

    /**
     * @brief Called from an item to inform another that it will be removed from a result
     * @param item : the item to inform
     * @param res : the result to pass to the item
     */
    static void staticInternalSetWillBeRemoveFromResult(CT_AbstractItem *item, const CT_AbstractResult *res);

    /**
     * @brief Change the result that contains this Element.
     * @warning Not intended for direct use by plugin developper
     */
    void internalSetResult(const CT_AbstractResult *newRes);

    /**
     * @warning Not intended for direct use by plugin developper
     */
    void internalSetModel(const CT_OutAbstractModel *model);

    /**
     * @brief Must return a string with a error message if the model is not valid. Otherwise return "".
     *        Overload it if you want to cast the model to check the validity per example.
     */
    virtual QString internalVerifyModel(const CT_OutAbstractModel *model) const;

private:
    CT_AbstractResult           *m_result;
    CT_OutAbstractModel         *m_model;
};

#endif // CT_ABSTRACTITEM_H
