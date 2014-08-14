#ifndef CT_ABSTRACTCLOUDINDEXREGISTRATIONMANAGERT_H
#define CT_ABSTRACTCLOUDINDEXREGISTRATIONMANAGERT_H

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanager.h"
#include "ct_cloud/tools/iglobalcloudlistener.h"

#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregisteredt.h"
#include "ct_cloudindex/registered/abstract/ct_abstractnotmodifiablecloudindexregisteredt.h"

#include <QMutex>

/**
 * A utility class that register cloud index of T (point, face, etc...) and sync indexes with
 * the global cloud of T when elements was deleted from them.
 */
template<typename T>
class CT_AbstractCloudIndexRegistrationManagerT : public CT_AbstractCloudIndexRegistrationManager, public IGlobalCloudListener
{
public:

    typedef QSharedPointer< CT_AbstractModifiableCloudIndexRegisteredT<T> >     CT_AbstractModifiableCIR;
    typedef QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<T> >  CT_AbstractNotModifiableCIR;
    typedef QSharedPointer< CT_AbstractCloudIndexRegisteredT<T> >               CT_AbstractCIR;

    CT_AbstractCloudIndexRegistrationManagerT() : CT_AbstractCloudIndexRegistrationManager() {}

protected:

    /**
     * @brief la liste des nuages d'index enregistré (à synchroniser)
     */
    QList< CT_AbstractModifiableCloudIndexRegisteredT<T>* >     m_cirArray;
    QList< CT_AbstractNotModifiableCloudIndexRegisteredT<T>* >  m_cirMergedArray;

    QMutex                                                      m_mutex;

    template<typename U> friend class CT_AbstractGlobalCloudManagerT;
    template<typename U> friend class CT_AbstractModifiableCloudIndexRegisteredT;
    template<typename U> friend class CT_AbstractNotModifiableCloudIndexRegisteredT;

    /**
     * @brief Inscrit un nuage d'index au gestionnaire. Celui-ci sera synchronisé. (utilisé par CT_AbstractGlobalCloudManagerT)
     *
     * @param index : le nuage d'index préalablement enregistré
     * @return true si il a bien été inscrit
     */
    bool registerIndex(typename CT_AbstractCloudIndexRegistrationManagerT<T>::CT_AbstractModifiableCIR cir);
    bool registerIndex(typename CT_AbstractCloudIndexRegistrationManagerT<T>::CT_AbstractNotModifiableCIR cir);

    /**
     * @brief Désinscrit un nuage d'index du gestionnaire. Celui-ci ne sera plus synchronisé ! (utilisé par CT_AbstractGlobalCloudManagerT
     * et CT_AbstractNotModifiableCloudIndexRegisteredT et CT_AbstractModifiableCloudIndexRegisteredT)
     *
     * @param index : le nuage d'index préalablement enregistré
     * @return true si il a bien été désinscrit
     */
    bool unregisterIndex(const CT_AbstractModifiableCloudIndexRegisteredT<T> *index);
    bool unregisterIndex(const CT_AbstractNotModifiableCloudIndexRegisteredT<T> *index);

    template<class CIR, class CI>
    CIR* createNewIndexRegistered(CI *index) const;

    void lockAllDocuments() const;
    void unlockAllDocuments() const;
};

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.hpp"

#endif // CT_ABSTRACTCLOUDINDEXREGISTRATIONMANAGERT_H
