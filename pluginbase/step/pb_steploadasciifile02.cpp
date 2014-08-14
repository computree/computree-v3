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

#include "pb_steploadasciifile02.h"

#include "ct_step/ct_stepinitializedata.h"

#include "ct_view/tools/ct_textfileconfigurationdialog.h"

#include "ct_global/ct_context.h"

#include "ct_itemdrawable/model/outModel/ct_outstandardgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstandarditemdrawablemodel.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_itemdrawable/ct_pointsattributesscalartemplated.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"
#include "ct_itemdrawable/ct_pointsattributesnormal.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"

#include "ct_normal.h"

#include <QTextStream>
#include <limits>

#include "qdebug.h"

#define DEF_SearchResult    "r2"
#define DEF_SearchGroup     "SG2"
#define DEF_SearchPAI       "pai"
#define DEF_SearchPARGB     "pargb"
#define DEF_SearchPNORM     "norm"

PB_StepLoadAsciiFile02::PB_StepLoadAsciiFile02(CT_StepInitializeData &data) : CT_AbstractStepLoadFileInScene(data)
{
    _linesToSkip = 0;
    _separator = ";";
    _localeName = QLocale(QLocale::English, QLocale::UnitedKingdom).name();
    _columnX = 0;
    _columnY = 1;
    _columnZ = 2;
    _columnI = 3;
    _columnR = 4;
    _columnG = 5;
    _columnB = 6;
    _columnNX = 7;
    _columnNY = 8;
    _columnNZ = 9;

    _firstCall = true;
}

void PB_StepLoadAsciiFile02::init()
{
    CT_AbstractStepLoadFileInScene::init();
}

SettingsNodeGroup* PB_StepLoadAsciiFile02::getAllSettings() const
{
    SettingsNodeGroup *root = CT_AbstractStepLoadFileInScene::getAllSettings();
    SettingsNodeGroup *group = new SettingsNodeGroup("PB_StepLoadAsciiFile02");
    group->addValue(new SettingsNodeValue("Version", "1"));
    group->addValue(new SettingsNodeValue("LinesToSkip", _linesToSkip));
    group->addValue(new SettingsNodeValue("Separator", _separator));
    group->addValue(new SettingsNodeValue("LocaleName", _localeName));
    group->addValue(new SettingsNodeValue("ColumnX", _columnX));
    group->addValue(new SettingsNodeValue("ColumnY", _columnY));
    group->addValue(new SettingsNodeValue("ColumnZ", _columnZ));
    group->addValue(new SettingsNodeValue("ColumnI", _columnI));
    group->addValue(new SettingsNodeValue("ColumnR", _columnR));
    group->addValue(new SettingsNodeValue("ColumnG", _columnG));
    group->addValue(new SettingsNodeValue("ColumnB", _columnB));
    group->addValue(new SettingsNodeValue("ColumnNX", _columnNX));
    group->addValue(new SettingsNodeValue("ColumnNY", _columnNY));
    group->addValue(new SettingsNodeValue("ColumnNZ", _columnNZ));

    root->addGroup(group);
    return root;
}

