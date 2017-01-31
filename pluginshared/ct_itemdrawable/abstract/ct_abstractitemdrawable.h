/****************************************************************************

 Copyright (C) 2010-2012 the Office National des ForÃªts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et MÃ©tiers (ENSAM), Cluny, France.
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

#ifndef CT_ABSTRACTITEMDRAWABLE_H
#define CT_ABSTRACTITEMDRAWABLE_H

#include "ct_item/abstract/ct_abstractitem.h"
#include "ct_result/ct_resultcopymodelist.h"
#include "ct_itemdrawable/tools/drawmanager/abstract/ct_abstractitemdrawabledrawmanager.h"
#include "ct_attributes/tools/ct_defaultitemattributemanager.h"

#include <QMap>
#include <QString>

#include <eigen/Eigen/Core>

class CT_OutAbstractItemModel;
class CT_AbstractResult;

/**
  * @brief Use this define in your class to add type information
  * @example class MyItemDrawable : public CT_AbstractSingularItemDrawable
  *          {
  *             Q_OBJECT
  *             CT_TYPE_IMPL_MACRO(CT_AbstractSingularItemDrawable, CT_AbstractItemDrawable, Singular Item)
  *
  *          public:
  *             MyItemDrawable();
  *             ....
  *          };
  */
#define CT_TYPE_IMPL_MACRO(ThisClassName, SuperClassName, Name) public: \
    virtual QString name() const {return staticName();} \
    static QString staticName() {return #Name;} \
    QString getType() const { return staticGetType(); } \
    static QString staticGetType() { QString statType = SuperClassName::staticGetType() + "/" + #ThisClassName; return statType; } \
    static void staticInitNameTypeCorresp() { ThisClassName::addNameTypeCorresp(staticGetType(), staticName()); } \
    CT_StaticInitDefaultIAInvoker<ThisClassName> ThisClassName::INVOKER_DEFAULT_NT = CT_StaticInitDefaultIAInvoker<ThisClassName>(&staticInitNameTypeCorresp);

/**
 * @brief Represent a item that can be added in a result or in another item
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemDrawable : public CT_AbstractItem
{
    Q_OBJECT

public:

    CT_AbstractItemDrawable();

    /**
     * @brief Create a CT_AbstractItemDrawable with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_AbstractItemDrawable(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result);

    /**
     * @brief Create a CT_AbstractItemDrawable with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_AbstractItemDrawable(const QString &modelUniqueName, const CT_AbstractResult *result);

    virtual ~CT_AbstractItemDrawable();

    /**
      * @warning DON'T FORGET TO REDEFINE THIS METHOD IN YOUR ITEM !!!
      *
      * @brief Redefine this method by using CT_TYPE_IMPL_MACRO (show example on top of this file)
      */
    virtual QString getType() const = 0;

    /**
      * @warning DON'T FORGET TO DEFINE THIS METHOD IN YOUR ITEM !!!
      *
      * @brief Redefine this method by using CT_TYPE_IMPL_MACRO (show example on top of this file)
      */
    static QString staticGetType();

    /**
     * @brief Returns the unique ID of this item
     */
    inline quint64 id() const {return _id;}

    /**
     * @brief Returns the name of this item (the name of the class). By default call "metaObject()->className()". You can
     *        overload this method if you want to show another name
     */
    virtual QString name() const;

    static QString staticName();

    /**
     * @brief Returns a displayable name. Use the method "setDisplayableName" to change them. By default if the displayable
     *        name is not empty return it. Otherwise if the displable name of the model is not empty return it. Otherwise
     *        return the result of the method "name".
     */
    QString displayableName() const;

    /**
     * @brief Change the displayable name of this item. Name used in GUI.
     */
    void setDisplayableName(const QString &displayableName);

    /**
     * @brief Returns information of this item that can be displayed in a tooltip per example
     */
    virtual QString getInfo();

    /**
     * @brief Change the result that contains this item.
     * @warning Not intended for direct use by plugin developper
     */
    virtual void changeResult(const CT_AbstractResult *newRes);

    /**
     * @brief Returns the model of this item
     */
    CT_OutAbstractItemModel* model() const;

    /**
     * @brief Set the model to this item
     * @warning Not intended for direct use by plugin developper
     */
    void setModel(const CT_OutAbstractItemModel *model);

    void setModel(const QString &modelUniqueName);

    /**
     * @brief Returns the parent item
     */
    CT_AbstractItemDrawable* itemParent() const;

    /**
     * @brief Returns the list of documents where the item is added
     */
    QList<DocumentInterface*> document() const;

    /**
     * @brief Called from GUI to inform this item that he was added to the document passed in parameter
     * @return false if the item is already in this document
     * @warning Not intended for direct use by plugin developper
     */
    virtual bool addDocumentParent(DocumentInterface *doc);

    /**
     * @brief Called from GUI to inform this item that he will be removed from the document passed in parameter
     * @warning Not intended for direct use by plugin developper
     */
    virtual void aboutToBeRemovedFromDocument(DocumentInterface *doc) { Q_UNUSED(doc) }

    /**
     * @brief Called from GUI to inform this item that he was removed from the document passed in parameter
     * @warning Not intended for direct use by plugin developper
     */
    virtual void removeDocumentParent(DocumentInterface *doc);

    /**
     * @brief Set this item selected or not in all documents.
     */
    void setSelected(bool value);

    /**
     * @brief Returns true if this item is selected.
     */
    bool isSelected() const;

    /**
     * @brief Returns true it this item is displayed at least in one document
     */
    bool isDisplayed() const;

    /**
     * @brief Set the x center coordinate of this item
     */
    virtual void setCenterX(double x);

    /**
     * @brief Set the y center coordinate of this item
     */
    virtual void setCenterY(double y);

    /**
     * @brief Set the z center coordinate of this item
     */
    virtual void setCenterZ(double z);

    inline virtual void setCenterCoordinate(const Eigen::Vector3d& center) {_centerCoordinate = center;}

    /**
     * @brief Returns the x center coordinate of this item
     */
    virtual double getCenterX() const;

    /**
     * @brief Returns the y center coordinate of this item
     */
    virtual double getCenterY() const;

    /**
     * @brief Returns the z center coordinate of this item
     */
    virtual double getCenterZ() const;

    inline const Eigen::Vector3d& getCenterCoordinate() const {return _centerCoordinate;}

    /**
     * @brief Returns true if this item has a bounding box. By default returns false.
     *        Overload this method in your item if you want to return a bounding box.
     */
    virtual bool hasBoundingBox() const {return false;}

    /**
     * @brief Returns the bounding box of this item. Use method "hasBoundingBox" to konw if this item has a bounding box.
     *        Overload this method in your item if you want to return a bounding box.
     *
     * @param min : min point to be modified
     * @param max : max point to be modified
     */
    virtual void getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const;

    /**
     * @brief Set the draw manager that must draw the item. Typically create a static draw manager in your item class
     *        and call this method in your constructor, example :
     *
     *        setBaseDrawManager(&MY_STATIC_BASE_DRAW_MANAGER);
     */
    void setBaseDrawManager(const CT_AbstractItemDrawableDrawManager *drawManager);

    /**
     * @brief Returns the base draw manager
     */
    const CT_AbstractItemDrawableDrawManager* getBaseDrawManager() const;

    /**
     * @brief If you want to set an alternative draw manager from your step (per example) you can do that with
     *        this method. If the alternative draw manager is != NULL it will be called in method draw.
     */
    void setAlternativeDrawManager(const CT_AbstractItemDrawableDrawManager *drawManager);

    /**
     * @brief Returns the alternative draw manager
     */
    const CT_AbstractItemDrawableDrawManager* getAlternativeDrawManager() const;

    /**
     * @brief Call the method draw of the alternative draw manager if exist otherwise call the method
     *        draw of the base draw manager if exist otherwise do nothing.
     * @param view : the view where the item will be draw
     * @param painter : painter used to draw elements
     */
    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter);

    /**
     * @brief Define if this item must delete from memory its childrens. True by default.
     */
    void setAutoDelete(bool autoDelete);

    /**
     * @brief Returns the draw manager its configuration (alternative or base). NULL if no draw manager exists.
     */
    CT_ItemDrawableConfiguration* getViewConfiguration();

    /**
     * @brief Returns for all differents children contained in the sub-hierarchy the list of draw manager its configuration
     */
    virtual QList<CT_ItemDrawableConfiguration*> getDependantViewConfigurations();

    /**
     * @brief Returns a copy of the item
     * @param model : the new model that must be set to the copy
     * @param result : the new result that will contains the copy
     * @param copyModeList : deprecated
     * @return NULL if it was an error otherwise a copy of the item
     */
    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList) = 0;

    static void addNameTypeCorresp(QString type, QString name);
    static QString getNameFromType(QString type);

