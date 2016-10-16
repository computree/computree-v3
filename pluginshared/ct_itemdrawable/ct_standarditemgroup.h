#ifndef CT_STANDARDITEMGROUP_H
#define CT_STANDARDITEMGROUP_H

#include "ct_itemdrawable/abstract/ct_abstractstandarditemgroup.h"

/*!
 *  \brief Une classe standard représentant un groupe
 * \ingroup PluginShared_Items
 *
 *  Le groupe va se servir du modèle passé en paramètre pour créer sa structure.
 *
 *  Pour chaque groupes modèles fils de ce modèle il y aura une liste de groupes (QMap< Key = ModelName, Value = List<Groupe> >.
 *  Pour chaque items modèles fils de ce modèle il y aura un item (QMap< Key = ModelName, Value = Item >.
 *
 *  !!! Puisque dans un arbre de modèles aucun item et aucun groupe n'a le même nom : la clé des maps est le nom du modèle !!!
 *  Donc lors de la recherche d'un élément dans la map on compare la valeur retournée par la méthode uniqueName() du modèle.
 *
 *  Lors d'une copie les groupes fils seront copiés et déplacés vers une map "backup". Par contre les items seul la référence
 *  sera gardé dans une map "backup" (pas de copie en mémoire). La destruction des groupes et items "backup" ou nouveau est automatique.
 */
class PLUGINSHAREDSHARED_EXPORT CT_StandardItemGroup : public CT_AbstractStandardItemGroup
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_StandardItemGroup, CT_AbstractStandardItemGroup, Standard group)

public:

    CT_StandardItemGroup();

    /**
     * @brief Create a group with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_StandardItemGroup(const CT_OutAbstractGroupModel *model,
                         const CT_AbstractResult *result);

    /**
     * @brief Create a group with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_StandardItemGroup(const QString &modelName,
                         const CT_AbstractResult *result);

    /*!
     *  \brief Ajoute un groupe (protégé par un Mutex)
     *
     *  Ajoute un groupe à l'ItemGroup. Si le modèle du groupe passé en paramètre n'est pas trouvé dans la structure de ce groupe la
     *  méthode n'ajoutera pas le groupe et renverra false.
     *
     *  INFO : On peut ajouter plusieurs groupes ayant le même modèle dans un groupe.
     *
     *  \param group : le groupe à ajouter
     *  \return false si le groupe n'a pas pu être ajouté.
     */
    bool addGroup(CT_AbstractItemGroup *group);

    /*!
     *  \brief Ajoute un item (protégé par un Mutex)
     *
     *  INFO : On peut ajouter un seul item ayant le même modèle dans un groupe.
     *
     *  \param item : l'item à ajouter
     *
     *  \return false si l'item n'a pas pu être ajouté (existe déjà).
     */
    bool addItemDrawable(CT_AbstractSingularItemDrawable *item);

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_STANDARDITEMGROUP_H
