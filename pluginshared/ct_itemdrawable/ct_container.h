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

#ifndef CT_CONTAINER_H
#define CT_CONTAINER_H

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardcontainerdrawmanager.h"

#include "ct_tools/ct_iteratortools.h"

/*!
 * \class CT_Container
 * \ingroup PluginShared_Items
 * \brief <b>Collection of ItemDrawables of the same type</b>
 *
 * \note Not for direct use by plugin developpers
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_Container : public CT_AbstractItemDrawable
{
    Q_OBJECT

public:
    CT_Container();
    CT_Container(const CT_OutAbstractItemModel *model,
                 const CT_AbstractResult *result,
                 QString defaultItemDrawableType);

    virtual ~CT_Container();

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    /**
      * \brief Ajoute un itemDrawable à la liste.
      *
      * \return false si l'itemdrawable ajout n'est pas du mme type que celui par dfaut
      */
    bool addItemDrawable(CT_AbstractItemDrawable *id);

    bool beginIterateItemDrawable();
    CT_AbstractItemDrawable* nextItemDrawable();
    CT_AbstractItemDrawable* currentItemDrawable();
    bool removeCurrentItemDrawable();

    bool removeItemDrawable(CT_AbstractItemDrawable *item);
    bool removeItemDrawableWithoutAutoDelete(CT_AbstractItemDrawable *item);

    /**
      * \brief Retourne la liste des ItemDrawable.
      */
    QList<CT_AbstractItemDrawable*>* getList() const;

    /**
     * @brief Returns the list of ItemDrawable that will be removed later from this container
     */
    const QList<CT_AbstractItemDrawable *>& itemThatWillBeRemovedLater() const;

    /**
     * @brief Inform the container that the ItemDrawable of this container will be removed later
     * @warning Not intended for direct use by plugin developper
     */
    void setItemDrawableBeRemovedLater(const CT_AbstractItemDrawable *item);

    /**
      * \brief Met  jour le centre.
      */
    void updateCenter();

    bool hasChildren() const;
    bool beginIterateChild();
    CT_AbstractItemDrawable* nextChild();

    virtual int indexOf(const CT_AbstractItemDrawable *child) const;
    virtual int getFastestIncrement() const;

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    virtual QList<CT_ItemDrawableConfiguration*> getDependantViewConfigurations();

protected:

    /**
     * @brief Called from Result or item to inform that this item will be removed from the result passed in parameter
     */
    virtual void internalSetWillBeRemovedFromResult(const CT_AbstractResult *res);

private:

    QList<CT_AbstractItemDrawable*>                 _list;
    QList<CT_AbstractItemDrawable*>                 m_itemRemovedLater;
    QString                                         _defaultItemDrawableType;
    CT_IteratorTools                                _itChildTools;
    CT_IteratorTools                                _itComputeTools;

    const static CT_StandardContainerDrawManager    CONTAINER_DRAW_MANAGER;

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        int n = _list.size();
        std::string stdStr = _defaultItemDrawableType.toStdString();

        ar & boost::serialization::base_object<CT_AbstractItemDrawableWithoutPointCloud>(*this);
        ar & stdStr & n;

        QListIterator<CT_AbstractItemDrawable*> it(_list);

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *id = it.next();

            ar & id;
        }
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        int n;
        std::string stdStr;

        ar & boost::serialization::base_object<CT_AbstractItemDrawableWithoutPointCloud>(*this);
        ar & stdStr & n;

        _defaultItemDrawableType = QString::fromStdString(stdStr);

        for(int i = 0; i<n; ++i)
        {
            CT_AbstractItemDrawable *id;

            ar & id;

            _list.append(id);
        }
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_EXPORT_KEY(CT_Container)

#else
};
#endif

#endif // CT_CONTAINER_H
