/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#ifndef DM_DOCUMENT_H
#define DM_DOCUMENT_H

#include <QObject>
#include <QList>
#include <QString>

#include "interfaces.h"

#include "dm_abstractinfo.h"

class CT_AbstractItemDrawable;

class DM_DocumentManager;
class DM_ActionsHandler;
class DM_Document;

class CT_AbstractItemGroup;
class CT_AbstractSingularItemDrawable;

/**
 * @brief Class to filter the close of a document
 */
class DM_IDocumentCloseFilter
{
public:
    virtual ~DM_IDocumentCloseFilter() {}

    /**
     * @brief Return true if the document can be closed
     */
    virtual bool canClose(const DM_Document *document) const = 0;
};

/**
 * @brief Class to filter the add of CT_AbstractItemDrawable to a document
 */
class DM_IDocumentAddFilter
{
public:
    virtual ~DM_IDocumentAddFilter() {}

    /**
     * @brief Return true if the document can add CT_AbstractItemDrawable
     */
    virtual bool canAddItemDrawable(const DM_Document *document, const CT_AbstractItemDrawable *item) const = 0;
};

/**
  * Classe qui représente un document. Un document peut avoir un
  * ou plusieurs CT_AbstractItemDrawable. Le document est géré par un DocumentManager,
  * et le document connait son manager.
  */
class DM_Document : public DocumentInterface
{
    Q_OBJECT
    Q_INTERFACES(DocumentInterface)

public:
    DM_Document(DM_DocumentManager &manager, QString title);
    virtual ~DM_Document();

    /**
     * @brief Set the close filter for this document
     */
    void setDocumentCloseFilter(const DM_IDocumentCloseFilter *filter);

    /**
     * @brief Return the close filter
     */
    DM_IDocumentCloseFilter* closeFilter() const;

    /**
     * @brief Set the add filter for this document
     */
    void setDocumentAddFilter(const DM_IDocumentAddFilter *filter);

    /**
     * @brief Return the add filter
     */
    DM_IDocumentAddFilter* addFilter() const;

    /**
      * \brief Retourne le manager du document
      */
    DM_DocumentManager* getManager() const;

    /**
      * \brief Retourne le numéro du document
      */
    int getNumber() const;

    /**
      * \brief Retourne le titre du document
      */
    QString getTitle() const;

    /**
     * @brief Returns the position of the document
     */
    virtual QPoint pos() const = 0;

    /**
     * @brief Returns the size of the document
     */
    virtual QSize size() const = 0;

    /**
     * @brief Returns if the document is maximized
     */
    virtual bool isMaximized() const = 0;

    /**
     * @brief Move the document to the position passed in parameter
     */
    virtual void move(const QPoint &pos) = 0;

    /**
     * @brief Resize he document
     */
    virtual void resize(const QSize &size) = 0;

    /**
     * @brief Change the maximized state
     */
    virtual void setMaximized(bool val) = 0;

    /**
     * @brief Return true if the document can add the CT_AbstractItemDrawable
     */
    bool canAddItemDrawable(const CT_AbstractItemDrawable *item) const;

    /**
     * @brief Call this method if you plan to add multiple CT_AbstractItemDrawable in one times
     */
    virtual void beginAddMultipleItemDrawable() = 0;

    /**
      * \brief Ajout d'un CT_AbstractItemDrawable au document
      */
    virtual void addItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Call this method after you add multiple CT_AbstractItemDrawable and if you have called the method 'beginAddMultipleItemDrawable'
     */
    virtual void endAddMultipleItemDrawable() = 0;

    /**
     * @brief Call this method if you plan to remove multiple CT_AbstractItemDrawable in one times
     */
    virtual void beginRemoveMultipleItemDrawable() = 0;

    /**
      * \brief Supprime l'CT_AbstractItemDrawable du document
      */
    virtual void removeItemDrawable(CT_AbstractItemDrawable &item);

