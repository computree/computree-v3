#ifndef CT_ABSTRACTSINGULARITEMDRAWABLE_H
#define CT_ABSTRACTSINGULARITEMDRAWABLE_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_attributes/tools/ct_defaultitemattributemanager.h"
#include "ct_attributes/tools/ct_itemattributecontainer.h"

/**
 * @brief Represent an item that must not contains childrens (other items). Its a leaf in a tree structure of items.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractSingularItemDrawable : public CT_AbstractItemDrawable
{
    Q_OBJECT

public:
    CT_AbstractSingularItemDrawable();

    /**
     * @brief Create a ItemDrawable with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_AbstractSingularItemDrawable(const CT_OutAbstractSingularItemModel *model,
                                    const CT_AbstractResult *result);

    /**
     * @brief Create a ItemDrawable with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_AbstractSingularItemDrawable(const QString &modelName,
                                    const CT_AbstractResult *result);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    virtual QString getType() const = 0;
    static QString staticGetType();

    /**
     * @brief Change the result that contains this item.
     * @warning Not intended for direct use by plugin developper
     */
    virtual void changeResult(const CT_AbstractResult *newRes);

    /**
     * @brief Add an attribute to this itemdrawable
     * @warning the attribute must have a result to be added to this item
     * @return false if the attribute already exist (model uniqueName already exist) in this singular item
     */
    bool addItemAttribute(CT_AbstractItemAttribute *att);

    /**
     * @brief Returns the item attribute corresponding to the OUTPUT model passed in parameter
     * @return NULL if item attribute don't exist in this singular item
     */
    CT_AbstractItemAttribute* itemAttribute(const CT_OutAbstractItemAttributeModel *outModel) const;

    /**
     * @brief Returns the item attribute corresponding to the OUTPUT model searched by modelName.
     *
     * @param modelName : typically a DEF_... in your step
     * @param step : the step that contains the model to search (if your are in the "compute" method pass "this" to this parameter)
     *
     * @warning if you pass a modelName that correspond to a IN model the method returns the value of the method "firstItemAttribute(IN model)"
     *
     * @return NULL if no item will be found
     */
    CT_AbstractItemAttribute* itemAttributeByOUTModelName(const CT_VirtualAbstractStep *step, const QString &modelName) const;

    /**
     * @brief Returns a list of item attributes corresponding to the possibility selected of the INPUT model passed in parameter
     * @return NULL if item attribute don't exist in this singular item
     */
    QList<CT_AbstractItemAttribute*> itemAttributes(const CT_InAbstractItemAttributeModel *inModel) const;

    /**
     * @brief Return all item attributes of this item drawable
     */
    QList<CT_AbstractItemAttribute*> itemAttributes() const;

    /**
     * @brief Returns the first item attribute that match with a possibility of the IN model.
     *
     *        An IN model can have a number of possibilities > 1 if you set Choose_MultipleIfMultiple. A
     *        group can contains items that match with one or two or etc... possibility of the model so he must
     *        return a list of ItemDrawable. This method test the first possibility, if a ItemDrawable is found,
     *        it will be returned otherwise the method continue to search with the next possibility, etc...
     *
     * @return NULL if no item will be found
     */
    CT_AbstractItemAttribute* firstItemAttribute(const CT_InAbstractItemAttributeModel *inModel) const;

    /**
     * @brief Returns the item attribute corresponding to the IN model searched by modelName
     * @param modelName : typically a DEF_... in your step
     * @param step : the step that contains the model to search (if your are in the "compute" method pass "this" to this parameter)
     *
     * @warning if you pass a modelName that correspond to a OUT model the method returns the value of the method "item(OUT model)"
     *
     * @return NULL if no item attribute will be found
     */
    CT_AbstractItemAttribute* firstItemAttributeByINModelName(const CT_VirtualAbstractStep *step, const QString &modelName) const;

    /**
     * @brief Return all default item attributes of this item drawable
     */
    QList<CT_AbstractItemAttribute*> defaultItemAttributes() const;

    /**
     * @brief Return all item attributes (added by user) of this item drawable
     */
    QList<CT_AbstractItemAttribute*> notDefaultItemAttributes() const;

    /**
     * @brief Returns true if this item has a bounding box. All singular item has a bounding box so returns true by default.
     */
    virtual bool hasBoundingBox() const {return true;}

    /**
     * @brief Get the bounding box of the item
     */
    virtual void getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const;

    /**
     * @brief Get the bounding box of the item but scaled
     *
     *        operation example ===> maxX = cx + (abs(maxX - cx) * xCoeff)
     *        operation example ===> minX = cx - (abs(minX - cx) * xCoeff)
     */
    void getScaledBoundingBox(double xCoeff, double yCoeff, double zCoeff, Eigen::Vector3d &min, Eigen::Vector3d &max);

    /**
     * @brief Get the bounding box of the item but scaled
     *
     *        operation example ===> maxX = maxX + xMeters
     *        operation example ===> minX = minX - xMeters
     */
    void getBufferedBoundingBox(double xMeters, double yMeters, double zMeters, Eigen::Vector3d &min, Eigen::Vector3d &max);

    /**
     * @brief Change the bounding box
     */
    void setBoundingBox(double minx, double miny, double minz, double maxx, double maxy, double maxz);

    /**
     * @brief Change the center of the item from coordinates of the bounding box
     */
    void updateCenterFromBoundingBox();

    /**
     * @brief Get min X coordinate of the bounding box
     */
    inline double minX() const {return _minCoordinates(0);}

    /**
     * @brief Get min Y coordinate of the bounding box
     */
    inline double minY() const {return _minCoordinates(1);}

    /**
     * @brief Get min Z coordinate of the bounding box
     */
    inline double minZ() const {return _minCoordinates(2);}

    /**
     * @brief Get max X coordinate of the bounding box
     */
    inline double maxX() const {return _maxCoordinates(0);}

    /**
     * @brief Get max Y coordinate of the bounding box
     */
    inline double maxY() const {return _maxCoordinates(1);}

    /**
     * @brief Get max Z coordinate of the bounding box
     */
    inline double maxZ() const {return _maxCoordinates(2);}

