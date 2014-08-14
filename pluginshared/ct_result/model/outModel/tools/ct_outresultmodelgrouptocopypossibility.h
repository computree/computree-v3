#ifndef CT_OUTRESULTMODELGROUPTOCOPYPOSSIBILITY_H
#define CT_OUTRESULTMODELGROUPTOCOPYPOSSIBILITY_H

#include "pluginShared_global.h"

class CT_InResultModelGroupToCopy;
class CT_InStdModelPossibility;
class CT_OutAbstractResultModelGroup;

class PLUGINSHAREDSHARED_EXPORT CT_OutResultModelGroupToCopyPossibility
{
public:
    CT_OutResultModelGroupToCopyPossibility();
    ~CT_OutResultModelGroupToCopyPossibility();

    /*!
     *  \brief Getter of class
     *
     *  \return Retourne le modèle de sortie que vous pouvez modifier (ajouter des groupes ou des items) puisque c'est une copie
     *          d'un modèle de sortie d'une étape précédente
     */
    CT_OutAbstractResultModelGroup* outModelForModification() const;

    /*!
     *  \brief Getter of class
     *
     *  \return Retourne le modèle de sortie qui doit vous servir pour rechercher un modèle d'entrée à partir de la
     *          méthode "getInModel(...)" de la CT_VirtualAbstractStep. ATTENTION ne pas modifier ce modèle !
     */
    const CT_OutAbstractResultModelGroup* outModelForSearch() const;

protected:

    friend class CT_InResultModelGroupToCopy;
    friend class CT_OutResultModelGroupCopy;

    /*!
     *  Le modèle ne sera pas supprimé de la mémoire mais la possibilité oui, il faut
     *  ainsi passer une copie de la possibilité et non une référence. La possibilityForSearch est la référence
     * qui a servit à faire la copie de la possibilité, celle-ci ne sera pas supprimé de la mémoire.
     */
    bool setInModelAndPossibility(const CT_InResultModelGroupToCopy *inModel,
                                  CT_InStdModelPossibility *possibility,
                                  const CT_InStdModelPossibility *possibilityForSearch);

    CT_InResultModelGroupToCopy* inModel() const;
    CT_InStdModelPossibility* possibility() const;
    CT_InStdModelPossibility* possibilityForSearch() const;

    CT_OutResultModelGroupToCopyPossibility* copy() const;

private:

    CT_InResultModelGroupToCopy         *_inModel;
    CT_InStdModelPossibility            *_possibility;
    CT_InStdModelPossibility            *_possibilityForSearch;
};

#endif // CT_OUTRESULTMODELGROUPTOCOPYPOSSIBILITY_H