    /**
     * @brief Call this method after you removed multiple CT_AbstractItemDrawable and if you have called the method 'beginRemoveMultipleItemDrawable'
     */
    virtual void endRemoveMultipleItemDrawable() = 0;

    /**
      * \brief Removes all CT_AbstractItemDrawable in the document that was in the result 'res'. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawableOfResult(const CT_AbstractResult &res);

    /**
      * \brief Removes all CT_AbstractItemDrawable in the document that have the model 'model'. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawableOfModel(const CT_OutAbstractModel &model);

    /**
      * \brief Removes all selected CT_AbstractItemDrawable in the document. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllSelectedItemDrawable();

    /**
      * \brief Removes all CT_AbstractItemDrawable in the document. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawable();

    /**
      * \brief Selectionne/De-Selectionne tous les CT_AbstractItemDrawable du document
      */
    void setSelectAllItemDrawable(bool select);

    /**
      * \brief Selectionne/De-Selectionne tous les CT_AbstractItemDrawable du document qui ont le modèle passé en paramètre
      */
    void setSelectAllItemDrawableOfModel(bool select, const CT_OutAbstractModel &model);

    /**
      * \brief Retourne la liste des CT_AbstractItemDrawable contenu dans ce document
      */
    const QList<CT_AbstractItemDrawable *>& getItemDrawable() const;

    /**
     * @brief Return items informations
     */
    const QHash<CT_AbstractResult *, QHash<CT_AbstractItemDrawable *, DM_AbstractInfo *> *>& getItemsInformations() const;

    /**
      * \brief Retourne la liste des CT_AbstractItemDrawable slectionn
      */
    QList<CT_AbstractItemDrawable*> getSelectedItemDrawable() const;

    /**
     * @brief Returns true if the itemdrawable is contained in this document
     */
    bool containsItemDrawable(const CT_AbstractItemDrawable *item) const;

    /**
     * @brief Returns true if the itemdrawable or at least one children is contained in this document
     */
    bool containsItemDrawableOrAtLeastOneChildren(const CT_AbstractItemDrawable *item) const;

    /**
     * @brief Returns true if this document use item color. By default return false.
     */
    virtual bool useItemColor() const;

    /**
     * @brief Set the color of the item passed in parameter. By default do nothing.
     */
    virtual void setColor(const CT_AbstractItemDrawable *item, const QColor &color);

    /**
     * @brief Returns true if the color of the itemdrawable passed in parameter is modified. By default return false.
     */
    virtual bool isColorModified(const CT_AbstractItemDrawable *item);

    /**
     * @brief Return the color of the item passed in parameter. By default return invalid color.
     */
    virtual QColor getColor(const CT_AbstractItemDrawable *item);

    /**
     * @brief Returns true if this document use octree for points. By default return false.
     */
    virtual bool useOctreeOfPoints() const;

    /**
     * @brief Returns the octree of points or NULL if usePointsOctree() return false. By default return NULL.
     */
    virtual OctreeInterface* octreeOfPoints() const;

    /**
     * @brief Returns true if this document has the capacity of change the visibility of an itemdrawable
     */
    virtual bool canChangeVisibility() const;

    /**
     * @brief Change the visibility of the item in the document
     */
    virtual void setVisible(const CT_AbstractItemDrawable *item, bool visible);

    /**
     * @brief Returns the visibility of the item in the document.
     */
    virtual bool isVisible(const CT_AbstractItemDrawable *item) const;

public slots:
    /**
     * @brief (Re)construct the octree. Do nothing by default.
     */
    virtual void constructOctreeOfPoints();

public:

    /**
      * \brief Returns the number of CT_AbstractItemDrawable
      */
    size_t nItemDrawable() const;

    /**
      * \brief Retoune le n ime CT_AbstractItemDrawable de la liste
      * \return NULL si il l'index dpasse la port de la liste
      */
    CT_AbstractItemDrawable* getItemDrawable(int i) const;

