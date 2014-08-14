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

class ItemDrawable;

class DM_DocumentManager;
class DM_ActionsHandler;
class DM_Document;

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
 * @brief Class to filter the add of ItemDrawable to a document
 */
class DM_IDocumentAddFilter
{
public:
    virtual ~DM_IDocumentAddFilter() {}

    /**
     * @brief Return true if the document can add ItemDrawable
     */
    virtual bool canAddItemDrawable(const DM_Document *document, const ItemDrawable *item) const = 0;
};

/**
  * Classe qui représente un document. Un document peut avoir un
  * ou plusieurs ItemDrawable. Le document est géré par un DocumentManager,
  * et le document connait son manager.
  */
class DM_Document : public DocumentInterface
{
    Q_OBJECT
    Q_INTERFACES(DocumentInterface)

public:
    DM_Document(DM_DocumentManager &manager, QString title);

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
     * @brief Return true if the document can add the ItemDrawable
     */
    bool canAddItemDrawable(const ItemDrawable *item) const;

    /**
     * @brief Call this method if you plan to add multiple ItemDrawable in one times
     */
    virtual void beginAddMultipleItemDrawable() = 0;

    /**
      * \brief Ajout d'un ItemDrawable au document
      */
    virtual void addItemDrawable(ItemDrawable &item);

    /**
     * @brief Call this method after you add multiple ItemDrawable and if you have called the method 'beginAddMultipleItemDrawable'
     */
    virtual void endAddMultipleItemDrawable() = 0;

    /**
     * @brief Call this method if you plan to remove multiple ItemDrawable in one times
     */
    virtual void beginRemoveMultipleItemDrawable() = 0;

    /**
      * \brief Supprime l'ItemDrawable du document
      */
    virtual void removeItemDrawable(ItemDrawable &item);

    /**
     * @brief Call this method after you removed multiple ItemDrawable and if you have called the method 'beginRemoveMultipleItemDrawable'
     */
    virtual void endRemoveMultipleItemDrawable() = 0;

    /**
      * \brief Removes all ItemDrawable in the document that was in the result 'res'. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawableOfResult(const Result &res);

    /**
      * \brief Removes all ItemDrawable in the document that have the model 'model'. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawableOfModel(const IItemModel &model);

    /**
      * \brief Removes all selected ItemDrawable in the document. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllSelectedItemDrawable();

    /**
      * \brief Removes all ItemDrawable in the document. Call the method 'beginRemoveMultipleItemDrawable' and 'endRemoveMultipleItemDrawable' automatically
      */
    virtual void removeAllItemDrawable();

    /**
      * \brief Selectionne/De-Selectionne tous les ItemDrawable du document
      */
    void setSelectAllItemDrawable(bool select);

    /**
      * \brief Selectionne/De-Selectionne tous les ItemDrawable du document qui ont le modèle passé en paramètre
      */
    void setSelectAllItemDrawableOfModel(bool select, const IItemModel &model);

    /**
      * \brief Retourne la liste des ItemDrawable contenu dans ce document
      */
    const QList<ItemDrawable *>& getItemDrawable() const;

    /**
      * \brief Retourne la liste des ItemDrawable slectionn
      */
    QList<ItemDrawable*> getSelectedItemDrawable() const;

    /**
      * \brief Returns the number of ItemDrawable
      */
    size_t nItemDrawable() const;

    /**
      * \brief Retoune le n ime ItemDrawable de la liste
      * \return NULL si il l'index dpasse la port de la liste
      */
    ItemDrawable* getItemDrawable(int i) const;

    /**
      * \brief Recherche des ItemDrawable dans la liste  partir du type pass en paramtre
      * \return une liste vide si aucun ItemDrawable n'est de ce type
      */
    QList<ItemDrawable*> findItemDrawable(const IItemModel &model) const;
    void findItemDrawable(const IItemModel &model, QList<ItemDrawable*> &outList) const;

    /**
      * \brief Recherche le premier ItemDrawable dans la liste  partir de son type
      * \return NULL si aucun ItemDrawable n'est de ce type
      */
    ItemDrawable* findFirstItemDrawable(const IItemModel &model) const;

    /**
     * @brief Return the current action
     */
    virtual ActionInterface* currentAction() const = 0;

    /**
     * @brief Return the default action
     */
    virtual ActionInterface* defaultAction() const = 0;

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
    DM_DocumentManager              *_manager;
    QList<ItemDrawable*>            _listItemDrawable;
    QString                         _title;
    int                             m_number;

private:
    DM_IDocumentCloseFilter         *m_closeFilter;
    DM_IDocumentAddFilter           *m_addFilter;

signals:

    /**
     * @brief Connect to this signal in Qt::QueuedConnection if you want to
     *        create/delete a widget because this signal can be emitted in a thread
     */
    void itemDrawableAdded(ItemDrawable &item);

    /**
     * @brief Connect to this signal in Qt::QueuedConnection if you want to
     *        create/delete a widget because this signal can be emitted in a thread
     */
    void itemDrawableToBeRemoved(ItemDrawable &item);

    /**
     * @brief emit when a itemdrawable is selected
     */
    void itemDrawableSelectionChanged(ItemDrawable *item, bool selected);

    /**
     * @brief Connect to this signal in Qt::DirectConnection because just after
     *        it will be emit the action is deleted
     */
    void currentActionChanged(ActionInterface *action);

    /**
     * @brief Connect to this signal in Qt::DirectConnection because just after
     *        it will be emit the action is deleted
     */
    void defaultActionChanged(ActionInterface *action);

private slots:

    void slotItemDrawableAdded(ItemDrawable &item);
    void slotItemToBeRemoved(ItemDrawable &item);
    void slotItemDrawableSelectionChanged(bool select);
};

#endif // DM_DOCUMENT_H
