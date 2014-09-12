#ifndef CT_DEFAULTITEMATTRIBUTEMANAGER_H
#define CT_DEFAULTITEMATTRIBUTEMANAGER_H

#include "ct_attributes/tools/ct_iacreator.h"
#include "ct_attributes/model/outModel/ct_outstditemattributemodel.h"
#include "ct_attributes/model/inModel/abstract/ct_inabstractitemattributemodel.h"

#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"

#include <QMultiHash>

/**
 * @brief Call CT_DEFAULT_IA_BEGIN in the private section of your singular item class BEFORE the use of method "CT_DEFAULT_IA_Vxxx"
 *        Call CT_DEFAULT_IA_END in the  private section of your singular item class AFTER the use of method "CT_DEFAULT_IA_Vxxx"
 *        Call CT_DEFAULT_IA_INIT in the cpp or hpp file
 *
 * @param ClassNameSI : your class name
 *
 * @example : private:
 *            CT_DEFAULT_IA_BEGIN(CT_Cylinder)
 *            CT_DEFAULT_IA_V2(CT_Cylinder, CT_AbstractCategory::DATA_ANGLE, &CT_Cylinder::getAngle, tr("Angle"))
 *            CT_DEFAULT_IA_V2(CT_Cylinder, CT_AbstractCategory::DATA_R2, &CT_Cylinder::getR2, tr("R²"))
 *            CT_DEFAULT_IA_END(CT_Cylinder)
 *
 *            cpp file:
 *
 *            CT_DEFAULT_IA_INIT(CT_Cylinder)
 *
 *            -----~ OR IF YOUR CLASS IS TEMPLATED ~------
 *
 *            CT_USE_DEFAULT_IA(CT_MetricT<T>)
 *            CT_DEFAULT_IA_V2(CT_MetricT<T>, CT_AbstractCategory::DATA_VALUE, &CT_MetricT<T>::getValue, tr("Value"))
 *
 *            hpp file:
 *
 *            template<typename T> CT_DEFAULT_IA_INIT(CT_MetricT<T>)
 */
#define CT_DEFAULT_IA_BEGIN(ClassNameSI) \
    friend class CT_StaticInitDefaultIAInvoker< ClassNameSI >; \
    static void staticInitDefaultItemAttributes() {

#define CT_DEFAULT_IA_END(ClassNameSI) \
    } \
    static CT_StaticInitDefaultIAInvoker< ClassNameSI >    INVOKER_DEFAULT_IA;

/**
 * @brief Call CT_DEFAULT_IA_V2 in the private section of your singular item class
 * @param UniqueNumber : must be a unique number for each new default attribute in your class
 * @param ClassNameSI : your class name
 * @param CategoryUniqueName : a category unique name (typically CT_AbstractCategory::DATA_XXXX)
 * @param GetterMethod_OR_Value : a pointer to a getter method of your class OR a value
 * @param DisplayableName : a displayable name for your default attribute
 *
 * @example : private:
 *            CT_DEFAULT_IA_V2(CT_Cylinder, CT_AbstractCategory::DATA_ANGLE, &CT_Cylinder::getCenterX, tr("Angle"))
 */
