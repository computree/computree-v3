#ifndef CT_ABSTRACTSTANDARDITEMGROUP_H
#define CT_ABSTRACTSTANDARDITEMGROUP_H

#include "ct_itemdrawable/abstract/ct_abstractitemgroup.h"
#include "ct_itemdrawable/ct_container.h"

#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractsingularitemmodel.h"

#include "ct_itemdrawable/tools/drawmanager/ct_standardstandarditemgroupdrawmanager.h"

#include <QHash>
#include <QMutex>

class CT_AbstractReader;

class PLUGINSHAREDSHARED_EXPORT CT_AbstractStandardItemGroup : public CT_AbstractItemGroup
{
public:
    CT_AbstractStandardItemGroup();

    /**
     * @brief Create a group with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_AbstractStandardItemGroup(const CT_OutAbstractGroupModel *model,
                                 const CT_AbstractResult *result);

    /**
     * @brief Create a group with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_AbstractStandardItemGroup(const QString &modelName,
                                 const CT_AbstractResult *result);

    ~CT_AbstractStandardItemGroup();

    QString getType() const;
    static QString staticGetType();

    bool isEmpty() const;

    /****** SINGULAR ITEMDRAWABLE METHODS ********/

    bool removeItem(CT_AbstractSingularItemDrawable *item);

    QHash<QString, CT_GroupItemDrawableContainer*> items() const;

    bool containsItemDrawable(const CT_InAbstractSingularItemModel *model) const;

    bool containsItemDrawable(const CT_OutAbstractSingularItemModel *model) const;

    int nItems() const;

    bool isEmptyOfSingularItems() const;

    CT_AbstractSingularItemDrawable* item(const CT_OutAbstractSingularItemModel *model) const;

    QList<CT_AbstractSingularItemDrawable*> items(const CT_InAbstractSingularItemModel *model) const;

    CT_AbstractSingularItemDrawable* firstItem(const CT_InAbstractSingularItemModel *model) const;

    const QHash<QString, CT_GroupItemDrawableContainer*>& itemsNew() const;
    const QHash<QString, CT_GroupItemDrawableContainer*>& itemsBackup() const;

    /****** GROUPS METHODS ********/

    virtual bool removeGroup(CT_AbstractItemGroup *group);   // (protégé par un Mutex)

    QHash<QString, CT_Container*> groups() const;

    int nGroups() const;

    bool isEmptyOfGroups() const;

    bool containsGroup(const CT_OutAbstractGroupModel *model) const;

    const QHash<QString, CT_Container*>& groupsNew() const;
    const QHash<QString, CT_Container*>& groupsBackup() const;

    // HERITE DE ItemDrawable //

    QList<CT_ItemDrawableConfiguration*> getDependantViewConfigurations();
    void draw(GraphicsViewInterface &view, PainterInterface &painter);  // (protégé par un Mutex)

    /**
     * @brief Delete all groups and items that must be removed later (recursively)
     * @warning Not intended for direct use by plugin developper
     */
    void recursiveSetComplete();

private:

    const static CT_StandardStandardItemGroupDrawManager SIG_DRAW_MANAGER;

private:

    QHash<QString, CT_Container*>                           _groupsNew;     /*!< Une map ayant comme clé le nom du modèle et comme valeur un container de groupes (ceux ajoutés après une copie ou un appel au constructeur) */
    QHash<QString, CT_Container*>                           _groupsBackup;  /*!< Une map ayant comme clé le nom du modèle et comme valeur un container de groupes (ceux contenu dans _groupsNew avant la copie ont été transférés ici) */

    QHash<QString, CT_GroupItemDrawableContainer*>          _itemsNew;      /*!< Une map ayant comme clé le nom du modèle et comme valeur un container d'ItemDrawable ajouté dans ce groupe. Il faudra les supprimer de la mémoire si on détruit le groupe. */
    QHash<QString, CT_GroupItemDrawableContainer*>          _itemsBackup;   /*!< Une map ayant comme clé le nom du modèle et comme valeur un container d'ItemDrawable qu'il ne faut pas supprimer de la mémoire
                                                                         mais juste effacer puisque lors d'une copie d'un groupe les ItemDrawable de la liste "itemsNew"
                                                                         sont déplacés dans cette liste. */
protected:

    QMutex                                                  *_mutexAccessGroup;
    QMutex                                                  *_mutexAccessItem;

    /**
     * @brief Called from result or parent group to inform that this item will be deleted from the result passed in parameter
     */
    virtual void internalSetWillBeRemovedFromResult(const CT_AbstractResult *res);

    bool protectedAddGroup(CT_AbstractItemGroup *group);

    bool protectedAddItemDrawable(CT_AbstractSingularItemDrawable *item);

    bool copyStructure(CT_AbstractStandardItemGroup *itemGroup);

    bool copyGroups(CT_AbstractResult *result, const CT_OutAbstractGroupModel &modelCopy,
                    const QHash<QString, CT_Container*> &mapSource,
                    QHash<QString, CT_Container *> &mapDest,
                    CT_AbstractStandardItemGroup &groupDest);

    bool copyItems(const QHash<QString, CT_GroupItemDrawableContainer*> &mapSource,
                   const CT_OutAbstractGroupModel &groupModelCopy,
                   CT_AbstractStandardItemGroup &groupDest);

    void recursiveGetDependantViewConfigurations(QList<CT_ItemDrawableConfiguration*> &list, CT_OutAbstractGroupModel* gModel);
};

#endif // CT_ABSTRACTSTANDARDITEMGROUP_H