signals:

    /**
     * @brief Emitted when the selection state change
     */
    void selectChange(bool value, CT_AbstractItemDrawable *item = NULL);

    /**
     * @brief Emitted when the display state change
     */
    void displayChange(bool value, CT_AbstractItemDrawable *item = NULL);

protected:

    /**
     * @brief Returns true if this item must auto delete its children
     */
    bool isAutoDelete() const;

    /**
     * @brief Set the parent item
     */
    void setItemDrawableParent(const CT_AbstractItemDrawable *parent);

    /**
     * @brief Set the id, per example if you want that the copy had the same id that the original
     */
    void setId(quint64 id);

    /**
     * @brief Overloaded to cast the model to check the validity.
     */
    virtual QString internalVerifyModel(const CT_OutAbstractModel *model) const;


private:

    quint64                     _id;
    QList<DocumentInterface*>   _documentList;
    bool                        _selected;
    bool                        _displayed;
    QString                     _name;
    bool                        _autoDelete;
    CT_AbstractItemDrawable     *_parent;
    Eigen::Vector3d             _centerCoordinate;

    CT_AbstractItemDrawableDrawManager  *_baseDrawManager;
    CT_AbstractItemDrawableDrawManager  *_alternativeDrawManager;

    static quint64  NEXTID;
    static QMap<QString, QString> NAMEMAP;
    static QMutex NAMEMAP_Mutex;

    /**
     * @brief Call this method when the display state of this item change
     *
     *        A signal is emitted if value changed
     */
    void setDisplayed(bool value);

#ifdef USE_BOOST_OLD
private:

    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        int r = _color.red();
        int v = _color.green();
        int b = _color.blue();

        qreal x = getCenterX();
        qreal y = getCenterY();
        qreal z = getCenterZ();

        CT_AbstractResult *res = (CT_AbstractResult*)_result;

        ar & _id & res & r & v & b & x & y & z & _autoDelete;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        int r, v, b;
        qreal x, y, z;

        CT_AbstractResult *res;

        ar & _id;
        ar & res;

        _result = res;

        ar & r & v & b & x & y & z & _autoDelete;

        _color = QColor::fromRgb(r, v, b);
        setCenterX(x);
        setCenterY(y);
        setCenterZ(z);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(CT_AbstractItemDrawable)
BOOST_CLASS_EXPORT_KEY(CT_AbstractItemDrawable)
#else
};
#endif

#endif // CT_ABSTRACTITEMDRAWABLE_H
