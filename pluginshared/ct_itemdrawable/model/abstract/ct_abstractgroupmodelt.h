#ifndef CT_ABSTRACTGROUPMODELT_H
#define CT_ABSTRACTGROUPMODELT_H

#include "pluginShared_global.h"

#include "ct_tools/model/ct_autorenamemodels.h"

#include <QList>
#include <QString>

#if defined(_MSC_VER)
#pragma warning( disable: 4250 )    // disable dominance warning
#endif

class SettingsNodeGroup;

/**
 * @brief Represent a model for an group (IN or OUT). A group can have other groups and singular items for childrens.
 *
 *        Use the template parameters to define if you want a IN or OUT model
 */
template< class ItemT, class InheritedT >
class CT_AbstractGroupModelT : public InheritedT
{
public:
    /*!
    *  \brief Constructeur
    *
    *  Constructeur de la classe
    */
    CT_AbstractGroupModelT(const QString &uniqueName,
                           const QString &description,
                           const QString &displayableName);
    virtual ~CT_AbstractGroupModelT();

    /*!
     *  \brief Retourne le groupe racine
     */
    virtual CT_AbstractGroupModelT<ItemT,InheritedT>* rootGroup() const;

    /*!
     *  \brief Getter of the class
     *
     *  \return Retourne le groupe modèle parent
     */
    virtual CT_AbstractGroupModelT<ItemT,InheritedT>* parentGroup() const;

   /*!
    *  \brief Ajoute un groupe à ce groupe
    *
    *  \param group : le groupe à ajouter
    *
    *  \return Retourne false et détruit le groupe de la mémoire si le groupe (modelName) ou que ses items (modelName) existe déjà dans l'arborescence
    */
    virtual bool addGroup(CT_AbstractGroupModelT<ItemT,InheritedT> *group);

    /*!
     *  \brief Ajoute un groupe à ce groupe
     *
     *  \param group : le groupe à ajouter
     *  \param autoName : une référence sur un objet de type CT_AutoRenameModels qui permet de renommer
     *                    automatiquement le groupe ajouté. Ainsi vous n'avez pas besoin de définir
     *                    vous même un nom unique. Par contre il faudra bien garder l'objet CT_AutoRenameModels
     *                    si vous voulez récupérer plus tard le nom qui a été attribué au groupe.
     *
     *  ATTENTION : le groupe doit être vide lorsque vous l'ajoutez ! pas d'items ni de groupes fils
     *
     *  \return Retourne false et détruit le groupe de la mémoire si il y a eu une erreur, par exemple si le groupe n'était pas vide
     */
    virtual bool addGroup(CT_AbstractGroupModelT<ItemT,InheritedT> *group, CT_AutoRenameModels &autoName);

    /*!
     *  \brief Supprime un groupe de ce groupe (supprime aussi de la mémoire)
     *
     *  \param group : le groupe modèle à supprimer
     *
     *  \return true si le groupe a été trouvé et supprimé
     */
    virtual bool removeGroup(CT_AbstractGroupModelT<ItemT,InheritedT> *group);

   /*!
    *  \brief Ajoute un item à ce groupe
    *
    *  \param item : l'item à ajouter
    *
    *  \return Retourne false et détruit l'item de la mémoire si l'item (modelName OU itemType) existe déjà dans la liste des items de ce groupe
    */
    virtual bool addItem(ItemT *item);

    /*!
     *  \brief Ajoute un item à ce groupe
     *
     *  \param item : l'item à ajouter
     *  \param autoName : une référence sur un objet de type CT_AutoRenameModels qui permet de renommer
     *                    automatiquement l'item ajouté. Ainsi vous n'avez pas besoin de définir
     *                    vous même un nom unique. Par contre il faudra bien garder l'objet CT_AutoRenameModels
     *                    si vous voulez récupérer plus tard le nom qui a été attribué à l'item.
     *
     *  \return Retourne false et détruit l'item de la mémoire si il y a eu une erreur (normalement jamais)
     */
    virtual bool addItem(ItemT *item, CT_AutoRenameModels &autoName);

    /*!
     *  \brief Supprime un item de ce groupe (supprime aussi de la mémoire)
     *
     *  \param item : l'item modèle à supprimer
     *
     *  \return true si l'item a été trouvé et supprimé
     */
    virtual bool removeItem(ItemT *item);

    /*!
     *  \brief Getter of the class
     *
     *  \return Retourne la liste des groupes modèles
     */
    const QList<CT_AbstractGroupModelT<ItemT,InheritedT>*>& groups() const;

    /*!
     *  \brief Getter of the class
     *
     *  \return Retourne la liste des items modèles
     */
    const QList<ItemT*>& items() const;

    /*!
     *  \brief Getter of the class
     *
     *  \param modelName : le nom du modèle à trouver
     *
     *  \return Retourne le groupe modèle trouvé à partir de son nom. NULL si aucun modèle ne correspond.
     */
    virtual CT_AbstractGroupModelT<ItemT,InheritedT>* findGroup(const QString &modelName) const;

    /*!
     *  \brief Getter of the class
     *
     *  \param modelName : le nom du modèle à trouver
     *
     *  \return Retourne l'item modèle trouvé à partir de son nom. NULL si aucun modèle ne correspond.
     */
    virtual ItemT* findItem(const QString &modelName) const;

    /*!
     *  \brief Supprime les groupes
     */
    virtual void clearGroups();

    /*!
     *  \brief Supprime les items
     */
    virtual void clearItems();

    /*!
     *  \brief Supprime les groupes et les items
     */
    virtual void clear();

    // INHERITED FROM CT_AbstractModel
    virtual QList<CT_AbstractModel*> childrens() const;
    virtual bool isEmpty() const;

    /*!
     *  \brief Retourne une copie du groupes modèles et de ses modèles fils
     */
    virtual CT_AbstractGroupModelT<ItemT,InheritedT>* copyGroup() const = 0;

protected:

    /*!
     *  \brief Ajoute un groupe à ce groupe sans faire de vérification. Utile lors de la copie.
     *
     *  \param group : le groupe à ajouter
     */
    virtual void addGroupWithoutVerification(CT_AbstractGroupModelT<ItemT,InheritedT> *group);

    /*!
     *  \brief Ajoute un item à ce groupe sans faire de vérification. Utile lors de la copie.
     *
     *  \param item : l'item à ajouter
     */
    virtual void addItemWithoutVerification(ItemT *item);

    /**
     * @brief Called when a group is added
     */
    virtual void groupAdded(const CT_AbstractGroupModelT<ItemT,InheritedT> *group) { Q_UNUSED(group) }

    /**
     * @brief Called when a item is added
     */
    virtual void itemAdded(const ItemT *item) { Q_UNUSED(item) }

    /**
     * @brief Called when a group will be removed
     */
    virtual void groupRemoved(const CT_AbstractGroupModelT<ItemT,InheritedT> *group) { Q_UNUSED(group) }

    /**
     * @brief Called when a item will be removed
     */
    virtual void itemRemoved(const ItemT *item) { Q_UNUSED(item) }

private:

    QList< CT_AbstractGroupModelT<ItemT,InheritedT>* >   _groups;
    QList<ItemT*>                                       _items;
};

// Includes the template implementations
#include "ct_abstractgroupmodelt.hpp"

#endif // CT_ABSTRACTGROUPMODELT_H
