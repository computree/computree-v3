/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre P

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


#ifndef PB_STEPPLUGINMANAGER_H
#define PB_STEPPLUGINMANAGER_H

#include "ct_abstractstepplugin.h"
#include "ct_log/ct_fileloglistener.h"

#ifdef USE_GDAL
#include "gdal.h"
#endif

class PB_StepPluginManager : public CT_AbstractStepPlugin
{
public:
    PB_StepPluginManager();
    ~PB_StepPluginManager();

    QString getPluginURL() const {return QString("http://rdinnovation.onf.fr/projects/plugin-base/wiki");}

    bool init();

protected:

    QSettings* initQSettings();

    bool loadGenericsStep();
    bool loadOpenFileStep();
    bool loadCanBeAddedFirstStep();
    bool loadActions();
    bool loadExporters();
    bool loadReaders();
    bool loadFilters();
    bool loadMetrics();

    bool loadAfterAllPluginsLoaded();
    void aboutToBeUnloaded();

private:
    CT_FileLogListener m_fileLog;

    #ifdef USE_GDAL
    static void staticGdalErrorHandler(CPLErr eErrClass, int err_no, const char *msg);
    #endif
};

#endif // PB_STEPPLUGINMANAGER_H
