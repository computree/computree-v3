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

#ifndef CT_STEPCANBEADDEDFIRSTSEPARATOR_H
#define CT_STEPCANBEADDEDFIRSTSEPARATOR_H

#include "interfaces.h"
#include "pluginShared_global.h"

class CT_AbstractStepCanBeAddedFirst;

class PLUGINSHAREDSHARED_EXPORT CT_StepCanBeAddedFirstSeparator
{
public:
    CT_StepCanBeAddedFirstSeparator(const QString &title = "");
    virtual ~CT_StepCanBeAddedFirstSeparator();

    virtual bool addStep(CT_AbstractStepCanBeAddedFirst *step);

    QString getTitle() const;
    QList<CT_AbstractStepCanBeAddedFirst*> getStepList() const;

protected:

    void addStep(CT_AbstractStepCanBeAddedFirst &step);

private:

    QList<CT_AbstractStepCanBeAddedFirst*> _stepList;
    QString                     _title;
};

#endif // CT_STEPCANBEADDEDFIRSTSEPARATOR_H