bool PB_StepLoadAsciiFile02::setAllSettings(const SettingsNodeGroup *settings)
{
    if(!CT_AbstractStepLoadFileInScene::setAllSettings(settings))
        return false;


    QList<SettingsNodeGroup*> groups = settings->groupsByTagName("PB_StepLoadAsciiFile02");

    if(groups.isEmpty())
        return false;

    QList<SettingsNodeValue*> values = groups.first()->valuesByTagName("LinesToSkip");
    if(values.isEmpty()) {return false;}
    _linesToSkip = values.first()->value().toInt();

    values = groups.first()->valuesByTagName("Separator");
    if(values.isEmpty()) {return false;}
    _separator = values.first()->value().toString();

    values = groups.first()->valuesByTagName("LocaleName");
    if(values.isEmpty()) {return false;}
    _localeName = values.first()->value().toString();

    values = groups.first()->valuesByTagName("ColumnX");
    if(values.isEmpty()) {return false;}
    _columnX = values.first()->value().toInt();

    values = groups.first()->valuesByTagName("ColumnY");
    if(values.isEmpty()) {return false;}
    _columnY = values.first()->value().toInt();

    values = groups.first()->valuesByTagName("ColumnZ");
    if(values.isEmpty()) {return false;}
    _columnZ = values.first()->value().toInt();

    values = groups.first()->valuesByTagName("ColumnI");
    if(values.isEmpty()) {return false;}
    _columnI = values.first()->value().toInt();

    values = groups.first()->valuesByTagName("ColumnR");
    if(values.isEmpty()) {return false;}
    _columnR = values.first()->value().toInt();

    values = groups.first()->valuesByTagName("ColumnG");
    if(values.isEmpty()) {return false;}
    _columnG = values.first()->value().toInt();

    values = groups.first()->valuesByTagName("ColumnB");
    if(values.isEmpty()) {return false;}
    _columnB = values.first()->value().toInt();

    values = groups.first()->valuesByTagName("ColumnNX");
    if(values.isEmpty()) {return false;}
    _columnNX = values.first()->value().toInt();

    values = groups.first()->valuesByTagName("ColumnNY");
    if(values.isEmpty()) {return false;}
    _columnNY = values.first()->value().toInt();

    values = groups.first()->valuesByTagName("ColumnNZ");
    if(values.isEmpty()) {return false;}
    _columnNZ = values.first()->value().toInt();

    _firstCall = false;

    return true;
}

QString PB_StepLoadAsciiFile02::getStepDescription() const
{
    return tr("Charge un fichier ascii");
}

CT_VirtualAbstractStep* PB_StepLoadAsciiFile02::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepLoadAsciiFile02(dataInit);
}

QList<QString> PB_StepLoadAsciiFile02::getFileExtensionAccepted() const
{
    QList<QString> list;
    list << ".xyz";
    list << ".asc";
    list << ".txt";

    return list;
}

bool PB_StepLoadAsciiFile02::mustShowConfigurationDialogBeforeAdd() const
{
    return true;
}

bool PB_StepLoadAsciiFile02::showConfigurationDialogBeforeAdd()
{
    return showColumnConfigurationDialog();
}


bool PB_StepLoadAsciiFile02::preConfigure()
{
    //bool val = CT_AbstractStepLoadFileInScene::preConfigure();

    //if(val)
    return showColumnConfigurationDialog();

    //return val;
}

bool PB_StepLoadAsciiFile02::postConfigure()
{
    //bool val = CT_AbstractStepLoadFileInScene::postConfigure();

    //if(val)
    return showColumnConfigurationDialog();

    //return val;
}


