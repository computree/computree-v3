/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

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



#ifndef CT_STEPSEPARATOR_H
#define CT_STEPSEPARATOR_H

#include "interfaces.h"
#include "pluginShared_global.h"

class CT_VirtualAbstractStep;

class PLUGINSHAREDSHARED_EXPORT CT_StepSeparator
{
public:
    enum LevelOperationType {
        LO_None,
        LO_Load,
        LO_Export,
        LO_Import,
        LO_Create,
        LO_Add,
        LO_Remove,
        LO_Extract,
        LO_Filter,
        LO_Separate,
        LO_Reduce,
        LO_Transform,
        LO_Compare,
        LO_Triangulate,
        LO_Color,
        LO_ExecutionFlow,
        LO_Other
    };

    enum LevelElementType {
        LE_Custom = 0,
        LE_Beam = 1,
        LE_Box = 2,
        LE_Circle = 4,
        LE_Cylinder = 8,
        LE_DataSource = 16,
        LE_EdgeAttributes = 32,
        LE_Ellipse = 64,
        LE_FaceAttributes = 128,
        LE_Grid = 256,
        LE_ImageOrRaster = 512,
        LE_Line = 1024,
        LE_Mesh = 2048,
        LE_Spline = 4096,
        LE_Profile = 8192,
        LE_PointOrPosition = 16384,
        LE_PointAttributes = 32768,
        LE_Polygon = 65536,
        LE_Scanner = 131072,
        LE_PointCloud = 262144,
        LE_Sphere = 524288,
        LE_Cluster = 1048576,
        LE_GenericItem = 2097152,
        LE_Other = 4194304
    };

    Q_DECLARE_FLAGS(LevelElementsType, LevelElementType)

    CT_StepSeparator(const QString &customMenu = "");
    CT_StepSeparator(LevelOperationType levelOperation, const QString &levelElementCustom = "");
    CT_StepSeparator(LevelOperationType levelOperation, LevelElementsType levelElement);
    CT_StepSeparator(LevelOperationType levelOperation, LevelElementsType levelElement, const QString &levelElementCustomTitle);
    virtual ~CT_StepSeparator();

    virtual bool addStep(CT_VirtualAbstractStep *step);

    LevelOperationType getLevelOperation() const;
    LevelElementsType getLevelElement() const;
    QString getLevelOperationToString() const;
    QString getLevelElementToString() const;
    QString getTitle() const;
    QList<CT_VirtualAbstractStep*> getStepList() const;

protected:

    void addStep(CT_VirtualAbstractStep &step);

private:

    QList<CT_VirtualAbstractStep*>      _stepList;
    QString                             m_levelCustom;
    LevelOperationType                  m_levelOperation;
    LevelElementsType                   m_levelElement;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CT_StepSeparator::LevelElementsType)

typedef CT_StepSeparator CT_SS;

#endif // CT_STEPSEPARATOR_H
