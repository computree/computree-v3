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

#ifndef CT_ABSTRACTSTEPLOADFILE_H
#define CT_ABSTRACTSTEPLOADFILE_H

#include "ct_abstractstepserializable.h"

/**
 * @brief Represent a step that can must be added to the root of the tree and must load file into results.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractStepLoadFile : public CT_AbstractStepSerializable
{
    Q_OBJECT

public:
    CT_AbstractStepLoadFile(CT_StepInitializeData &dataInit);
    virtual ~CT_AbstractStepLoadFile();

    /**
     * @brief If settings (QSettings) exist in CT_StepInitializeData this step will get the last
     *        used path to set it in the open file dialog
     * @overload Overloaded from CT_AbstractStepSerializable
     */
    virtual void init();

    /**
     * @brief Backup settings (per example the file path)
     * @return The SettingsNodeGroup to save
     * @overload Overloaded from CT_AbstractStepSerializable
     */
    virtual SettingsNodeGroup* getAllSettings() const;

    /**
     * @brief Restore settings (per example the file path)
     * @return False if it was a problem in settings
     * @overload Overloaded from CT_AbstractStepSerializable
     */
    virtual bool setAllSettings(const SettingsNodeGroup *settings);

    /**
     * @brief Returns true if the filepath passed in parameter is accepted
     * @param filePath : path to the file
     */
    virtual bool acceptFile(QString filePath, bool *allAccepted = 0) const;

    /**
     * @brief Returns a list of extension accepted like this ".XXX" << ".YYY" << etc...
     */
    virtual QList<QString> getFileExtensionAccepted() const = 0;

    /**
     * @brief Set the path to the file to read
     * @return true if this file can be read
     */
    virtual bool setFilePath(QString filePath);

    /**
     * @brief Returns the path to the file that must be or was readed
     */
    QString getFilePath() const;

    /**
     * @brief Returns the path to the last directory used in open file dialog
     */
    QString getDefaultDirPath() const;

protected:

    /**
     * @brief Set the path to the directory to use when the next open file dialog will be displayed
     */
    void setDefaultDirPath(const QString &path);

    /**
     * @brief Append all extensions in one string
     * @param preString : pre string to add to all extension. Typically " *".
     * @return the results string. If preString == " *" and extensions are ".xxx" and ".yyy" the results will be " *.xxx *.yyy".
     */
    virtual QString createAcceptedExtensionString(const QString &preString) const;

    /**
     * @brief Convert a string to a double
     * @param data : the string to convert
     * @param little_endian : true if data is in littleEndian
     * @return data converted to double
     */
    static double getDouble(char *data, bool little_endian = false);

private:
    QString     _defaultDirPath;
    QString     _filePath;

protected:
    /**
     * @brief Show the open file dialog and set file path if user accept the dialog
     * @return true if the file path is changed
     * @overload Overloaded from CT_AbstractStepSerializable
     */
    virtual bool postConfigure();
};

#endif // CT_ABSTRACTSTEPLOADFILE_H
