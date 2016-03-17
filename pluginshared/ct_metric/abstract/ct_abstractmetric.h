#ifndef CT_ABSTRACTMETRIC_H
#define CT_ABSTRACTMETRIC_H

#include "ct_element/abstract/ct_abstractconfigurableelement.h"
#include "ct_attributes/abstract/ct_abstractitemattribute.h"
#include "ct_tools/model/ct_autorenamemodels.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"

/**
 * @brief A metric can export only attributes (but multiple attributes of different type)
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractMetric : public CT_AbstractConfigurableElement
{
    Q_OBJECT

public:
    CT_AbstractMetric();
    CT_AbstractMetric(const CT_AbstractMetric &other);
    ~CT_AbstractMetric();

    /**
     * @brief Returns the name of this metric + name of all attributes declared
     */
    QString getDetailledDisplayableName() const;

    /**
     * @brief Call this method to clear and create attributes (after configuration)
     */
    void postConfigure();

    /**
     * @brief Call this method to init attributes models
     * @param resPoss : the result model
     * @param parentModel : the autorename object of the singular item that will contains attributes
     */
    void initAttributesModels(CT_OutResultModelGroupToCopyPossibilities* resPoss, const CT_AutoRenameModels &parentModel);

    /**
     * @brief Call this method to compute metrics for the item passed in parameter
     */
    void computeMetric(CT_AbstractSingularItemDrawable *item);

protected:
    /**
     * @brief Abstract class that declare a method to create a attribute and
     *        save all necessary element to create the attribute later
     */
    class PLUGINSHAREDSHARED_EXPORT AbstractAttributeObject
    {
    public:
        AbstractAttributeObject()
        {
            _autoRenameModel = NULL;
            _attributeInstance = NULL;
        }

        AbstractAttributeObject(const AbstractAttributeObject &other)
        {
            _displayableName = other._displayableName;
            _categoryName = other._categoryName;
            _autoRenameModel = new CT_AutoRenameModels();
            _attributeInstance = NULL;
        }

        AbstractAttributeObject(QString displayableName,
                                QString category,
                                CT_AutoRenameModels* autoRenameModel)
        {
            _displayableName = displayableName;
            _categoryName = category;
            _autoRenameModel = autoRenameModel;
            _attributeInstance = NULL;
        }

        virtual ~AbstractAttributeObject() {
            delete _autoRenameModel;
            delete _attributeInstance;
        }

        virtual CT_AbstractCategory::ValueType valueType() const = 0;

        /**
         * @brief Create a new attribute and return it
         */
        virtual CT_AbstractItemAttribute* createPrototypeAttribute() const = 0;

        /**
         * @brief Returns a copy of this object
         */
        virtual AbstractAttributeObject* copy() const = 0;

        QString                                 _displayableName;
        QString                                 _categoryName;
        CT_AutoRenameModels*                    _autoRenameModel;
        CT_AbstractItemAttribute*               _attributeInstance;
    };

    /**
     * @brief Class that inherit from AbstractAttributeObject but know the type of the attribute to create
     */
    template<typename VType>
    class AttributeObject : public AbstractAttributeObject
    {
    public:
        AttributeObject() : AbstractAttributeObject() {}
        AttributeObject(const AttributeObject &other) : AbstractAttributeObject(other) {}
        AttributeObject(QString displayableName,
                        QString category,
                        CT_AutoRenameModels* autoRenameModel)  : AbstractAttributeObject(displayableName, category, autoRenameModel) {}

        CT_AbstractCategory::ValueType valueType() const {
            return CT_AbstractCategory::staticValueTypeToCategoryType<VType>();
        }

        CT_AbstractItemAttribute* createPrototypeAttribute() const {
            return new CT_StdItemAttributeT<VType>(NULL, PS_CATEGORY_MANAGER->findByUniqueName(_categoryName), NULL, VType());
        }

        AbstractAttributeObject* copy() const {
            return new AttributeObject<VType>(*this);
        }
    };

    typedef QMap<void*, CT_AbstractMetric::AbstractAttributeObject*>                    AttributesContainer;
    typedef QMapIterator<void*, CT_AbstractMetric::AbstractAttributeObject*>            AttributesContainerIterator;
    typedef QMutableMapIterator<void*, CT_AbstractMetric::AbstractAttributeObject*>     AttributesContainerMutableIterator;

    /**
     * @brief Redeclare this method in your class and call "addAttribute" method for each attribute you want to declare
     */
    virtual void createAttributes() = 0;

    /**
     * @brief Redeclare this method in your class to do computing of metrics and call method "setAttribute(...)" to set results
     */
    virtual void computeMetric() = 0;

    /**
     * @brief Declare a attribute that you will set in your compute method
     * @example : addAttribute(m_classMemberThatWillBeComputedInComputeMetrics, CT_AbstractCategory::DATA_DIMENSION, tr("my attribute that was nice"));
     * @param classMember : a variable member of your class that will be used in "computeMetrics" method.
     * @param categoryName : the name of the category of the attribute (example : CT_AbstractCategory::DATA_AREA)
     * @param displayableName : a displayable name for this attribute (displayed in gui), must not be empty !
     */
    template<typename VType>
    void addAttribute(VType &classMember, const QString &categoryName, const QString &displayableName) {
        if(!m_attributes.contains((void*)&classMember))
            m_attributes.insert((void*)&classMember, new CT_AbstractMetric::AttributeObject<VType>(displayableName, categoryName, new CT_AutoRenameModels()));
    }

    /**
     * @brief Call setAttribute(....) in your "compute" method to set results of attributes declared with "addAttribute" method
     * @param classMemberAndValue : the variable member of your class that was filled with the good value and that you used in method "addAttribute"
     */
    template<typename VType>
    void setAttributeValue(VType &classMemberAndValue) {
        AbstractAttributeObject *attributeObject = getAttributeObject((void*)&classMemberAndValue, CT_AbstractCategory::staticValueTypeToCategoryType<VType>());

        if (attributeObject != NULL)
        {
            delete attributeObject->_attributeInstance;
            attributeObject->_attributeInstance = new CT_StdItemAttributeT<VType>(attributeObject->_autoRenameModel->completeName(),
                                                                         attributeObject->_categoryName,
                                                                         m_result,
                                                                         classMemberAndValue);
        }
    }

    /**
     * @brief Clear all attributes declared
     */
    virtual void clearAttributes();

    /**
     * @brief Find and returns the attribute object  with a unique name and a type
     * @param classMember : the variable member of your class declared with method "addAttribute"
     * @param type : the type of the attribute (must be the same that when it was declared with method "addAttribute"
     * @return NULL if attribute was not found or type is different
     */
    AbstractAttributeObject* getAttributeObject(void* classMember, CT_AbstractCategory::ValueType type);

    /**
     * @brief Returns the result used to add attributes (only available in "computeMetrics" method)
     */
    CT_ResultGroup* result() const;

    /**
     * @brief Returns attributes added in "createAttributes" method
     */
    const AttributesContainer& attributes() const;

private:
    CT_ResultGroup*                     m_result;
    AttributesContainer                 m_attributes;

protected:

    /**
     * @brief Insert a attribute object
     * @return false if key already exist
     */
    bool internalInsertAttributeObject(void *key, CT_AbstractMetric::AbstractAttributeObject *obj);

    /**
     * @brief Remove an attribute declared with method "addAttribute"
     */
    virtual bool removeAttribute(void* key);
};

#endif // CT_AbstractMetric_H
