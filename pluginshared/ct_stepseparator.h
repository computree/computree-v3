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
    CT_StepSeparator(const QString &title = "");
    virtual ~CT_StepSeparator();

    virtual bool addStep(CT_VirtualAbstractStep *step);

    QString getTitle() const;
    QList<CT_VirtualAbstractStep*> getStepList() const;

protected:

    void addStep(CT_VirtualAbstractStep &step);

private:

    QList<CT_VirtualAbstractStep*>    _stepList;
    QString         _title;
};

#endif // CT_STEPSEPARATOR_H