bool PB_StepLoadAsciiFile02::showColumnConfigurationDialog()
{
    // première boite de dialog
    QStringList fieldList;
    fieldList.append("X");
    fieldList.append("Y");
    fieldList.append("Z");
    fieldList.append("Intensité");
    fieldList.append("Rouge");
    fieldList.append("Vert");
    fieldList.append("Bleu");
    fieldList.append("Normale X");
    fieldList.append("Normale Y");
    fieldList.append("Normale Z");

    CT_TextFileConfigurationDialog dialog(fieldList, NULL, getFilePath());
    dialog.setFileExtensionAccepted(getFileExtensionAccepted());

    QMap<QString, int> corresp;

    if(!_firstCall)
    {
        corresp.insert("X", _columnX);
        corresp.insert("Y", _columnY);
        corresp.insert("Z", _columnZ);
        corresp.insert("Intensité", _columnI);
        corresp.insert("Rouge", _columnR);
        corresp.insert("Vert", _columnG);
        corresp.insert("Bleu", _columnB);
        corresp.insert("Normale X", _columnNX);
        corresp.insert("Normale Y", _columnNY);
        corresp.insert("Normale Z", _columnNZ);

        dialog.setNLinesToSkip(_linesToSkip);
        dialog.setSeparator(_separator);
        dialog.setQLocale(_localeName);
        dialog.setFieldColumnsSelected(corresp);
    }

    _firstCall = false;

    if (!dialog.exec()==QDialog::Accepted) {return false;}

    corresp = dialog.getNeededFieldColumns();

    int columnX;
    int columnY;
    int columnZ;
    int columnI;
    int columnR;
    int columnG;
    int columnB;
    int columnNX;
    int columnNY;
    int columnNZ;

    bool ok = true;
    if (corresp.contains("X")) {columnX = corresp.value("X");} else {ok = false;}
    if (corresp.contains("Y")) {columnY = corresp.value("Y");} else {ok = false;}
    if (corresp.contains("Z")) {columnZ = corresp.value("Z");} else {ok = false;}
    columnI = corresp.value("Intensité", -1);
    columnR = corresp.value("Rouge", -1);
    columnG = corresp.value("Vert", -1);
    columnB = corresp.value("Bleu", -1);
    columnNX = corresp.value("Normale X", -1);
    columnNY = corresp.value("Normale Y", -1);
    columnNZ = corresp.value("Normale Z", -1);

    if (columnX<0 || columnY<0 ||columnZ<0) {ok = false;}

    if (!ok || !setFilePath(dialog.getFileNameWithPath())) {return false;}

    _columnX = columnX;
    _columnY = columnY;
    _columnZ = columnZ;
    _columnI = columnI;
    _columnR = columnR;
    _columnG = columnG;
    _columnB = columnB;
    _columnNX = columnNX;
    _columnNY = columnNY;
    _columnNZ = columnNZ;

    _linesToSkip = dialog.getNlinesToSkip();
    if (dialog.hasHeader()) {_linesToSkip++;}
    _separator = dialog.getSeparator();
    _localeName = dialog.getQLocaleName();

    setSettingsModified(true);

    return true;
}

QList<CT_OutAbstractItemDrawableModel *> PB_StepLoadAsciiFile02::createOtherOutResultListProtected(const QString &sceneModelName)
{
    QList<CT_OutAbstractItemDrawableModel *> root;

    if (_columnI > 0)
    {
        CT_OutStdSingularItemModel *pai = new CT_OutStdSingularItemModel(DEF_SearchPAI, new CT_PointsAttributesScalarTemplated<float>(), "Intensity");
        root.append(pai);
    }

    if((_columnR > 0)
            || (_columnG > 0)
            || (_columnB > 0))
    {
        CT_OutStdSingularItemModel *pargb = new CT_OutStdSingularItemModel(DEF_SearchPARGB, new CT_PointsAttributesColor(), "Color");
        root.append(pargb);
    }

    if((_columnNX > 0)
            || (_columnNY > 0)
            || (_columnNZ > 0))
    {
        CT_OutStdSingularItemModel *pnorm = new CT_OutStdSingularItemModel(DEF_SearchPNORM, new CT_PointsAttributesNormal(), "Normale");
        root.append(pnorm);
    }

    return root;
}

int PB_StepLoadAsciiFile02::readHeaderFile(QFile &f)
{
    Q_UNUSED(f)

    return 0;
}

