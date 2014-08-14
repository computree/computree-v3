#ifndef CT_DEFAULTITEMATTRIBUTEMANAGER_H
#define CT_DEFAULTITEMATTRIBUTEMANAGER_H

#include "ct_tools/model/ct_autorenamemodels.h"
#include "ct_attributes/tools/ct_iacreator.h"
#include "ct_attributes/model/outModel/ct_outstditemattributemodel.h"

#include "ct_global/ct_context.h"
#include "ct_categories/tools/ct_categorymanager.h"

#include <QMultiHash>

/**
 * @brief Call CT_USE_DEFAULT_IA in the private section of your singular item class before the use of method
 *        "CT_DEFAULT_IA_Vxxx"
 * @param ClassNameSI : your class name
 *
 * @example : private:
 *            CT_USE_DEFAULT_IA(CT_Cylinder)
 *            CT_DEFAULT_IA_V2(0, CT_Cylinder, CT_AbstractCategory::DATA_ANGLE, &CT_Cylinder::getCenterX, tr("Angle"))
 *
 *            cpp file:
 *
 *            CT_INIT_DEFAULT_IA(0, CT_Cylinder)
 *
 *            -----~ OR IF YOUR CLASS IS TEMPLATED ~------
 *
 *            CT_USE_DEFAULT_IA(CT_MetricT<T>)
 *            CT_DEFAULT_IA_V2(0, CT_MetricT<T>, CT_AbstractCategory::DATA_VALUE, &CT_MetricT<T>::getValue, tr("Value"))
 *
 *            cpp file:
 *
 *            template<typename T> CT_INIT_DEFAULT_IA(0, CT_MetricT<T>)
 */
#define CT_USE_DEFAULT_IA(ClassNameSI) \
    friend class CT_StaticInitDefaultIAInvoker<ClassNameSI>;

/**
 * @brief Call CT_DEFAULT_IA_V2 in the private section of your singular item class
 * @param UniqueNumber : must be a unique number for each new default attribute in your class
 * @param ClassNameSI : your class name
 * @param CategoryUniqueName : a category unique name (typically CT_AbstractCategory::DATA_XXXX)
 * @param GetterMethod_OR_Value : a pointer to a getter method of your class OR a value
 * @param DisplayableName : a displayable name for your default attribute
 *
 * @example : private:
 *            CT_DEFAULT_IA_V2(0, CT_Cylinder, CT_AbstractCategory::DATA_ANGLE, &CT_Cylinder::getCenterX, tr("Angle"))
 */
#define CT_DEFAULT_IA_V2(UniqueNumber, ClassNameSI, CategoryUniqueName, GetterMethod_OR_Value, DisplayableName) \
    static void staticInitDefaultItemAttributes##UniqueNumber(const QString &className) { \
        PS_DIAM->addItemAttribute(#ClassNameSI, CategoryUniqueName, GetterMethod_OR_Value, DisplayableName); \
    } \
    static CT_StaticInitDefaultIAInvoker<ClassNameSI>    INVOKER_##UniqueNumber;

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
 *            CT_DEFAULT_IA_V3(0, CT_Cylinder, CT_AbstractCategory::DATA_ANGLE, &CT_Cylinder::getCenterX, tr("Angle"), DEF_CYL_ANGLE_KEY)
 */
#define CT_DEFAULT_IA_V3(UniqueNumber, ClassNameSI, CategoryUniqueName, GetterMethod_OR_Value, DisplayableName, UniqueKey) \
    static void staticInitDefaultItemAttributes##UniqueNumber(const QString &className) { \
        PS_DIAM->addItemAttribute(#ClassNameSI, CategoryUniqueName, GetterMethod_OR_Value, DisplayableName, UniqueKey); \
    } \
    static CT_StaticInitDefaultIAInvoker<ClassNameSI>    INVOKER_##UniqueNumber;

/**
 * @brief Call CT_INIT_DEFAULT_IA in the bottom of your header file (.h) and pass the name of your singular item class
 *        between parenthesys
 *
 * @example : CT_INIT_DEFAULT_IA(0, CT_Cylinder)
 *            CT_INIT_DEFAULT_IA(1, CT_Cylinder)
 *            etc...
 */
#define CT_INIT_DEFAULT_IA(UniqueNumber, ClassNameSI) \
    CT_StaticInitDefaultIAInvoker<ClassNameSI> ClassNameSI::INVOKER_##UniqueNumber = CT_StaticInitDefaultIAInvoker<ClassNameSI>(#ClassNameSI, &staticInitDefaultItemAttributes##UniqueNumber);

template<typename ClassNameT>
class PLUGINSHAREDSHARED_EXPORT CT_StaticInitDefaultIAInvoker
{
public:

    typedef void (*staticInit)(const QString &);

    CT_StaticInitDefaultIAInvoker(const QString &classNameString, staticInit initMethod)
    {
        (*initMethod)(classNameString);
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

        // create a CT_AutoRenameModels object to rename the model automatically
        CT_AutoRenameModels *autoName = new CT_AutoRenameModels();

        // add element to the collection
        CT_DefaultItemAttributeManagerContainer *c = new CT_DefaultItemAttributeManagerContainer();
        c->m_model = model;
        c->m_autoName = autoName;
        c->m_userKey = uniqueKey;

        // m_collection is a multi map so for all attributes created by a class we will insert a new container
        m_collection.insert(className, c);

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

        // create a CT_AutoRenameModels object to rename the model automatically
        CT_AutoRenameModels *autoName = new CT_AutoRenameModels();

        // add element to the collection
        CT_DefaultItemAttributeManagerContainer *c = new CT_DefaultItemAttributeManagerContainer();
        c->m_model = model;
        c->m_autoName = autoName;
        c->m_userKey = uniqueKey;

        // m_collection is a multi map so for all attributes created by a class we will insert a new container
        m_collection.insert(className, c);

        return true;
    }

    /**
     * @brief Returns the list of attributes for the item's type.
     * @param itemType : the item type => myItem->getType();
     */
    QList<CT_AbstractItemAttribute*> itemAttributes(const QString &itemType) const;

private:

    class CT_DefaultItemAttributeManagerContainer
    {
    public:

        CT_DefaultItemAttributeManagerContainer()
        {
            m_autoName = NULL;
            m_model = NULL;
        }

        ~CT_DefaultItemAttributeManagerContainer()
        {
            delete m_model;
            delete m_autoName;
        }

        CT_AutoRenameModels                 *m_autoName;
        CT_OutAbstractItemAttributeModel    *m_model;
        QString                             m_userKey;
    };

    /**
     * @brief A QMultiHash with a QString for key (name of the class of the item) and
     *        a CT_DefaultItemAttributeManagerContainer for value.
     */
    QMultiHash<QString, CT_DefaultItemAttributeManagerContainer*>    m_collection;
};

#endif // CT_DEFAULTITEMATTRIBUTEMANAGER_H
