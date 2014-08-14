/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/


#ifndef CT_ABSTRACTRESULT_H
#define CT_ABSTRACTRESULT_H

#include "interfaces.h"
#include "pluginShared_global.h"

#include <QList>
#include <QMutex>

#include "ct_result/ct_resultcopymodelist.h"

#ifdef USE_BOOST_OLD
#include "serialization.h"
#endif

#include "ct_result/ct_resultinitializedata.h"

class CT_VirtualAbstractStep;
class CT_AbstractItemDrawable;
class CT_OutAbstractResultModel;
class CT_OutResultManager;

/**
 * @brief Class that represent a result
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractResult : public QObject
{
    Q_OBJECT
public:

    CT_AbstractResult();
    CT_AbstractResult(const CT_OutAbstractResultModel *model,
                      CT_ResultInitializeData &data,
                      CT_VirtualAbstractStep &parentStep);
    virtual ~CT_AbstractResult();

    /**
     * @brief Set or not the result busy
     *
     * Call this method with "true" when you want to access to element of the result (per example if you iterate over group or item or ...)
     * Call this method with "false" when you have finished to access element
     *
     * When you call this method with true it will block your thread if another thread as set the result busy
     * You will be unlocked when the resource is available
     */
    void setBusy(bool busy);

    /**
     * @brief Returns true if another thread as set this result busy
     */
    bool isBusy() const;

    /**
        @brief Returns a unique type for a result
      * @warning DON'T FORGET to overload this method if you INHERIT from this class or a subclass
      */
    virtual QString getType() const;

    /**
      * @brief Returns a unique type for a result
      * @warning DON'T FORGET to overload this method if you INHERIT from this class or a subclass
      */
    static QString staticGetType();

    /**
     * @brief Set if this result must auto delete it's root elements in the destructor. True by default.
     */
    void setAutoDelete(bool autoDelete);

    /**
     * @brief Returns true if this result auto delete it's root elements in the destructor.
     */
    bool isAutoDelete() const;

    /**
     * @brief Returns the model of this result
     */
    CT_OutAbstractResultModel* model() const;

    /**
     * @brief Returns the step who created this result
     */
    CT_VirtualAbstractStep* parentStep() const;

    /**
     * @brief Returns true if at least one element of this result is visible in a document
     */
    bool isVisible() const;

    /**
     * @brief Set the result name (used in gui)
     */
    void setName(const QString &name);

    /**
     * @brief Returns the result name
     */
    QString getName() const;

    /**
     * @brief Returns a tooltip when the mouse cursor get over the result in gui
     */
    virtual QString getToolTip() const;

    /**
     * @brief Returns the clear memory progress (0 if clear is complete)
     */
    int getClearFromMemoryProgress() const;

    /**
     * @brief Returns true if this result is cleared from memory (the result is empty)
     */
    bool isClearedFromMemory() const;

    /**
     * @brief Clear this result : remove all elements from this result (delete from memory if isAutoDelete() returns true)
     */
    void clearFromMemory();

    /**
     * @brief Use this method to remove an item from this result or something in the structure
     *
     *        Prefer use a MutableIterator to remove an item because is faster than this method.
     *
     * @param parent : the parent of the item or NULL if the item is the root
     * @param item : the item to remove from this result
     * @return true if item is removed
     */
    virtual bool removeItemSomethingInStructure(CT_AbstractItemDrawable *parent,
                                                        CT_AbstractItemDrawable *item) = 0;

    /**
     * @brief Returns a copy of the result and its childrens
     * @param model : the new model for the copy
     * @param data : initialize data
     * @param parentStep : the step who will keep the copy
     * @param copyModeList : a list of copy mode
     * @return NULL if the model is not valid otherwise a copy of the results and its children. If in the structure of the model
     *         there was missing models : childrens that the new model is not found will not be copied !
     */
    virtual CT_AbstractResult* createNewInstance(CT_OutAbstractResultModel *model,
                                                 CT_ResultInitializeData &data,
                                                 CT_VirtualAbstractStep &parentStep,
                                                 CT_ResultCopyModeList copyModeList = (CT_ResultCopyModeList() << CT_ResultCopyModeList::DontCopyItemDrawable)) = 0;

protected:

    CT_ResultInitializeData *_dataInit;

    friend class CT_OutResultManager;

    /**
     * @brief Called from CT_OutResultManager to inform the result that is complete. The method
     *        "isClearedFromMemory" will now returns false.
     */
    virtual void setComplete();

    friend class CT_AbstractItemDrawable;

    /**
     * @brief Called from CT_AbstractItemDrawable to inform the result that a new item is added to a document.
     *        The signal visibilityChange(true); will be emitted if the number of item visible change to 1
     */
    void incrementItemVisible();

    /**
     * @brief Called from CT_AbstractItemDrawable to inform the result that a new item is removed from a document.
     *        The signal visibilityChange(false); will be emitted if the number of item visible change to 0
     */
    void decrementItemVisible();

    /**
     * @brief Overload this method to clear elements of your result
     */
    virtual void clearFromMemoryProtected() = 0;

protected slots:

    /**
     * @brief Call this method to inform the progress of the clear memory process
     */
    void setClearFromMemoryProgress(int progress);

private:

    CT_VirtualAbstractStep      *_parentStep;
    CT_OutAbstractResultModel   *_model;

    QMutex                      *_mutexBusy;

    QString                     _name;
    bool                        _autoDelete;
    int                         _numberOfItemVisible;
    int                         _clearFromMemoryProgress;

signals:
    /**
     * @brief Emitted if one element is visible in a document or when zero element is visible in document
     */
    void visibilityChange(bool visible);

    /**
     * @brief Emitted when clear memory progress changed
     */
    void clearFromMemoryInProgress(int progress);

    /**
     * @brief Emitted when the result busy state changed
     */
    void busyStateChanged(bool busy);
};

#endif // CT_ABSTRACTRESULT_H
