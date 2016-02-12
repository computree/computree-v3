#include "ct_reader_points_ascii.h"

#include "ct_view/tools/ct_textfileconfigurationdialog.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointsattributesscalartemplated.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"
#include "ct_itemdrawable/ct_pointsattributesnormal.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_normalcloud/ct_normalcloudstdvector.h"
#include "ct_point.h"
#include "ct_normal.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

#define CHK_ERR(argFunc, argErrStr) if(!error && !argFunc) { error = true; PS_LOG->addErrorMessage(LogInterface::reader, argErrStr); }

#define X_COLUMN "X"
#define Y_COLUMN "Y"
#define Z_COLUMN "Z"
#define INTENSITY_COLUMN tr("Intensité")
#define RED_COLUMN tr("Rouge")
#define GREEN_COLUMN tr("Vert")
#define BLUE_COLUMN tr("Bleu")
#define NX_COLUMN tr("Normale X")
#define NY_COLUMN tr("Normale Y")
#define NZ_COLUMN tr("Normale Z")
#define NW_COLUMN tr("Normale Curvature")

CT_Reader_Points_ASCII::CT_Reader_Points_ASCII()
{
    m_firstConfiguration = true;
    m_columnXIndex = -1;
    m_columnYIndex = -1;
    m_columnZIndex = -1;
    m_columnIntensityIndex = -1;
    m_columnRedIndex = -1;
    m_columnGreenIndex = -1;
    m_columnBlueIndex = -1;
    m_columnNxIndex = -1;
    m_columnNyIndex = -1;
    m_columnNzIndex = -1;
    m_columnNCurvatureIndex = -1;
    m_nLinesToSkip = 0;
    m_hasHeader = false;
    m_separator = ".";
    m_localeName = "";
}

QString CT_Reader_Points_ASCII::GetReaderName() const
{
    return tr("Nuage de points, Fichiers ASCII (paramétrable)");
}

CT_StepsMenu::LevelPredefined CT_Reader_Points_ASCII::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}

