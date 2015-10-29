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

#include "pb_steploadasciifile.h"

#include "ct_global/ct_context.h"

#include "ct_step/ct_stepinitializedata.h"
#include "ct_point.h"

#include <QTextStream>
#include <limits>

const QString PB_StepLoadAsciiFile::COLUMN_X           = "X";
const QString PB_StepLoadAsciiFile::COLUMN_Y           = "Y";
const QString PB_StepLoadAsciiFile::COLUMN_Z           = "Z";
const QString PB_StepLoadAsciiFile::COLUMN_INTENSITY   = "Intensity";
const QString PB_StepLoadAsciiFile::COLUMN_NONE        = "None";

PB_StepLoadAsciiFile::PB_StepLoadAsciiFile(CT_StepInitializeData &data) : CT_AbstractStepLoadFileInScene(data)
{
    _columnTab[0] = COLUMN_X;
    _columnTab[1] = COLUMN_Y;
    _columnTab[2] = COLUMN_Z;
    _columnTab[3] = COLUMN_INTENSITY;
    _columnTab[4] = COLUMN_NONE;
    _columnTab[5] = COLUMN_NONE;
    _columnTab[6] = COLUMN_NONE;

    _columnConfigDialog = NULL;
}

PB_StepLoadAsciiFile::~PB_StepLoadAsciiFile()
{
    delete _columnConfigDialog;
}

void PB_StepLoadAsciiFile::init()
{
    createColumnConfigurationDialog();

    CT_AbstractStepLoadFileInScene::init();
}

void PB_StepLoadAsciiFile::aboutToBeDeleted()
{
    delete _columnConfigDialog;
    _columnConfigDialog = NULL;
}

SettingsNodeGroup* PB_StepLoadAsciiFile::getAllSettings() const
{
    SettingsNodeGroup *root = CT_AbstractStepLoadFileInScene::getAllSettings();

    QList<SettingsNodeGroup*> groupConfigValues = _columnConfigDialog->getAllValues();

    if(!groupConfigValues.isEmpty())
    {
        SettingsNodeGroup *group = new SettingsNodeGroup("PB_StepLoadAsciiFile");
        SettingsNodeGroup *groupConfig = new SettingsNodeGroup("Column Configuration Dialog");

        while(!groupConfigValues.isEmpty())
        {
            groupConfig->addGroup(groupConfigValues.takeFirst());
        }

        group->addGroup(groupConfig);
        root->addGroup(group);
    }

    return root;
}

bool PB_StepLoadAsciiFile::setAllSettings(const SettingsNodeGroup *settings)
{
    if(!CT_AbstractStepLoadFileInScene::setAllSettings(settings))
        return false;

    QList<SettingsNodeGroup*> groups = settings->groupsByTagName("PB_StepLoadAsciiFile");

    if(!groups.isEmpty())
    {
        QList<SettingsNodeGroup*> groupsConfig = groups.first()->groupsByTagName("Column Configuration Dialog");

        if(!groupsConfig.isEmpty())
        {
            return _columnConfigDialog->setAllValues(groupsConfig.first()->groups());
        }
    }

    return false;
}

QString PB_StepLoadAsciiFile::getStepDescription() const
{
    return tr("Fichier XYZI (sans entête)");
}

CT_VirtualAbstractStep* PB_StepLoadAsciiFile::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepLoadAsciiFile(dataInit);
}

QList<FileFormat> PB_StepLoadAsciiFile::getFileExtensionAccepted() const
{
    QList<FileFormat> list;
    list << FileFormat(QStringList() << "xyz" << "asc", tr("Ascii Files"));

    return list;
}

bool PB_StepLoadAsciiFile::mustShowConfigurationDialogBeforeAdd() const
{
    return true;
}

bool PB_StepLoadAsciiFile::showConfigurationDialogBeforeAdd()
{
    return showColumnConfigurationDialog();
}

int PB_StepLoadAsciiFile::readHeaderFile(QFile &f)
{
    Q_UNUSED(f)

    return 0;
}