    /**
      * \brief Recherche des CT_AbstractItemDrawable dans la liste  partir du type pass en paramtre
      * \return une liste vide si aucun CT_AbstractItemDrawable n'est de ce type
      */
    QList<CT_AbstractItemDrawable*> findItemDrawable(const CT_OutAbstractModel &model) const;
    void findItemDrawable(const CT_OutAbstractModel &model, QList<CT_AbstractItemDrawable*> &outList) const;

    /**
      * \brief Recherche le premier CT_AbstractItemDrawable dans la liste  partir de son type
      * \return NULL si aucun CT_AbstractItemDrawable n'est de ce type
      */
    CT_AbstractItemDrawable* findFirstItemDrawable(const CT_OutAbstractModel &model) const;

    /**
     * @brief Return the current action
     */
    virtual CT_AbstractAction* currentAction() const = 0;

    /**
     * @brief Return the default action
     */
    virtual CT_AbstractAction* defaultAction() const = 0;

    /**
     * @brief Remove (delete) actions from the document that have the unique name passed in parameter
     */
    virtual void removeActions(const QString &uniqueName) const = 0;

    /**
      * \brief Reprsente le nombre de document cr depuis l'ouverture de l'application
      *        afin d'ajouter ce nombre au titre du document
      */
    static int              NUMBER;

protected:
    DM_DocumentManager                                                              *_manager;
    QList<CT_AbstractItemDrawable*>                                                 _listItemDrawable;
    QHash<CT_AbstractResult*, QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*>* >  m_itemsInformation;
    QString                                                                         _title;
    int                                                                             m_number;

    /**
     * @brief Create item informations in collection for the result passed in parameter.
     * @warning Verify if informations don't exist for this result before call this method
     */
    QHash<CT_AbstractItemDrawable *, DM_AbstractInfo *> *createItemInformationsForResult(CT_AbstractResult *result);

    /**
     * @brief Overload this method if you want to create your own item information. By default return NULL;
     */
    virtual DM_AbstractInfo* createNewItemInformation(const CT_AbstractItemDrawable *item) const;

    /**
     * @brief Add this group and this information to the collection, recursively add childrens item
     */
    void recursiveAddChildrensToInformationsCollection(const CT_AbstractItemGroup *group,
                                                       QHash<CT_AbstractItemDrawable*, DM_AbstractInfo*> *hash,
                                                       const bool &searchInHashIfItemExist = true);

    /**
     * @brief returns true if this model or one children is contained in this document
     */
    bool recursiveContainsItemDrawableModelOrAtLeastOneChildren(const CT_OutAbstractModel *model) const;

private:
    DM_IDocumentCloseFilter         *m_closeFilter;
    DM_IDocumentAddFilter           *m_addFilter;

signals:

    /**
     * @brief Connect to this signal in Qt::QueuedConnection if you want to
     *        create/delete a widget because this signal can be emitted in a thread
     */
    void itemDrawableAdded(CT_AbstractItemDrawable &item);

    /**
     * @brief Connect to this signal in Qt::QueuedConnection if you want to
     *        create/delete a widget because this signal can be emitted in a thread
     */
    void itemDrawableToBeRemoved(CT_AbstractItemDrawable &item);

    /**
     * @brief emit when a itemdrawable is selected
     */
    void itemDrawableSelectionChanged(CT_AbstractItemDrawable *item, bool selected);

    /**
     * @brief Connect to this signal in Qt::DirectConnection because just after
     *        it will be emit the action is deleted
     */
    void currentActionChanged(CT_AbstractAction *action);

    /**
     * @brief Connect to this signal in Qt::DirectConnection because just after
     *        it will be emit the action is deleted
     */
    void defaultActionChanged(CT_AbstractAction *action);

protected slots:

    virtual void slotItemDrawableAdded(CT_AbstractItemDrawable &item);
    virtual void slotItemToBeRemoved(CT_AbstractItemDrawable &item);
    virtual void slotItemDrawableSelectionChanged(bool select);
    virtual void slotResultDestroyed(QObject *result);
};

#endif // DM_DOCUMENT_H