bool CT_Reader_Points_ASCII::configure()
{
    // Columns that can be found in the ascii file
    QList<CT_TextFileConfigurationFields> fieldList;
    fieldList.append(CT_TextFileConfigurationFields(X_COLUMN, QRegExp(" *[xX] *")));            // x
    fieldList.append(CT_TextFileConfigurationFields(Y_COLUMN, QRegExp(" *[yY] *")));            // y
    fieldList.append(CT_TextFileConfigurationFields(Z_COLUMN, QRegExp(" *[zZ] *")));            // z
    fieldList.append(CT_TextFileConfigurationFields(INTENSITY_COLUMN, QRegExp("[iI].*")));      // intensité - intensity - intensidad - intensität - i
    fieldList.append(CT_TextFileConfigurationFields(RED_COLUMN, QRegExp("[rR].*")));            // rouge - red - rojo - rot - r
    fieldList.append(CT_TextFileConfigurationFields(GREEN_COLUMN, QRegExp("[vVgG].*")));        // vert - green - verde - grün - v - g
    fieldList.append(CT_TextFileConfigurationFields(BLUE_COLUMN, QRegExp("[bBaA].*")));         // bleu - blue - azul - blau - b - a
    fieldList.append(CT_TextFileConfigurationFields(NX_COLUMN, QRegExp("[nN].*[xX]")));         // normale x - normal x - normali x - normalen x - nx
    fieldList.append(CT_TextFileConfigurationFields(NY_COLUMN, QRegExp("[nN].*[yY]")));         // normale y - normal y - normali y - normalen y - ny
    fieldList.append(CT_TextFileConfigurationFields(NZ_COLUMN, QRegExp("[nN].*[zZ]")));         // normale z - normal z - normali z - normalen z - nz
    fieldList.append(CT_TextFileConfigurationFields(NW_COLUMN, QRegExp("[nN].*[wW]")));         // normale w - normal w - normali w - normalen w - nw

    // a configurable dialog that help the user to select the right column and auto-detect some columns
    CT_TextFileConfigurationDialog dialog(fieldList, NULL, filepath());
    dialog.setFileExtensionAccepted(readableFormats());
    dialog.setFilePathCanBeModified(filePathCanBeModified());

    // table that link a sought column to a column in the ascii file
    QMap<QString, int> corresp;

    // if it is not the first configuration
    if(!m_firstConfiguration)
    {
        // we will set by default columns detected in last configuration
        corresp.insert(X_COLUMN, m_columnXIndex);
        corresp.insert(Y_COLUMN, m_columnYIndex);
        corresp.insert(Z_COLUMN, m_columnZIndex);
        corresp.insert(INTENSITY_COLUMN, m_columnIntensityIndex);
        corresp.insert(RED_COLUMN, m_columnRedIndex);
        corresp.insert(GREEN_COLUMN, m_columnGreenIndex);
        corresp.insert(BLUE_COLUMN, m_columnBlueIndex);
        corresp.insert(NX_COLUMN, m_columnNxIndex);
        corresp.insert(NY_COLUMN, m_columnNyIndex);
        corresp.insert(NZ_COLUMN, m_columnNzIndex);
        corresp.insert(NW_COLUMN, m_columnNCurvatureIndex);

        // and other elements like header, number of lines to skip, etc...
        dialog.setHeader(m_hasHeader);
        dialog.setNLinesToSkip(m_nLinesToSkip);
        dialog.setSeparator(m_separator);
        dialog.setQLocale(m_localeName);
        dialog.setFieldColumnsSelected(corresp);
    }

    if(dialog.exec() != QDialog::Accepted)
        return false;

    // get the link between sought columns and column in the ascii file selected by the user
    corresp = dialog.getNeededFieldColumns();

    if((corresp.value(X_COLUMN, -1) < 0)
            || (corresp.value(Y_COLUMN, -1)  < 0)
            || (corresp.value(Z_COLUMN, -1)  < 0))
        return false;

    m_firstConfiguration = false;

    int columnXIndex = corresp.value(X_COLUMN);
    int columnYIndex = corresp.value(Y_COLUMN);
    int columnZIndex = corresp.value(Z_COLUMN);
    int columnIntensityIndex = corresp.value(INTENSITY_COLUMN, -1);
    int columnRedIndex = corresp.value(RED_COLUMN, -1);
    int columnGreenIndex = corresp.value(GREEN_COLUMN, -1);
    int columnBlueIndex = corresp.value(BLUE_COLUMN, -1);
    int columnNxIndex = corresp.value(NX_COLUMN, -1);
    int columnNyIndex = corresp.value(NY_COLUMN, -1);
    int columnNzIndex = corresp.value(NZ_COLUMN, -1);
    int columnNCurvatureIndex = corresp.value(NW_COLUMN, -1);

    m_columnXIndex = columnXIndex;
    m_columnYIndex = columnYIndex;
    m_columnZIndex = columnZIndex;
    m_columnIntensityIndex = columnIntensityIndex;
    m_columnRedIndex = columnRedIndex;
    m_columnGreenIndex = columnGreenIndex;
    m_columnBlueIndex = columnBlueIndex;
    m_columnNxIndex = columnNxIndex;
    m_columnNyIndex = columnNyIndex;
    m_columnNzIndex = columnNzIndex;
    m_columnNCurvatureIndex = columnNCurvatureIndex;

    m_nLinesToSkip = dialog.getNlinesToSkip();
    m_hasHeader = dialog.hasHeader();
    m_separator = dialog.getSeparator();
    m_localeName = dialog.getQLocaleName();

    if(!filePathCanBeModified() && !filepath().isEmpty())
        return true;

    return setFilePath(dialog.getFileNameWithPath());
}

