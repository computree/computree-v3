#ifndef CT_IDVDCREATOR_H
#define CT_IDVDCREATOR_H

#include "ct_tools/itemdrawable/ct_standarditemdatavaluedelegatet.h"
#include "ct_tools/itemdrawable/ct_standarditemdatavaluedelegatestatict.h"
#include "ct_tools/itemdrawable/ct_standardarraytitemdatavaluedelegatet.h"
#include "ct_tools/itemdrawable/ct_standardglobalclouditemdatavaluedelegatet.h"

class PLUGINSHAREDSHARED_EXPORT CT_IDVDCreator
{
public:
    /**
     *  @brief Create a wrapper to a const function that return a Scalar Type :
     *         ScalarType myFunction() const
     */
    template <typename T, typename C>
    static CT_StandardItemDataValueDelegateT<C, T>* staticScalarFunc(T (C::*func)() const)
    {
        return new CT_StandardItemDataValueDelegateT<C, T>(func);
    }

    /**
     *  @brief Create a wrapper to a static variable :
     */
    template <typename T>
    static CT_StandardItemDataValueDelegateStaticT<T>* staticScalarVar(const T &var)
    {
        return new CT_StandardItemDataValueDelegateStaticT<T>(var);
    }

    /**
     *  @brief Create a wrapper to a const function that return a Array templated :
     *         const Array<ScalarType>* myFunction() const
     */
    template < template< typename T > class P, typename T, typename C>
    static CT_StandardArrayTItemDataValueDelegateT<C, P, T>* staticArrayTFunc(const P<T>* (C::*func)() const)
    {
        return new CT_StandardArrayTItemDataValueDelegateT<C, P, T>(func);
    }

    /**
     *  @brief Create a wrapper to a const function that return a Cloud and a second
     *         function that return the CloudIndex :
     *         const Cloud* getCloudFunction() const
     *         const CloudIndex* getCloudIndexFunction() const
     *
     * @param indexVar : per example if you want to wrap the X variable of a PointCloud you set 0 to indexVar ===> double value = cloud->data((*cloudIndex)[index], indexVar);
     */
    template <typename CL, typename CLI, typename C>
    static CT_StandardGlobalCloudItemDataValueDelegateT<C, CL, CLI>* staticCloudFunc(const CL* (C::*fa)() const, const CLI* (C::*fb)() const, const int &indexVar)
    {
        return new CT_StandardGlobalCloudItemDataValueDelegateT<C, CL, CLI>(fa, fb, indexVar);
    }
};

#endif // CT_IDVDCREATOR_H