#define CT_DEFAULT_IA_V2(ClassNameSI, CategoryUniqueName, GetterMethod_OR_Value, DisplayableName) \
    PS_DIAM->addItemAttribute(#ClassNameSI, CategoryUniqueName, GetterMethod_OR_Value, DisplayableName);

/**
 * @brief Call CT_DEFAULT_IA_V3 in the private section of your singular item class
 * @param UniqueNumber : must be a unique number for each new default attribute in your class
 * @param ClassNameSI : your class name
 * @param CategoryUniqueName : a category unique name (typically CT_AbstractCategory::DATA_XXXX)
 * @param GetterMethod_OR_Value : a pointer to a getter method of your class OR a value
 * @param DisplayableName : a displayable name for your default attribute
 * @param UniqueKey : a unique key if you want to retreive your default attribute
 *
 * @example : private:
 *            CT_DEFAULT_IA_V3(CT_Cylinder, CT_AbstractCategory::DATA_ANGLE, &CT_Cylinder::getCenterX, tr("Angle"), DEF_CYL_ANGLE_KEY)
 */
#define CT_DEFAULT_IA_V3(ClassNameSI, CategoryUniqueName, GetterMethod_OR_Value, DisplayableName, UniqueKey) \
    PS_DIAM->addItemAttribute(#ClassNameSI, CategoryUniqueName, GetterMethod_OR_Value, DisplayableName, UniqueKey);

/**
 * @brief Call CT_DEFAULT_IA_INIT in the top of your source file (.cpp or .hpp) and pass the name of your singular item class
 *        between parenthesys
 *
 * @example : CT_DEFAULT_IA_INIT(CT_Cylinder)
 */
#define CT_DEFAULT_IA_INIT(ClassNameSI) \
    CT_StaticInitDefaultIAInvoker<ClassNameSI> ClassNameSI::INVOKER_DEFAULT_IA = CT_StaticInitDefaultIAInvoker<ClassNameSI>(&staticInitDefaultItemAttributes);

template<typename ClassNameT>
class PLUGINSHAREDSHARED_EXPORT CT_StaticInitDefaultIAInvoker
{
public:

    typedef void (*staticInit)();

    CT_StaticInitDefaultIAInvoker(staticInit initMethod)
    {
        (*initMethod)();
    }
};

/**
 * @brief A class tools used by the CT_Context to all default item attributes created
 *        by singular item
 */
class PLUGINSHAREDSHARED_EXPORT CT_DefaultItemAttributeManager
{
public:
    CT_DefaultItemAttributeManager();
    ~CT_DefaultItemAttributeManager();

    /**
     * @brief Add an item attribute to this manager.
     * @param className : the class name of the singular item
     * @param categoryUniqueName : the unique name to get the category in category manager
     * @param gMethod : a method to your singular item to get the data
     * @param displayableName : a displayable name for the model of the item attribute (show in GUI). If empty we will use the displayable name of the category
     * @param uniqueKey : a unique key if you want to retrieve your attribute. Can be empty but if you do that you can never
     *        retrieve your attribute by a unique key because it will be generated automatically
     * @return true if the item attribute is created and added to this manager
     */
    template <typename ItemClass, typename VType>
    bool addItemAttribute(const QString &className,
                                 const QString &categoryUniqueName,
                                 VType (ItemClass::*getter)() const,
                                 const QString &displayableName = "",
                                 const QString &uniqueKey = "")
    {
        const CT_AbstractCategory *cat = PS_CATEGORY_MANAGER->findByUniqueName(categoryUniqueName);

        Q_ASSERT_X(cat != NULL, "CT_DefaultItemAttributeManager::addItemAttribute", qPrintable(QString("You created a default item attribute but the category with name \"") + categoryUniqueName + "\" was not found"));

        // create an empty item attribute with a model and a result NULL. This attribute will only be used in model.
        CT_AbstractItemAttribute *attModel = CT_IACreator::create<ItemClass, VType>(NULL, cat, NULL, getter);

        // create the model
        CT_OutStdItemAttributeModel *model = new CT_OutStdItemAttributeModel("", attModel, displayableName.isEmpty() ? cat->displayableName() : displayableName, cat->description());

        QList<CT_DefaultItemAttributeManagerContainer*> *newL = m_collection.value(className);

        if(newL == NULL)
        {
            newL = new QList<CT_DefaultItemAttributeManagerContainer*>();
            m_collection.insert(className, newL);
        }

        CT_DefaultItemAttributeManagerContainer *c = new CT_DefaultItemAttributeManagerContainer();
        c->m_model = model;
        c->m_userKey = uniqueKey;

        newL->append(c);

        return true;
    }

    /**
     * @brief Add an item attribute to this manager.
     * @param className : the class name of the singular item
     * @param categoryUniqueName : the unique name to get the category in category manager
     * @param data : the data that will never change
     * @param displayableName : a displayable name for the model of the item attribute (show in GUI). If empty we will use the displayable name of the category
     * @param uniqueKey : a unique key if you want to retrieve your attribute. Can be empty but if you do that you can never
     *        retrieve your attribute by a unique key because it will be generated automatically
     * @return true if the item attribute is created and added to this manager
     */
    template <typename VType>
    bool addItemAttribute(const QString &className,
                          const QString &categoryUniqueName,
                          const VType &data,
                          const QString &displayableName = "",
                          const QString &uniqueKey = "")
    {
        const CT_AbstractCategory *cat = PS_CATEGORY_MANAGER->findByUniqueName(categoryUniqueName);

        Q_ASSERT_X(cat != NULL, "CT_DefaultItemAttributeManager::addItemAttribute", qPrintable(QString("You created a default item attribute but the category with name \"") + categoryUniqueName + "\" was not found"));

        // create an empty item attribute with a model and a result NULL. This attribute will only be used in model.
        CT_AbstractItemAttribute *attModel = CT_IACreator::create<VType>(NULL, cat, NULL, data);

        // create the model
        CT_OutStdItemAttributeModel *model = new CT_OutStdItemAttributeModel("", attModel, displayableName.isEmpty() ? cat->displayableName() : displayableName, cat->description());

        // add element to the collection
        QList<CT_DefaultItemAttributeManagerContainer*> *newL = m_collection.value(className);

        if(newL == NULL)
        {
            newL = new QList<CT_DefaultItemAttributeManagerContainer*>();
            m_collection.insert(className, newL);
        }

        CT_DefaultItemAttributeManagerContainer *c = new CT_DefaultItemAttributeManagerContainer();
        c->m_model = model;
        c->m_userKey = uniqueKey;

        newL->append(c);

        return true;
    }

    /**
     * @brief Returns the list of attributes for the item's type.
     * @param itemType : the item type => myItem->getType();
     */
    QList<CT_AbstractItemAttribute*> itemAttributes(const QString &itemType) const;

    /**
     * @brief Returns the item attribute corresponding to the key (uniqueKey) that you install when you add the default item attribute
     * @param uniqueKey : the unique key used in method addItemAttribute...
     * @param itemType : the item type => myItem->getType();
     * @return NULL if the element was not found.
     */
    CT_AbstractItemAttribute* itemAttributeFromUniqueKey(const QString &uniqueKey, const QString &itemType) const;

    /**
     * @brief Returns the item attribute corresponding to the OUTPUT model passed in parameter
     * @return NULL if item attribute don't exist
     */
    CT_AbstractItemAttribute* itemAttributeFromModel(const CT_OutAbstractItemAttributeModel *outModel, const QString &itemType) const;

    /**
     * @brief Returns a list of item attributes corresponding to the possibility selected of the INPUT model passed in parameter
     * @return an empty list if item attributes don't exist
     */
    QList<CT_AbstractItemAttribute*> itemAttributesFromModel(const CT_InAbstractItemAttributeModel *inModel, const QString &itemType) const;

private:

    class CT_DefaultItemAttributeManagerContainer
    {
    public:

        CT_DefaultItemAttributeManagerContainer()
        {
            m_model = NULL;
        }

        ~CT_DefaultItemAttributeManagerContainer()
        {
            delete m_model;
        }

        CT_OutAbstractItemAttributeModel    *m_model;
        QString                             m_userKey;
    };

    /**
     * @brief A QMultiHash with a QString for key (name of the class of the item) and
     *        a CT_DefaultItemAttributeManagerContainer for value.
     */
    QHash<QString, QList<CT_DefaultItemAttributeManagerContainer*>* >    m_collection;
};

#endif // CT_DEFAULTITEMATTRIBUTEMANAGER_H