SettingsNodeGroup *CT_Reader_Points_ASCII::getAllSettings() const
{
    SettingsNodeGroup *root = CT_AbstractReader::getAllSettings();

    SettingsNodeGroup *group = new SettingsNodeGroup("CT_Reader_Points_ASCII_Settings");
    group->addValue(new SettingsNodeValue("Version", "1"));
    group->addValue(new SettingsNodeValue("HasHeader", m_hasHeader));
    group->addValue(new SettingsNodeValue("NLinesToSkip", m_nLinesToSkip));
    group->addValue(new SettingsNodeValue("Separator", m_separator));
    group->addValue(new SettingsNodeValue("LocaleName", m_localeName));
    group->addValue(new SettingsNodeValue("ColumnXIndex", m_columnXIndex));
    group->addValue(new SettingsNodeValue("ColumnYIndex", m_columnYIndex));
    group->addValue(new SettingsNodeValue("ColumnZIndex", m_columnZIndex));
    group->addValue(new SettingsNodeValue("ColumnIntensityIndex", m_columnIntensityIndex));
    group->addValue(new SettingsNodeValue("ColumnRedIndex", m_columnRedIndex));
    group->addValue(new SettingsNodeValue("ColumnGreenIndex", m_columnGreenIndex));
    group->addValue(new SettingsNodeValue("ColumnBlueIndex", m_columnBlueIndex));
    group->addValue(new SettingsNodeValue("ColumnNxIndex", m_columnNxIndex));
    group->addValue(new SettingsNodeValue("ColumnNyIndex", m_columnNyIndex));
    group->addValue(new SettingsNodeValue("ColumnNzIndex", m_columnNzIndex));
    group->addValue(new SettingsNodeValue("ColumnNCurvatureIndex", m_columnNCurvatureIndex));

    root->addGroup(group);

    return root;
}

bool CT_Reader_Points_ASCII::setAllSettings(const SettingsNodeGroup *settings)
{
    if(CT_AbstractReader::setAllSettings(settings))
    {
        QList<SettingsNodeGroup*> groups = settings->groupsByTagName("CT_Reader_Points_ASCII_Settings");

        if(groups.isEmpty())
            return false;

        QList<SettingsNodeValue*> values = groups.first()->valuesByTagName("HasHeader");
        if(values.isEmpty()) {return false;}
        m_hasHeader = values.first()->value().toBool();

        values = groups.first()->valuesByTagName("NLinesToSkip");
        if(values.isEmpty()) {return false;}
        m_nLinesToSkip = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("Separator");
        if(values.isEmpty()) {return false;}
        m_separator = values.first()->value().toString();

        values = groups.first()->valuesByTagName("LocaleName");
        if(values.isEmpty()) {return false;}
        m_localeName = values.first()->value().toString();

        values = groups.first()->valuesByTagName("ColumnXIndex");
        if(values.isEmpty()) {return false;}
        m_columnXIndex = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("ColumnYIndex");
        if(values.isEmpty()) {return false;}
        m_columnYIndex = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("ColumnZIndex");
        if(values.isEmpty()) {return false;}
        m_columnZIndex = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("ColumnIntensityIndex");
        if(values.isEmpty()) {return false;}
        m_columnIntensityIndex = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("ColumnRedIndex");
        if(values.isEmpty()) {return false;}
        m_columnRedIndex = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("ColumnGreenIndex");
        if(values.isEmpty()) {return false;}
        m_columnGreenIndex = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("ColumnBlueIndex");
        if(values.isEmpty()) {return false;}
        m_columnBlueIndex = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("ColumnNxIndex");
        if(values.isEmpty()) {return false;}
        m_columnNxIndex = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("ColumnNyIndex");
        if(values.isEmpty()) {return false;}
        m_columnNyIndex = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("ColumnNzIndex");
        if(values.isEmpty()) {return false;}
        m_columnNzIndex = values.first()->value().toInt();

        values = groups.first()->valuesByTagName("ColumnNCurvatureIndex");
        if(values.isEmpty()) {return false;}
        m_columnNCurvatureIndex = values.first()->value().toInt();

        m_firstConfiguration = false;

        return true;
    }

    return false;
}