protected:
    Eigen::Vector3d   _minCoordinates;
    Eigen::Vector3d   _maxCoordinates;

    /**
     * @brief Overloaded to cast the model to check the validity.
     */
    virtual QString internalVerifyModel(const CT_OutAbstractModel *model) const;

    /**
     * @brief Called from result ot parent item to inform that this item will be deleted from the result passed in parameter
     */
    virtual void internalSetWillBeRemovedFromResult(const CT_AbstractResult *res);

private:

    CT_ItemAttributeContainer   m_itemAttributes;

    // declare that we will add default item attributes in this class
    //  => We must add CT_DEFAULT_IA_INIT(CT_AbstractSingularItemDrawable) in top of cpp file
    CT_DEFAULT_IA_BEGIN(CT_AbstractSingularItemDrawable)
    CT_DEFAULT_IA_V2(CT_AbstractSingularItemDrawable, CT_AbstractCategory::staticInitDataId(), &CT_AbstractSingularItemDrawable::id, QObject::tr("ID"))
    CT_DEFAULT_IA_V2(CT_AbstractSingularItemDrawable, CT_AbstractCategory::staticInitDataDisplayableName(), &CT_AbstractSingularItemDrawable::displayableName, QObject::tr("Nom"))
    CT_DEFAULT_IA_V2(CT_AbstractSingularItemDrawable, CT_AbstractCategory::staticInitDataCx(), &CT_AbstractSingularItemDrawable::getCenterX, QObject::tr("Centre X"))
    CT_DEFAULT_IA_V2(CT_AbstractSingularItemDrawable, CT_AbstractCategory::staticInitDataCy(), &CT_AbstractSingularItemDrawable::getCenterY, QObject::tr("Centre Y"))
    CT_DEFAULT_IA_V2(CT_AbstractSingularItemDrawable, CT_AbstractCategory::staticInitDataCz(), &CT_AbstractSingularItemDrawable::getCenterZ, QObject::tr("Centre Z"))
    CT_DEFAULT_IA_END(CT_AbstractSingularItemDrawable)

    //CT_DECLARE_ADD_DEFAULT_IA(CT_AbstractSingularItemDrawable)
};

#endif // CT_ABSTRACTSINGULARITEMDRAWABLE_H
