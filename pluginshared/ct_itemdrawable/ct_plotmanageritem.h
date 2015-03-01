/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#ifndef CT_PLOTMANAGERITEM_H
#define CT_PLOTMANAGERITEM_H

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_plotmanager/abstract/ct_abstractplotmanager.h"

/**
  * Représente une liste de readers au même format
  */
class PLUGINSHAREDSHARED_EXPORT CT_PlotManagerItem : public CT_AbstractSingularItemDrawable
{
    // IMPORTANT pour avoir le nom de l'ItemDrawable
    Q_OBJECT

public:
    /**
      * \brief Contructeur vide
      */
    CT_PlotManagerItem();
    /**
      * \brief Contructeur
      */
    CT_PlotManagerItem(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result, CT_AbstractPlotManager* plotManager);

    CT_PlotManagerItem(const QString &modelName,
                     const CT_AbstractResult *result, CT_AbstractPlotManager *plotManager);

    virtual ~CT_PlotManagerItem();

    /**
      * ATTENTION : ne pas oublier de redfinir ces deux mthodes si vous hrit de cette classe.
      */
    virtual QString getType() const;
    static QString staticGetType();

    virtual bool hasBoundingBox() const {return false;}

    inline size_t getPlotNumber() const {return (_plotManager == NULL)?0:_plotManager->getPlotNumber();}
    inline CT_AbstractPlotManager* getPlotManager() const {return _plotManager;}

    virtual CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);


protected:
    CT_AbstractPlotManager*     _plotManager;

private:
    CT_DEFAULT_IA_BEGIN(CT_PlotManagerItem)
    CT_DEFAULT_IA_V2(CT_PlotManagerItem, CT_AbstractCategory::staticInitDataNumber(), &CT_PlotManagerItem::getPlotNumber, QObject::tr("Nombre de placettes"))
    CT_DEFAULT_IA_END(CT_PlotManagerItem)

};

#endif // CT_PLOTMANAGERITEM_H