void CT_Reader_Points_ASCII::setXColumnIndex(int index)
{
    m_columnXIndex = index;
}

void CT_Reader_Points_ASCII::setYColumnIndex(int index)
{
    m_columnYIndex = index;
}

void CT_Reader_Points_ASCII::setZColumnIndex(int index)
{
    m_columnZIndex = index;
}

void CT_Reader_Points_ASCII::setIntensityColumnIndex(int index)
{
    m_columnIntensityIndex = index;
}

void CT_Reader_Points_ASCII::setRedColumnIndex(int index)
{
    m_columnRedIndex = index;
}

void CT_Reader_Points_ASCII::setGreenColumnIndex(int index)
{
    m_columnGreenIndex = index;
}

void CT_Reader_Points_ASCII::setBlueColumnIndex(int index)
{
    m_columnBlueIndex = index;
}

void CT_Reader_Points_ASCII::setNxColumnIndex(int index)
{
    m_columnNxIndex = index;
}

void CT_Reader_Points_ASCII::setNyColumnIndex(int index)
{
    m_columnNyIndex = index;
}

void CT_Reader_Points_ASCII::setNzColumnIndex(int index)
{
    m_columnNzIndex = index;
}

void CT_Reader_Points_ASCII::setNCurvatureIndex(int index)
{
    m_columnNCurvatureIndex = index;
}

void CT_Reader_Points_ASCII::setFirstConfiguration(bool first)
{
    m_firstConfiguration = first;
}

int CT_Reader_Points_ASCII::xColumnIndex() const
{
    return m_columnXIndex;
}

int CT_Reader_Points_ASCII::yColumnIndex() const
{
    return m_columnYIndex;
}

int CT_Reader_Points_ASCII::zColumnIndex() const
{
    return m_columnZIndex;
}

int CT_Reader_Points_ASCII::intensityColumnIndex() const
{
    return m_columnIntensityIndex;
}

int CT_Reader_Points_ASCII::redColumnIndex() const
{
    return m_columnRedIndex;
}

int CT_Reader_Points_ASCII::greenColumnIndex() const
{
    return m_columnGreenIndex;
}

int CT_Reader_Points_ASCII::blueColumnIndex() const
{
    return m_columnBlueIndex;
}

int CT_Reader_Points_ASCII::nXColumnIndex() const
{
    return m_columnNxIndex;
}

int CT_Reader_Points_ASCII::nYColumnIndex() const
{
    return m_columnNyIndex;
}

int CT_Reader_Points_ASCII::nZColumnIndex() const
{
    return m_columnNzIndex;
}

int CT_Reader_Points_ASCII::nCurvatureIndex() const
{
    return m_columnNCurvatureIndex;
}

bool CT_Reader_Points_ASCII::canLoadPoints() const
{
    return (m_columnXIndex >= 0) && (m_columnYIndex >= 0) && (m_columnZIndex >= 0);
}

bool CT_Reader_Points_ASCII::canLoadIntensity() const
{
    return m_columnIntensityIndex >= 0;
}

bool CT_Reader_Points_ASCII::canLoadColors() const
{
    return (m_columnRedIndex >= 0) && (m_columnGreenIndex >= 0) && (m_columnBlueIndex >= 0);
}

bool CT_Reader_Points_ASCII::canLoadNormals() const
{
    return (m_columnNxIndex >= 0) && (m_columnNyIndex >= 0) && (m_columnNzIndex >= 0);
}

CT_AbstractReader* CT_Reader_Points_ASCII::copy() const
{
    return new CT_Reader_Points_ASCII();
}

