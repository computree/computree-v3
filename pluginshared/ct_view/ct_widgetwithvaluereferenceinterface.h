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



#ifndef CT_WIDGETWITHVALUEREFERENCEINTERFACE_H
#define CT_WIDGETWITHVALUEREFERENCEINTERFACE_H

#include "pluginShared_global.h"

class SettingsNodeGroup;

#include <QWidget>
#include <QVariant>

class PLUGINSHAREDSHARED_EXPORT CT_WidgetWithValueReferenceInterface : public QObject
{
    Q_OBJECT
public:    

    virtual ~CT_WidgetWithValueReferenceInterface() {}
    virtual QString type() const = 0;
    virtual QList<SettingsNodeGroup*> getAllValues() const = 0;
    virtual bool setAllValues(const QList<SettingsNodeGroup*> &list) = 0;

    virtual QWidget* createWidget(QWidget &parent) = 0;

    virtual void updateValue() = 0;

    virtual bool isValueAndWidgetValueDifferent() const = 0;

    virtual QVariant getValue() const = 0;

    virtual bool setWidgetValue(QVariant val) = 0;
};

#endif // CT_WIDGETWITHVALUEREFERENCEINTERFACE_H
