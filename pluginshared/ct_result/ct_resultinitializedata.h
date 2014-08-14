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



#ifndef CT_RESULTINITIALIZEDATA_H
#define CT_RESULTINITIALIZEDATA_H

#include "pluginShared_global.h"

#include <QString>

#ifdef USE_BOOST_OLD
#include "serialization.h"
#endif

class PLUGINSHAREDSHARED_EXPORT CT_ResultInitializeData
{
public:
    CT_ResultInitializeData();
    CT_ResultInitializeData(int id);
    CT_ResultInitializeData(const CT_ResultInitializeData &data);
    virtual ~CT_ResultInitializeData();

    inline int getId() const { return _id; }

    void setId(int id);

private:

    int                 _id;

#ifdef USE_BOOST_OLD

    /*!
     *  \brief Serialisation / De-Serialisation
     */
    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & _id;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & _id;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_EXPORT_KEY(CT_ResultInitializeData)

#else
};
#endif

#endif // CT_RESULTINITIALIZEDATA_H