void CT_Reader_Points_ASCII::protectedInit()
{
    addNewReadableFormat(FileFormat(QStringList() << "xyz" << "asc" << "txt" << "csv", tr("Fichiers ascii")));

    setToolTip(tr("Chargement d'un fichier de points au format ASCII.<br>"
                  "L'import est configurable, le fichier devant contenir les champs suivants :<br>"
                  "- X      : Coordonnée X du points<br>"
                  "- Y      : Coordonnée Y du point<br>"
                  "- Z      : Coordonnée Y du point<br><br>"
                  "De plus les champs suivants peuvent être fournis mais sont optionnels :<br>"
                  "- Intensité      : Intensité du point<br>"
                  "- Rouge          : Composante rouge du point<br>"
                  "- Vert           : Composante verte du point<br>"
                  "- Bleu           : Composante bleue du point<br>"
                  "- Normale X      : Coordonnée Y de la normale au point<br>"
                  "- Normale Y      : Coordonnée Y de la normale au point<br>"
                  "- Normale Z      : Coordonnée Y de la normale au point<br>"));
}

void CT_Reader_Points_ASCII::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    if(canLoadPoints())
        addOutItemDrawableModel(DEF_CT_Reader_Points_ASCII_pointCloudOut, new CT_Scene());

    if(canLoadIntensity())
        addOutItemDrawableModel(DEF_CT_Reader_Points_ASCII_intensityOut, new CT_PointsAttributesScalarTemplated<float>());

    if(canLoadColors())
        addOutItemDrawableModel(DEF_CT_Reader_Points_ASCII_colorsOut, new CT_PointsAttributesColor());

    if(canLoadNormals())
        addOutItemDrawableModel(DEF_CT_Reader_Points_ASCII_normalsOut, new CT_PointsAttributesNormal());
}

bool CT_Reader_Points_ASCII::protectedReadFile()
{
    if(!canLoadPoints())
        return false;

    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);
            QString     currentLine;
            QStringList wordsOfLine;

            int nLine = 1;
            int maxIndex = maxColumnIndex();

            // Getting the file size to set progress
            qint64 fileSize = f.size();
            qint64 currentSizeRead = 0;

            // create a new point cloud that size was undefined for the moment
            CT_AbstractUndefinedSizePointCloud *uspc = createPointCloud();
            CT_StandardCloudStdVectorT<float> *intensityCloud = createIntensityArray();
            CT_ColorCloudStdVector *colorCloud = createColorsArray();
            CT_NormalCloudStdVector *normalCloud = createNormalsArray();

            Eigen::Vector3d minBB, maxBB;
            initBoundinBox(minBB, maxBB);

            float minIntensity, maxIntensity;
            minIntensity = std::numeric_limits<float>::max();
            maxIntensity = -minIntensity;

            skipLines(stream, currentSizeRead);

            QLocale locale(m_localeName);
            bool error = false;

            // While we did not reached the end of file
            while( !stream.atEnd()
                   && !isStopped()
                   && !error)
            {
                // Read the currentLine
                currentLine = stream.readLine();
                currentSizeRead += currentLine.size();

                if(!currentLine.isEmpty())
                {
                    // Read each word separately
                    wordsOfLine = currentLine.split(m_separator, QString::SkipEmptyParts );

                    // Checking for a valid line
                    if(maxIndex >= wordsOfLine.size())
                    {
                        PS_LOG->addErrorMessage(LogInterface::reader, tr("Error while loading the file at line %1. Number of columns is too less.").arg(nLine));
                        error = true;
                    }

                    CHK_ERR(readAndAddPoint(wordsOfLine, locale, uspc, minBB, maxBB), tr("Error while loading point in file at line ") + QString().number(nLine));
                    CHK_ERR(readAndAddIntensity(wordsOfLine, locale, intensityCloud, minIntensity, maxIntensity), tr("Error while intensity in file at line ") + QString().number(nLine));
                    CHK_ERR(readAndAddColor(wordsOfLine, locale, colorCloud), tr("Error while loading color in file at line ") + QString().number(nLine));
                    CHK_ERR(readAndAddNormal(wordsOfLine, locale, normalCloud), tr("Error while loading normal in file at line ") + QString().number(nLine));

                }

                ++nLine;

                setProgress((currentSizeRead*100) / fileSize);
            }

            f.close();

            CT_PCIR pcir = PS_REPOSITORY->registerUndefinedSizePointCloud(uspc);

            addOutItemDrawable(DEF_CT_Reader_Points_ASCII_pointCloudOut, new CT_Scene(NULL, NULL, pcir));

            if(intensityCloud != NULL) {
                CT_PointsAttributesScalarTemplated<float> *item = new CT_PointsAttributesScalarTemplated<float>(NULL,
                                                                                                                NULL,
                                                                                                                pcir,
                                                                                                                intensityCloud,
                                                                                                                minIntensity,
                                                                                                                maxIntensity);
                item->setBoundingBox(minBB[0], minBB[1], minBB[2], maxBB[0], maxBB[1], maxBB[2]);

                addOutItemDrawable(DEF_CT_Reader_Points_ASCII_intensityOut, item);
            }

            if(colorCloud != NULL) {
                CT_PointsAttributesColor *item = new CT_PointsAttributesColor(NULL,
                                                                              NULL,
                                                                              pcir,
                                                                              colorCloud);

                item->setBoundingBox(minBB[0], minBB[1], minBB[2], maxBB[0], maxBB[1], maxBB[2]);

                addOutItemDrawable(DEF_CT_Reader_Points_ASCII_colorsOut, item);
            }

            if(normalCloud != NULL) {
                CT_PointsAttributesNormal *item = new CT_PointsAttributesNormal(NULL,
                                                                                NULL,
                                                                                pcir,
                                                                                normalCloud);

                item->setBoundingBox(minBB[0], minBB[1], minBB[2], maxBB[0], maxBB[1], maxBB[2]);

                addOutItemDrawable(DEF_CT_Reader_Points_ASCII_normalsOut, item);
            }

            return !error;
        }
    }

    return false;
}

