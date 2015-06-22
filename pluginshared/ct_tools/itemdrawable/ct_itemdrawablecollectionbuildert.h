#ifndef CT_ITEMDRAWABLECOLLECTIONBUILDERT_H
#define CT_ITEMDRAWABLECOLLECTIONBUILDERT_H

#include "ct_tools/itemdrawable/abstract/ct_abstractitemdrawablecollectionbuilder.h"

#include <QList>

class CT_Nothing1 { public: static QString staticGetType(){ return "."; } };
class CT_Nothing2 { public: static QString staticGetType(){ return "."; } };
class CT_Nothing3 { public: static QString staticGetType(){ return "."; } };

/**
 * @brief Recursively build a collection of ItemDrawable of <Type> available from a step and recursively in childs (steps). Step must
 *        have produced results and real ItemDrawable, not just models !
 *
 *        example : I wan't all CT_AbstractPointsAttributes of type Color and Scalar
 *
 *        CT_ItemDrawableCollectionBuilderT<CT_AbstractPointsAttributes> builder;
 *
 *        // create the collection of CT_AbstractPointsAttributes
 *        builder.buildFrom(rootStep);
 *
 *        // remove CT_AbstractPointsAttributes that can not be casted in type CT_AttributesColor or CT_AbstractAttributesScalar
 *        builder.removeTypeInCollection<CT_AttributesColor, CT_AbstractAttributesScalar>();
 *
 *        // get the collection
 *        const QList<CT_ItemDrawableCollectionHierarchyStep> collection = builder.collection();
 *
 */
template<typename ItemDrawable1, typename ItemDrawable2 = CT_Nothing1, typename ItemDrawable3 = CT_Nothing2, typename ItemDrawable4 = CT_Nothing3>
class CT_ItemDrawableCollectionBuilderT : public CT_AbstractItemDrawableCollectionBuilder
{
public:
    CT_ItemDrawableCollectionBuilderT();

    /**
     * @brief Build the collection of ItemDrawable that type (staticGetType) match with template (ItemDrawable1, ItemDrawable2, ...).
     * @param step : the step where begin the search (algorithme go up in the hierarchy of step to find in results items searched)
     * @param forceSearchModels : True to search only models if step was not already launched. In this case the attribute
     *                         "modelsCollection" and "modelResult" of CT_ItemDrawableCollectionHierarchyResult will be filled.
     *
     *                         False to search only real ItemDrawable. In this case the attribute "collection" and "result" of CT_ItemDrawableCollectionHierarchyResult
     *                         will be filled.
     */
    bool buildFrom(const CT_VirtualAbstractStep *step, bool forceSearchModels);

    /**
     * @brief Only keeps in the collection ItemDrawable that cannot be casted in types of template (Cast1, Cast2, etc...)
     */
    template<typename Cast1>
    inline void onlyKeepsTheseTypesInCollection1() { onlyKeepsTheseTypesInCollection2<Cast1, CT_Nothing1>(); }

    template<typename Cast1, typename Cast2>
    inline void onlyKeepsTheseTypesInCollection2() { onlyKeepsTheseTypesInCollection3<Cast1, Cast2, CT_Nothing1>(); }

    template<typename Cast1, typename Cast2, typename Cast3>
    inline void onlyKeepsTheseTypesInCollection3() { onlyKeepsTheseTypesInCollection4<Cast1, Cast2, Cast3, CT_Nothing1>(); }

    template<typename Cast1, typename Cast2, typename Cast3, typename Cast4>
    void onlyKeepsTheseTypesInCollection4();

    /**
     * @brief Get the collection builded
     */
    const QList<CT_ItemDrawableCollectionHierarchyStep>& collection() const;

private:
    QList<CT_ItemDrawableCollectionHierarchyStep>   m_collection;
    bool                                            m_onlyFindModels;

    void recursiveBuildFromStep(const CT_VirtualAbstractStep *step, bool findOnlyModels);
    void recursiveBuildFromModels(QList<CT_OutAbstractModel*> models, CT_ItemDrawableCollectionHierarchyResult &hir, bool findOnlyModels);
};

#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.hpp"

#endif // CT_ITEMDRAWABLECOLLECTIONBUILDERT_H