void PB_StepLoadAsciiFile02::readDataFile(QFile &f, int offset, bool little_endian)
{
    Q_UNUSED(offset)
    Q_UNUSED(little_endian)

    // Creating a undefined size point cloud
    CT_AbstractUndefinedSizePointCloud *uspc = PS_REPOSITORY->createNewUndefinedSizePointCloud();

    CT_StandardCloudStdVectorT<float> *collection = NULL;

    if (_columnI > 0)
    {
        collection = new CT_StandardCloudStdVectorT<float>();
    }

    // Getting the file size
    qint64 fileSize = f.size();
    qint64 currentSizeRead = 0;

    CT_ColorCloudStdVector *colorCloud = NULL;

    if((_columnR > 0)
            || (_columnG > 0)
            || (_columnB > 0))
    {
        colorCloud = new CT_ColorCloudStdVector(false);
    }

    CT_NormalCloudStdVector *normalCloud = NULL;

    if((_columnNX > 0)
            || (_columnNY > 0)
            || (_columnNZ > 0))
    {
        normalCloud = new CT_NormalCloudStdVector();
    }

    qDebug() << "Normal cloud = null ? " << (normalCloud == NULL);
    qDebug() << "Colonne x y z " << _columnNX << _columnNY << _columnNZ;

    float xmin = std::numeric_limits<float>::max();
    float ymin = xmin;
    float zmin = xmin;
    float imin = xmin;

    float xmax = -std::numeric_limits<float>::max();
    float ymax = xmax;
    float zmax = xmax;
    float imax = xmax;

    QTextStream stream ( &f );
    QString     currentLine;
    QStringList wordsOfLine;

    int nPoints = 0;
    int nLine = 0;
    float currentX, currentY, currentZ, currentIntensity;
    CT_Normal currentNormal;
    CT_Color currentRGB;

    int maxSize = 3;
    if (_columnX > maxSize - 1) {maxSize = _columnX + 1;}
    if (_columnY > maxSize - 1) {maxSize = _columnY + 1;}
    if (_columnZ > maxSize - 1) {maxSize = _columnZ + 1;}
    if (_columnI > maxSize - 1) {maxSize = _columnI + 1;}
    if (_columnR > maxSize - 1) {maxSize = _columnR + 1;}
    if (_columnG > maxSize - 1) {maxSize = _columnG + 1;}
    if (_columnB > maxSize - 1) {maxSize = _columnB + 1;}
    if (_columnNX > maxSize - 1) {maxSize = _columnNX + 1;}
    if (_columnNY > maxSize - 1) {maxSize = _columnNY + 1;}
    if (_columnNZ > maxSize - 1) {maxSize = _columnNZ + 1;}


    for (int i = 0 ; i < _linesToSkip ; i++)
    {
        stream.readLine();
    }

    QLocale locale(_localeName);
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
            wordsOfLine = currentLine.split(_separator, QString::SkipEmptyParts );

            // Checking for a valid line
            if( wordsOfLine.size() < maxSize )
            {
                setErrorCode(1);
                setErrorMessage(1, "Error while loading the file at line " + QString().number(nLine));
                qDebug() << "Error while loading the file at line " << QString().number(nLine);
            }

            if(getErrorCode() == 0)
            {
                bool okx = true, oky = true, okz = true, oki = true;
                bool okr = true, okg = true, okb = true, oknx = true, okny = true, oknz = true;
                currentX = locale.toFloat(wordsOfLine.at(_columnX), &okx);
                currentY = locale.toFloat(wordsOfLine.at(_columnY), &oky);
                currentZ = locale.toFloat(wordsOfLine.at(_columnZ), &okz);

                if(collection != NULL)
                {
                    if(_columnI<0)
                    {
                        currentIntensity = 0;
                        oki = true;
                    }
                    else
                    {
                        currentIntensity = locale.toFloat(wordsOfLine.at(_columnI), &oki);
                    }
                }

                if(colorCloud != NULL)
                {
                    if(_columnR<0)
                    {
                        currentRGB.r = 0;
                        okr = true;
                    }
                    else
                    {
                        currentRGB.r = locale.toFloat(wordsOfLine.at(_columnR), &okr);
                    }

                    if(_columnG<0)
                    {
                        currentRGB.g = 0;
                        okg = true;
                    }
                    else
                    {
                        currentRGB.g = locale.toFloat(wordsOfLine.at(_columnG), &okg);
                    }

                    if(_columnB<0)
                    {
                        currentRGB.b = 0;
                        okb = true;
                    }
                    else
                    {
                        currentRGB.b = locale.toFloat(wordsOfLine.at(_columnB), &okb);
                    }
                }

                if(normalCloud != NULL)
                {
                    if(_columnNX<0)
                    {
                        currentNormal.normal_x = 0;
                        oknx = true;
                    }
                    else
                    {
                        currentNormal.normal_x = locale.toFloat(wordsOfLine.at(_columnNX), &oknx);
                    }

                    if(_columnNY<0)
                    {
                        currentNormal.normal_y = 0;
                        okny = true;
                    }
                    else
                    {
                        currentNormal.normal_y = locale.toFloat(wordsOfLine.at(_columnNY), &okny);
                    }

                    if(_columnNZ<0)
                    {
                        currentNormal.normal_z = 0;
                        oknz = true;
                    }
                    else
                    {
                        currentNormal.normal_z = locale.toFloat(wordsOfLine.at(_columnNZ), &oknz);
                    }
//                    qDebug() << "Colonnes x y z = " << _columnNX << _columnNY << _columnNZ;
//                    qDebug() << currentLine;
//                    qDebug() << "Added " << currentNormal.normal_x << currentNormal.normal_y << currentNormal.normal_z;
                }

                if(!okx || !oky || !okz || !oki)
                {
                    qDebug() << "x y z i r g b nx ny nz" << okx << oky << okz << oki << oknx << okny << oknz;
                    qDebug() << "Ligne = " << currentLine;
                    setErrorCode(2);
                    setErrorMessage(2, QString(QString("Unable to convert the ascii format to a floatting point at line ").append( QString().number(nLine))));
                    qDebug() << QString(QString("Unable to convert the ascii format to a floatting point at line ").append( QString().number(nLine)));
                }

                // Checks for min and max
                if ( currentX < xmin )
                    xmin = currentX;

                if ( currentY < ymin )
                    ymin = currentY;

                if ( currentZ < zmin )
                    zmin = currentZ;

                if ( currentIntensity < imin )
                    imin = currentIntensity;

                if ( currentX > xmax )
                    xmax = currentX;

                if ( currentY > ymax )
                    ymax = currentY;

                if ( currentZ > zmax )
                    zmax = currentZ;

                if ( currentIntensity > imax )
                    imax = currentIntensity;

                // Add this point to the point cloud
                uspc->addPoint( createCtPoint(currentX, currentY, currentZ) );

                if (collection != NULL && oki)
                {
                    collection->addT(currentIntensity);
                }

                if(colorCloud != NULL && okr && okg && okb)
                {
                    colorCloud->addColor(currentRGB);
                }

                if(normalCloud != NULL && oknx && okny && oknz)
                {
                    normalCloud->addNormal(currentNormal);
                }

                // Progress bar
                setProgress( currentSizeRead*100.0/fileSize );

                ++nPoints;
            }
        }

        ++nLine;
    }

    setProgress(100);

    CT_ABSTRACT_PCIR pcir = PS_REPOSITORY->registerUndefinedSizePointCloud(uspc);

    CT_ResultGroup *out_res = getOutResultList().first();
    QList<CT_AbstractSingularItemDrawable*> lAdd;

    if(collection != NULL)
    {    CT_PointsAttributesScalarTemplated<float> *pas = new CT_PointsAttributesScalarTemplated<float>(getOutModelForCreation(out_res, DEF_SearchPAI),
                                                                                                        out_res,
                                                                                                        pcir,
                                                                                                        collection,
                                                                                                        imin,
                                                                                                        imax);

        lAdd << pas;
    }

    if(colorCloud != NULL)
    {
        CT_PointsAttributesColor *pac = new CT_PointsAttributesColor(getOutModelForCreation(out_res, DEF_SearchPARGB),
                                                                     out_res,
                                                                     pcir,
                                                                     colorCloud);

        lAdd << pac;
    }

    if(normalCloud != NULL)
    {
        CT_PointsAttributesNormal *pan = new CT_PointsAttributesNormal(getOutModelForCreation(out_res, DEF_SearchPNORM),
                                                                       out_res,
                                                                       pcir,
                                                                       normalCloud);

        lAdd << pan;
    }

    createOutResult(pcir, xmin, xmax, ymin, ymax, zmin, zmax, lAdd);
}