CT_AbstractUndefinedSizePointCloud *CT_Reader_Points_ASCII::createPointCloud() const
{
    return PS_REPOSITORY->createNewUndefinedSizePointCloud();
}

CT_StandardCloudStdVectorT<float> *CT_Reader_Points_ASCII::createIntensityArray() const
{
    if(canLoadIntensity())
        return new CT_StandardCloudStdVectorT<float>();

    return NULL;
}

CT_ColorCloudStdVector* CT_Reader_Points_ASCII::createColorsArray() const
{
    if(canLoadColors())
        return new CT_ColorCloudStdVector();

    return NULL;
}

CT_NormalCloudStdVector* CT_Reader_Points_ASCII::createNormalsArray() const
{
    if(canLoadNormals())
        return new CT_NormalCloudStdVector();

    return NULL;
}

int CT_Reader_Points_ASCII::maxColumnIndex() const
{
    int index = -1;
    index = qMax(index, m_columnXIndex);
    index = qMax(index, m_columnYIndex);
    index = qMax(index, m_columnZIndex);
    index = qMax(index, m_columnIntensityIndex);
    index = qMax(index, m_columnRedIndex);
    index = qMax(index, m_columnGreenIndex);
    index = qMax(index, m_columnBlueIndex);
    index = qMax(index, m_columnNxIndex);
    index = qMax(index, m_columnNyIndex);
    index = qMax(index, m_columnNzIndex);
    index = qMax(index, m_columnNCurvatureIndex);

    return index;
}

void CT_Reader_Points_ASCII::initBoundinBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    min[0] = std::numeric_limits<double>::max();
    min[1] = min[0];
    min[2] = min[0];

    max[0] = -min[0];
    max[1] = -min[0];
    max[2] = -min[0];
}

