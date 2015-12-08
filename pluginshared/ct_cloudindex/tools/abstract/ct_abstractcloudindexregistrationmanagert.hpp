#ifndef CT_ABSTRACTCLOUDINDEXREGISTRATIONMANAGERT_HPP
#define CT_ABSTRACTCLOUDINDEXREGISTRATIONMANAGERT_HPP

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"

template<typename T>
CT_AbstractCloudIndexRegistrationManagerT<T>::~CT_AbstractCloudIndexRegistrationManagerT()
{
    delete m_mutex;
}

template<typename T>
bool CT_AbstractCloudIndexRegistrationManagerT<T>::registerIndex(typename CT_AbstractCloudIndexRegistrationManagerT<T>::CT_AbstractModifiableCIR cir)
{
    m_cirArray.append(cir.data());

    return true;
}

template<typename T>
bool CT_AbstractCloudIndexRegistrationManagerT<T>::registerIndex(typename CT_AbstractCloudIndexRegistrationManagerT<T>::CT_AbstractNotModifiableCIR cir)
{
    m_cirMergedArray.append(cir.data());

    return true;
}

template<typename T>
bool CT_AbstractCloudIndexRegistrationManagerT<T>::unregisterIndex(const CT_AbstractModifiableCloudIndexRegisteredT<T> *index)
{
    QMutexLocker locker(m_mutex);

    if(m_cirArray.removeOne((CT_AbstractModifiableCloudIndexRegisteredT<T>*)index))
        return true;

    return false;
}

template<typename T>
bool CT_AbstractCloudIndexRegistrationManagerT<T>::unregisterIndex(const CT_AbstractNotModifiableCloudIndexRegisteredT<T> *index)
{
    QMutexLocker locker(m_mutex);

    if(m_cirMergedArray.removeOne((CT_AbstractNotModifiableCloudIndexRegisteredT<T>*)index))
        return true;

    return false;
}

template<typename T>
void CT_AbstractCloudIndexRegistrationManagerT<T>::lockAllDocuments() const
{
    if(PS_CONTEXT->documentManager() != NULL)
        PS_CONTEXT->documentManager()->lockAllDocuments();
}

template<typename T>
void CT_AbstractCloudIndexRegistrationManagerT<T>::unlockAllDocuments() const
{
    if(PS_CONTEXT->documentManager() != NULL)
        PS_CONTEXT->documentManager()->unlockAllDocuments();
}

template<typename T>
template<class CIR, class CI>
CIR* CT_AbstractCloudIndexRegistrationManagerT<T>::createNewIndexRegistered(CI *index) const
{
    CIR* cir = new CIR(index);
    cir->setCloudIndexRegistrationManager(this);

    return cir;
}

#endif // CT_ABSTRACTCLOUDINDEXREGISTRATIONMANAGERT_HPP