void PB_StepLoadAsciiFile::readDataFile(QFile &f, int offset, bool little_endian)
{
    Q_UNUSED(offset)
    Q_UNUSED(little_endian)

    // Creating a undefined size point cloud
    CT_AbstractUndefinedSizePointCloud *uspc = PS_REPOSITORY->createNewUndefinedSizePointCloud();

    // Getting the file size
    qint64 fileSize = f.size();
    qint64 currentSizeRead = 0;

    float xmin = std::numeric_limits<float>::max();
    float ymin = xmin;
    float zmin = xmin;

    float xmax = -std::numeric_limits<float>::max();
    float ymax = xmax;
    float zmax = xmax;

    QTextStream stream ( &f );
    QString     currentLine;
    QStringList wordsOfLine;

    int nLine = 0;
    double currentX, currentY, currentZ, currentIntensity;

    bool convertionToFloatSuccess;
    int maxSize = 0;

    for(int i=0; i<7; ++i)
    {
        if(_columnTab[i] != COLUMN_NONE)
            maxSize = i+1;
    }

    if(maxSize > 0)
    {
        // While we did not reached the end of file
        while( !stream.atEnd()
                && !isStopped()
                && (getErrorCode() == 0))
        {
            // Read the currentLine
            currentLine = stream.readLine();
            currentSizeRead += currentLine.size();

            if(!currentLine.isEmpty())
            {
                // Read each word separately
                wordsOfLine = currentLine.split( QRegExp("\\s+"), QString::SkipEmptyParts );

                // Checking for a valid line
                if( wordsOfLine.size() < maxSize )
                {
                    setErrorCode(1);
                    setErrorMessage(1, "Error while loading the file at line " + QString().number(nLine));
                }

                if(getErrorCode() == 0)
                {
                    currentX = 0;
                    currentY = 0;
                    currentZ = 0;
                    currentIntensity = 0;

                    // Reads the attributes of the point
                    double *outputDouble;

                    for(int i=0; i<7; ++i)
                    {
                        outputDouble = NULL;

                        if(_columnTab[i] == COLUMN_X)
                            outputDouble = &currentX;
                        else if(_columnTab[i] == COLUMN_Y)
                            outputDouble = &currentY;
                        else if(_columnTab[i] == COLUMN_Z)
                            outputDouble = &currentZ;
                        else if(_columnTab[i] == COLUMN_INTENSITY)
                            outputDouble = &currentIntensity;

                        if(outputDouble != NULL)
                            readDouble( *outputDouble, wordsOfLine, i, convertionToFloatSuccess, QString(QString("Unable to convert the ascii format to a floatting point at line ").append( QString().number(nLine))) );
                    }

                    // Checks for min and max
                    if ( currentX < xmin )
                        xmin = currentX;

                    if ( currentY < ymin )
                        ymin = currentY;

                    if ( currentZ < zmin )
                        zmin = currentZ;

                    if ( currentX > xmax )
                        xmax = currentX;

                    if ( currentY > ymax )
                        ymax = currentY;

                    if ( currentZ > zmax )
                        zmax = currentZ;

                    // Add this point to the point cloud
                    uspc->addPoint( createCtPoint(currentX, currentY, currentZ) );

                    // Progress bar
                    setProgress( currentSizeRead*100.0/fileSize );
                }
            }

            ++nLine;
        }
    }

    createOutResult(PS_REPOSITORY->registerUndefinedSizePointCloud(uspc), xmin, xmax, ymin, ymax, zmin, zmax);
}

void PB_StepLoadAsciiFile::readDouble(double &outputDouble, QStringList &stringList, int nWord, bool &success, const QString &msgError)
{
    outputDouble = stringList[nWord].toDouble(&success);

    if(!success)
    {
        setErrorCode(2);
        setErrorMessage(2, msgError);
    }
}

bool PB_StepLoadAsciiFile::showColumnConfigurationDialog()
{
    if(_columnConfigDialog != NULL)
    {
        if(_columnConfigDialog->exec() == 1)
        {
            setSettingsModified(_columnConfigDialog->isSettingsModified());

            return true;
        }

        return false;
    }

    return true;
}

void PB_StepLoadAsciiFile::createColumnConfigurationDialog()
{
    if(_columnConfigDialog == NULL)
    {
        _columnConfigDialog = new CT_StepConfigurableDialog();
        _columnConfigDialog->setStep(*this);

        QStringList categories;
        categories.append(COLUMN_X);
        categories.append(COLUMN_Y);
        categories.append(COLUMN_Z);
        categories.append(COLUMN_INTENSITY);
        categories.append(COLUMN_NONE);

        for(int i=0; i<7; ++i)
            _columnConfigDialog->addStringChoice(tr("Colonne %1").arg(i+1), "", categories, _columnTab[i]);
    }
}

bool PB_StepLoadAsciiFile::preConfigure()
{
    bool val = CT_AbstractStepLoadFileInScene::preConfigure();

    if(val)
        return showColumnConfigurationDialog();

    return val;
}