void CT_Reader_Points_ASCII::skipLines(QTextStream &stream, qint64 &currentSizeRead) const
{
    for(int i = 0 ; i < m_nLinesToSkip; ++i)
        currentSizeRead += stream.readLine().size();

    if (m_hasHeader)
        currentSizeRead += stream.readLine().size();
}

void CT_Reader_Points_ASCII::updateBoundingBox(const CT_Point &point, Eigen::Vector3d &bboxMin, Eigen::Vector3d &bboxMax) const
{
    bboxMin[0] = qMin(point[CT_Point::X], bboxMin[0]);
    bboxMin[1] = qMin(point[CT_Point::Y], bboxMin[1]);
    bboxMin[2] = qMin(point[CT_Point::Z], bboxMin[2]);

    bboxMax[0] = qMax(point[CT_Point::X], bboxMax[0]);
    bboxMax[1] = qMax(point[CT_Point::Y], bboxMax[1]);
    bboxMax[2] = qMax(point[CT_Point::Z], bboxMax[2]);
}

bool CT_Reader_Points_ASCII::readAndAddPoint(const QStringList &wordsOfLine, const QLocale &locale, CT_AbstractUndefinedSizePointCloud *array, Eigen::Vector3d &minBB, Eigen::Vector3d &maxBB) const
{
    bool ok;

    CT_Point point;

    point[CT_Point::X] = locale.toDouble(wordsOfLine.at(m_columnXIndex), &ok);

    if(!ok)
        return false;

    point[CT_Point::Y] = locale.toDouble(wordsOfLine.at(m_columnYIndex), &ok);

    if(!ok)
        return false;

    point[CT_Point::Z] = locale.toDouble(wordsOfLine.at(m_columnZIndex), &ok);

    if(!ok)
        return false;

    array->addPoint(point);

    updateBoundingBox(point, minBB, maxBB);

    return true;
}

bool CT_Reader_Points_ASCII::readAndAddIntensity(const QStringList &wordsOfLine, const QLocale &locale, CT_StandardCloudStdVectorT<float> *array, float &minI, float &maxI) const
{
    if(array == NULL)
        return true;

    bool ok;

    float value = locale.toFloat(wordsOfLine.at(m_columnIntensityIndex), &ok);

    if(!ok)
        return false;

    array->addT(value);

    minI = qMin(value, minI);
    maxI = qMax(value, maxI);

    return true;
}

bool CT_Reader_Points_ASCII::readAndAddColor(const QStringList &wordsOfLine, const QLocale &locale, CT_ColorCloudStdVector *array) const
{
    if(array == NULL)
        return true;

    bool ok;

    CT_Color color;
    color.r() = locale.toInt(wordsOfLine.at(m_columnRedIndex), &ok);

    if(!ok)
        return false;

    color.g() = locale.toInt(wordsOfLine.at(m_columnGreenIndex), &ok);

    if(!ok)
        return false;

    color.b() = locale.toInt(wordsOfLine.at(m_columnBlueIndex), &ok);

    if(!ok)
        return false;

    array->addColor(color);

    return true;
}

bool CT_Reader_Points_ASCII::readAndAddNormal(const QStringList &wordsOfLine, const QLocale &locale, CT_NormalCloudStdVector *array) const
{
    if(array == NULL)
        return true;

    bool ok;
    CT_Normal normal;
    normal.x() = locale.toInt(wordsOfLine.at(m_columnNxIndex), &ok);

    if(!ok)
        return false;

    normal.y() = locale.toInt(wordsOfLine.at(m_columnNyIndex), &ok);

    if(!ok)
        return false;

    normal.z() = locale.toInt(wordsOfLine.at(m_columnNzIndex), &ok);

    if(!ok)
        return false;

    if(m_columnNCurvatureIndex >= 0) {
        normal.w() = locale.toInt(wordsOfLine.at(m_columnNCurvatureIndex), &ok);

        if(!ok)
            return false;
    }

    array->addNormal(normal);

    return true;
}
