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



#ifndef CT_STEPLOADFILESEPARATOR_H
#define CT_STEPLOADFILESEPARATOR_H

#include "interfaces.h"
#include "pluginShared_global.h"

class CT_AbstractStepLoadFile;

class PLUGINSHAREDSHARED_EXPORT CT_StepLoadFileSeparator
{
public:
    CT_StepLoadFileSeparator(const QString &typeOfFile);
    virtual ~CT_StepLoadFileSeparator();

    virtual bool addStep(CT_AbstractStepLoadFile *step);

    QString typeOfFile() const;
    QList<CT_AbstractStepLoadFile*> getStepList() const;

protected:

    friend class CT_AbstractStepPlugin;

    CT_StepsMenu    *m_menuOfSteps;

private:
    QString         _typeOfFile;
};

#endif // CT_STEPLOADFILESEPARATOR_H
