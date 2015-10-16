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

#include "ct_abstractsteploadfile.h"

#include "ct_stepinitializedata.h"

#include <QFileDialog>
#include <QDir>
#include <math.h>

CT_AbstractStepLoadFile::CT_AbstractStepLoadFile(CT_StepInitializeData &dataInit) : CT_AbstractStepSerializable(dataInit)
{
    _filePath = "";
    _defaultDirPath = ".";
}

CT_AbstractStepLoadFile::~CT_AbstractStepLoadFile()
{
}

void CT_AbstractStepLoadFile::init()
{
    QSettings *settings = _data->getSettingsFile();

    if(settings != NULL)
    {
        settings->beginGroup(getStepName());

        _defaultDirPath = settings->value("defaultDirPath", ".").toString();

        settings->endGroup();
    }

    CT_VirtualAbstractStep::init();
}

SettingsNodeGroup* CT_AbstractStepLoadFile::getAllSettings() const
{
    SettingsNodeGroup *root = CT_AbstractStepSerializable::getAllSettings();

    SettingsNodeGroup *group = new SettingsNodeGroup("CT_AbstractStepLoadFile");
    SettingsNodeValue *value = new SettingsNodeValue("FilePath", _filePath);
    group->addValue(value);

    root->addGroup(group);

    return root;
}

bool CT_AbstractStepLoadFile::setAllSettings(const SettingsNodeGroup *settings)
{
    if(!CT_AbstractStepSerializable::setAllSettings(settings))
        return false;

    QList<SettingsNodeGroup*> listG = settings->groupsByTagName("CT_AbstractStepLoadFile");

    if(!listG.isEmpty())
    {
        QList<SettingsNodeValue*> listV = listG.first()->valuesByTagName("FilePath");

        if(!listV.isEmpty())
        {
            QString filePath = listV.first()->value().toString();
            setFilePath(filePath);

            return true;
        }
    }

    return false;
}

bool CT_AbstractStepLoadFile::acceptFile(QString filePath, bool *allAccepted) const
{
    QList<FileFormat> extList = getFileExtensionAccepted();
    QListIterator<FileFormat> it(extList);

    QFileInfo info(filePath);
    QString suffixToTest = info.suffix().toLower();

    bool allAcceptedTmp = false;
    if (allAccepted != NULL) {*allAccepted = false;}

    while(it.hasNext())
    {
        FileFormat ff = it.next();

        QListIterator<QString> itS(ff.suffixes());

        while(itS.hasNext()) {

            QString ext = itS.next().toLower();

            if (ext == "*") {allAcceptedTmp = true;}

            if(suffixToTest == ext)
                return true;
        }
    }

    if (allAccepted != NULL) {*allAccepted = allAcceptedTmp;}
    return allAcceptedTmp;

    return false;
}

bool CT_AbstractStepLoadFile::setFilePath(QString filePath)
{
    if(acceptFile(filePath))
    {
        _filePath = filePath;

        setSettingsModified(true);

        return true;
    }

    return false;
}

QString CT_AbstractStepLoadFile::getFilePath() const
{
    return _filePath;
}

QString CT_AbstractStepLoadFile::getDefaultDirPath() const
{
    return _defaultDirPath;
}

//////////// PROTECTED /////////

void CT_AbstractStepLoadFile::setDefaultDirPath(const QString &path)
{
    _defaultDirPath = path;

    QSettings *settings = _data->getSettingsFile();

    if(settings != NULL)
    {
        settings->beginGroup(getStepName());

        settings->setValue("defaultDirPath", _defaultDirPath);

        settings->endGroup();
    }
}

QString CT_AbstractStepLoadFile::createAcceptedExtensionString(const QString &preString) const
{
    QString tmp;

    QList<FileFormat> ext = getFileExtensionAccepted();
    QListIterator<FileFormat> it(ext);

    while(it.hasNext()) {
        FileFormat ff = it.next();

        QListIterator<QString> itS(ff.suffixes());

        while(itS.hasNext())
            tmp += preString + "." + itS.next();
    }

    return tmp;
}

double CT_AbstractStepLoadFile::getDouble(char *data, bool little_endian)
{
    qint64 result = 0;

    if(little_endian)
    {
        for(int i=7; i>=0; --i)
        {
            result = (result << 8) | ((quint8)data[i]);
        }
    }
    else
    {
        for(int i=0; i<8; ++i)
        {
            result = (result << 8) | ((quint8)data[i]);
        }
    }

    // conversion long vers double (tire de la doc de java methode longBitsToDouble).
    int s = ((result >> 63) == 0) ? 1 : -1;

#ifndef _MSC_VER
    int e = (int)((result >> 52) & 0x7ffLLU);
    qint64 m = (e == 0) ?
                     (result & 0xfffffffffffffLLU) << 1 :
                     (result & 0xfffffffffffffLLU) | 0x10000000000000LLU;
#else
    int e = (int)((result >> 52) & 0x7ffULL);
    qint64 m = (e == 0) ?
                     (result & 0xfffffffffffffULL) << 1 :
                     (result & 0xfffffffffffffULL) | 0x10000000000000ULL;
#endif

    return pow(2.0, e-1075) * m * s;
}

bool CT_AbstractStepLoadFile::postConfigure()
{
    if(!isRunning() && CT_AbstractStepSerializable::postConfigure())
    {
        QString s = QFileDialog::getOpenFileName(0, getStepDescription(), getDefaultDirPath(), tr("%1 compatible file (%2);;All Files (*.*)").arg(getStepDisplayableName()).arg(createAcceptedExtensionString(" *")));

        if(!s.isEmpty())
        {
            if(getFilePath() != s)
            {
                QFileInfo info(s);

                setDefaultDirPath(info.dir().path());
                setFilePath(s);
            }

            return true;
        }
    }

    return false;
}
