#ifndef CT_REPOSITORYMANAGER_H
#define CT_REPOSITORYMANAGER_H

#include "ct_global/ct_repository.h"

class PLUGINSHAREDSHARED_EXPORT CT_RepositoryManager
{
public:

    ~CT_RepositoryManager();

    /**
     * @brief repository getter
     * @return return the repository
     */
    CT_Repository* repository();

private:

    CT_Repository       *m_repository;

    friend class CT_Context;

    explicit CT_RepositoryManager();
};

#endif // CT_REPOSITORYMANAGER_H
