#ifndef CT_ABSTRACTCATEGORY_H
#define CT_ABSTRACTCATEGORY_H

#include "pluginShared_global.h"

#include <QObject>
#include <QString>
#include <typeinfo>

#define CT_SPECIALIZE_VALUE_TYPE_TO(SCALARTYPE, CATEGORY_TYPE, CATEGORY_TYPE_STRING) \
template<> \
inline CT_AbstractCategory::ValueType CT_AbstractCategory::staticValueTypeToCategoryType<SCALARTYPE>() { return CATEGORY_TYPE; } \
template<> \
inline QString CT_AbstractCategory::staticValueTypeToCategoryTypeString<SCALARTYPE>() { return QString(CATEGORY_TYPE_STRING); }

/**
 * @brief \defgroup CT_AbstractCategoryClass
 *        A category is used in conjonction with a CT_ItemAttribute. It represent a type of
 *        data (x, y, z coordinate or a radius or a height, etc....) and has a type of
 *        value (boolean or string or number or real number, etc....). You can test if a category
 *        is equivalent to another by use the method "isEquivalentTo(...)".
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractCategory : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Type of value (boolean or number or string, etc...)
     */
    enum ValueType {
        BOOLEAN       = 0,    // a boolean

        NUMBER        = 63,   // a number

        NUMBER_REAL   = 7,    // a real number (it's a ISDVT_NUMBER too)
        DOUBLE        = 1,    // a double (it's a ISDVT_NUMBERREAL too)
        FLOAT         = 2,    // un float (it's a ISDVT_NUMBERREAL too)
        LONG_DOUBLE   = 4,    // un long double

        NUMBER_INT    = 56,   // a integer (it's a ISDVT_NUMBER too)
        INT           = 8,    // a signed integer 32 or 64 bits (depending the compilation plateforme) (it's a integer too)
        UINT64        = 16,   // a unsigned integer 64 bits (it's a number too)
        SIZE_T        = 32,   // un entier non signés 32 bits ou 64 bits (dépend de la plateforme de compilation)

        STRING        = 256,  // a string

        UNKNOWN       = 1024  // a unknown type
    };
    Q_ENUMS(ValueType)

    /**
     * @brief Type of data (generic). Use them for uniqueName to create default categories.
     */
    static const QString DATA_VALUE;                // generic value
    static const QString DATA_ID;                   // unique id (equivalent to "DATA_VALUE")
    static const QString DATA_DISPLAYABLE_NAME;     // displayable name (equivalent to "DATA_VALUE")
    static const QString DATA_COORDINATE;           // generic coordinate (equivalent to "DATA_VALUE")
    static const QString DATA_X;                    // x coordinate (equivalent to "DATA_COORDINATE")
    static const QString DATA_Y;                    // y coordinate (equivalent to "DATA_VALUE")
    static const QString DATA_Z;                    // z coordinate (equivalent to "DATA_VALUE")
    static const QString DATA_CX;                   // x center coordinate (equivalent to "DATA_X")
    static const QString DATA_CY;                   // y center coordinate (equivalent to "DATA_Y")
    static const QString DATA_CZ;                   // z center coordinate (equivalent to "DATA_Z")
    static const QString DATA_DIRECTION;            // generic direction (equivalent to "DATA_VALUE")
    static const QString DATA_X_DIRECTION;          // x direction (equivalent to "DATA_X" and "DATA_DIRECTION")
    static const QString DATA_Y_DIRECTION;          // y direction (equivalent to "DATA_Y" and "DATA_DIRECTION")
    static const QString DATA_Z_DIRECTION;          // z direction (equivalent to "DATA_Z" and "DATA_DIRECTION")
    static const QString DATA_DIMENSION;            // generic dimension (equivalent to "DATA_VALUE")
    static const QString DATA_X_DIMENSION;          // x dimension (equivalent to "DATA_X" and "DATA_DIMENSION")
    static const QString DATA_Y_DIMENSION;          // y dimension (equivalent to "DATA_Y" and "DATA_DIMENSION")
    static const QString DATA_Z_DIMENSION;          // z dimension (equivalent to "DATA_Z" and "DATA_DIMENSION")
    static const QString DATA_RADIUS;               // radius (equivalent to "DATA_VALUE")
    static const QString DATA_R2;                   // r² for fitting (equivalent to "DATA_VALUE")
    static const QString DATA_SIZE;                 // size (equivalent to "DATA_VALUE")
    static const QString DATA_LENGTH;               // length (equivalent to "DATA_SIZE")
    static const QString DATA_WIDTH;                // width (equivalent to "DATA_LENGTH")
    static const QString DATA_HEIGHT;               // height (equivalent to "DATA_LENGTH")
    static const QString DATA_ANGLE;                // angle (equivalent to "DATA_VALUE")
    static const QString DATA_RATIO;                // ratio (equivalent to "DATA_VALUE")
    static const QString DATA_RESOLUTION;           // generic resolution (equivalent to "DATA_VALUE")
    static const QString DATA_X_RESOLUTION;         // x resolution (equivalent to "DATA_X" and "DATA_RESOLUTION")
    static const QString DATA_Y_RESOLUTION;         // y resolution (equivalent to "DATA_Y" and "DATA_RESOLUTION")
    static const QString DATA_Z_RESOLUTION;         // z resolution (equivalent to "DATA_Z" and "DATA_RESOLUTION")
    static const QString DATA_NA;                   // NA (equivalent to "DATA_VALUE")

    /**
     * @brief Construct a category
     * @param uniqueName : a category must have a unique name to find it in a list.
     * @param categoryEquivalentUniqueName : a unique name of a category that can be equivalent to this category
     * @param displayableName : a displayable name (by default the unique name if empty)
     * @param description : a description
     */
    CT_AbstractCategory(const QString &uniqueName,
                        const QString &categoryEquivalentUniqueName = "",
                        const QString &displayableName = "",
                        const QString &description = "");

    /**
     * @brief Construct a category
     * @param uniqueName : a category must have a unique name to find it in a list
     * @param categoryEquivalentUniqueNameCollection : a collection of unique name of categories that can be equivalent to this category
     * @param displayableName : a displayable name (by default the unique name if empty)
     * @param description : a description
     */
    CT_AbstractCategory(const QString &uniqueName,
                        const QList<QString> &categoryEquivalentUniqueNameCollection,
                        const QString &displayableName = "",
                        const QString &description = "");

    virtual ~CT_AbstractCategory() {}

    /**
     * @brief Add a new equivalent category
     */
    bool addEquivalentCategory(const QString &categoryEquivalentUniqueName);

    /**
     * @brief Returns the unique name of the category that can be used to find it in a list
     */
    QString uniqueName() const;

    /**
     * @brief Convert a value type to a displayable string (cannot be static because use metaObject() method)
     */
    QString valueTypeToString(ValueType v) const;

    /**
     * @brief Returns the displayable name
     */
    QString displayableName() const;

    /**
     * @brief Returns the description
     */
    QString description() const;

    /**
     * @brief Returns true if this category is equivalent to category passed in parameter
     */
    bool isEquivalentTo(const CT_AbstractCategory *c) const;

    /**
     * @brief Returns true if this category is equivalent to category passed in parameter
     */
    bool isEquivalentTo(const QString &categoryUniqueName) const;

    /**
     * @brief Convert a type of data to a type of data of a category
     */
    template <typename VType>
    static inline CT_AbstractCategory::ValueType staticValueTypeToCategoryType()
    {
        qFatal(qPrintable(QString("staticValueTypeToCategoryType unknown data type (") + typeid(VType).name() + ") ===>" + __FILE__ + __LINE__));

        return CT_AbstractCategory::UNKNOWN;
    }

    /**
     * @brief Convert a type of data to a type of data of a category (String)
     */
    template <typename VType>
    static inline QString staticValueTypeToCategoryTypeString()
    {
        qFatal(qPrintable(QString("staticValueTypeToCategoryType unknown data type (") + typeid(VType).name() + ") ===>" + __FILE__ + __LINE__));

        return QString("UNKNOWN");
    }

