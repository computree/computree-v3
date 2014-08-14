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

#ifndef CT_ABSTRACTSTEPSERIALIZABLE_H
#define CT_ABSTRACTSTEPSERIALIZABLE_H

#include "ct_virtualabstractstep.h"

/**
 * @brief Represent a step that the serialization process can begin (must be step that can be added to the root of the tree !)
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractStepSerializable : public CT_VirtualAbstractStep
{
    Q_OBJECT

public:
    CT_AbstractStepSerializable(CT_StepInitializeData &dataInit);
    virtual ~CT_AbstractStepSerializable();

    /**
     * @brief Call this method to begin the serialization process.
     * @param dirPath : the path of the directory where store files
     * @param clearResultAfter : true if you want that step delete results from memory after there was serialized
     * @return true if serialization is a success
     */
    virtual bool serialize(const QString &dirPath, bool clearResultAfter);

    /**
     * @brief Call this method to know if we can deserialize elements from a directory
     * @param dirPath : the path of the directory where contains serialized files
     * @return true if files is ok
     */
    virtual bool canBeDeserialized(const QString &dirPath) const;

    /**
     * @brief Deserialize files contained in the directory passed in parameter
     * @param dirPath : the path of the directory where contains serialized files
     * @return true if deserialization process is a success
     */
    virtual bool deserialize(const QString &dirPath);

protected:

    /**
     * @brief Create INPUT models : Not need input models ! because we can be added to the root of the tree
     * @overload Overloaded from CT_VirtualAbstractStep to create INPUT models
     */
    void createInResultModelListProtected();

private:

#ifdef USE_BOOST_OLD
    template <class Archive>
    bool writeResult(QString dirPath, bool clearResultAfter);
#endif
};

#endif // CT_ABSTRACTSTEPSERIALIZABLE_H