private:
    QString         m_uName;
    ValueType       m_vType;
    QString         m_dName;
    QString         m_description;
    QList<QString>  m_eCategories;

    /**
     * @brief check attributes and assert if it was a problem
     */
    void internalTestAttributes(const QString &where);
};

CT_SPECIALIZE_VALUE_TYPE_TO(bool, CT_AbstractCategory::BOOLEAN, "BOOL")
CT_SPECIALIZE_VALUE_TYPE_TO(double, CT_AbstractCategory::DOUBLE, "DOUBLE")
CT_SPECIALIZE_VALUE_TYPE_TO(float, CT_AbstractCategory::FLOAT, "FLOAT")
CT_SPECIALIZE_VALUE_TYPE_TO(long double, CT_AbstractCategory::LONG_DOUBLE, "LONG_DOUBLE")
CT_SPECIALIZE_VALUE_TYPE_TO(int, CT_AbstractCategory::INT, "INT")

#ifdef ENVIRONMENT32
CT_SPECIALIZE_VALUE_TYPE_TO(quint64, CT_AbstractCategory::UINT64, "UINT64")
#endif

CT_SPECIALIZE_VALUE_TYPE_TO(size_t, CT_AbstractCategory::SIZE_T, "SIZE_T")
CT_SPECIALIZE_VALUE_TYPE_TO(QString, CT_AbstractCategory::STRING, "STRING")

#endif // CT_ABSTRACTCATEGORY_H
