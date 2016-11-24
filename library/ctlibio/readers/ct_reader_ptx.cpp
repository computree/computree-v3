#include "ct_reader_ptx.h"

#include "ct_global/ct_context.h"
#include "ct_itemdrawable/ct_pointsattributesscalartemplated.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"
#include "ct_itemdrawable/ct_transformationmatrix.h"
#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_iterator/ct_mutablepointiterator.h"
#include "ct_view/ct_genericconfigurablewidget.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"

#include <QTextStream>
#include <QDebug>

#define DEF_CT_Reader_PTX_groupOut      "grp"
#define DEF_CT_Reader_PTX_sceneOut      "sce"
#define DEF_CT_Reader_PTX_intensityOut  "int"
#define DEF_CT_Reader_PTX_rgbOut        "rgb"
#define DEF_CT_Reader_PTX_scannerOut    "sca"
#define DEF_CT_Reader_PTX_matrixOut     "mt"

CT_Reader_PTX::CT_Reader_PTX() : CT_AbstractReader()
{
    m_applyTransformation = true;
}

QString CT_Reader_PTX::GetReaderName() const
{
    return tr("Points, Fichier PTX");
}

CT_StepsMenu::LevelPredefined CT_Reader_PTX::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}

bool CT_Reader_PTX::setFilePath(const QString &filepath)
{
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);

            int nColumn, nRow;
            Eigen::Matrix4d matrix;
            bool hasColors;

            if(readHeaderValues(stream, nColumn, nRow, matrix, hasColors)) {
                CT_AbstractReader::setFilePath(filepath);
                return true;
            }

            f.close();;
        }
    }

    return false;
}

bool CT_Reader_PTX::configure()
{
    CT_GenericConfigurableWidget w;
    w.addBool("","",tr("Appliquer la matrice de transformation"), m_applyTransformation);

    CT_ConfigurableWidgetToDialog d;
    return (d.exec(&w) == QDialog::Accepted);
}

void CT_Reader_PTX::setApplyTransformationMatrix(bool enable)
{
    m_applyTransformation = enable;
}

bool CT_Reader_PTX::applyTransformationMatrix() const
{
    return m_applyTransformation;
}

SettingsNodeGroup *CT_Reader_PTX::getAllSettings() const
{
    SettingsNodeGroup *root = CT_AbstractReader::getAllSettings();

    SettingsNodeGroup *group = new SettingsNodeGroup("CT_Reader_PTX_Settings");
    group->addValue(new SettingsNodeValue("applyTransformationMatrix", applyTransformationMatrix()));

    root->addGroup(group);

    return root;
}

bool CT_Reader_PTX::setAllSettings(const SettingsNodeGroup *settings)
{
    if(CT_AbstractReader::setAllSettings(settings))
    {
        QList<SettingsNodeGroup*> listG = settings->groupsByTagName("CT_Reader_PTX_Settings");

        if(listG.isEmpty())
            return false;

        QList<SettingsNodeValue*> listV = listG.first()->valuesByTagName("applyTransformationMatrix");

        if(listV.isEmpty())
            return false;

        setApplyTransformationMatrix(listV.first()->value().toBool());

        return true;
    }

    return false;
}

CT_AbstractReader* CT_Reader_PTX::copy() const
{
    return new CT_Reader_PTX();
}

bool CT_Reader_PTX::readHeaderValues(QTextStream &stream, int &nColumn, int &nRow, Eigen::Matrix4d &matrix, bool &hasColors) const
{
    if(stream.atEnd())
        return false;

    QString firstLine = stream.readLine().simplified();

    if(firstLine.isEmpty()) {
        if(stream.atEnd())
            return false;

        firstLine = stream.readLine();
    }

    bool ok;
    nColumn = firstLine.toInt(&ok);

    if(!ok || stream.atEnd())
        return false;

    nRow = stream.readLine().toInt(&ok);

    if(!ok || stream.atEnd())
        return false;

    // skip 4 lines
    stream.readLine();
    if(stream.atEnd())
        return false;

    stream.readLine();
    if(stream.atEnd())
        return false;

    stream.readLine();
    if(stream.atEnd())
        return false;

    stream.readLine();
    if(stream.atEnd())
        return false;

    // read matrix
    for(int i=0; i<4; ++i) {
        for(int j=0; j<4; ++j) {
            stream >> matrix(i,j);
        }
    }

    qint64 pos = stream.pos();
    firstLine = stream.readLine().simplified();

    if(firstLine.isEmpty()) {
        if(stream.atEnd())
            return false;

        pos = stream.pos();
        firstLine = stream.readLine();
    }

    QString pointLine = firstLine.simplified();
    int nPointsCol = pointLine.split(" ").size();
    hasColors = (nPointsCol == 7);
    stream.seek(pos);

    return true;
}

void CT_Reader_PTX::protectedInit()
{
    addNewReadableFormat(FileFormat("ptx", tr("Fichiers de points .ptx")));

    setToolTip(tr("Chargement de points depuis un fichier format ascii PTX (FARO)"));
}

void CT_Reader_PTX::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    CT_OutStdGroupModel *group = new CT_OutStdGroupModel(DEF_CT_Reader_PTX_groupOut, new CT_StandardItemGroup(), tr("Scenes"));

    m_sceneModel = new CT_OutStdSingularItemModel(DEF_CT_Reader_PTX_sceneOut, new CT_Scene(), tr("Scène"));
    m_intModel = new CT_OutStdSingularItemModel(DEF_CT_Reader_PTX_intensityOut, new CT_PointsAttributesScalarTemplated<double>(), tr("Intensité"));
    m_rgbModel = new CT_OutStdSingularItemModel(DEF_CT_Reader_PTX_rgbOut, new CT_PointsAttributesColor(), tr("RGB"));
    m_matrixModel = new CT_OutStdSingularItemModel(DEF_CT_Reader_PTX_matrixOut, new CT_TransformationMatrix(), tr("Matrice de transformation"));
    m_scannerModel = new CT_OutStdSingularItemModel(DEF_CT_Reader_PTX_scannerOut, new CT_Scanner(), tr("Scanner"));

    group->addItem(m_sceneModel);
    group->addItem(m_intModel);
    group->addItem(m_rgbModel);
    group->addItem(m_matrixModel);
    group->addItem(m_scannerModel);

    addOutGroupModel(group);
}

bool CT_Reader_PTX::protectedReadFile()
{
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);

            int nColumn, nRow;
            Eigen::Matrix4d matrix;
            bool hasColors;

            while(readHeaderValues(stream, nColumn, nRow, matrix, hasColors)) {

                size_t nPoints = nRow*nColumn;

                CT_Point pReaded;

                CT_NMPCIR pcir = PS_REPOSITORY->createNewPointCloud(nPoints);
                CT_MutablePointIterator it(pcir);

                CT_StandardCloudStdVectorT<double> *collection = new CT_StandardCloudStdVectorT<double>(nPoints);
                CT_ColorCloudStdVector *cc;

                if(hasColors) {
                    cc = new CT_ColorCloudStdVector(nPoints);
                }

                double xmin = std::numeric_limits<double>::max();
                double ymin = std::numeric_limits<double>::max();
                double zmin = std::numeric_limits<double>::max();
                double imin = std::numeric_limits<double>::max();

                double xmax = -std::numeric_limits<double>::max();
                double ymax = -std::numeric_limits<double>::max();
                double zmax = -std::numeric_limits<double>::max();
                double imax = -std::numeric_limits<double>::max();

                Eigen::Vector4d pXYZ;
                double reflectance;
                quint16 r,g,b;
                CT_Color color;
                size_t a = 0;

                while((a<nPoints)
                      && !stream.atEnd()
                      && !isStopped())
                {
                    stream >> pXYZ(0);
                    stream >> pXYZ(1);
                    stream >> pXYZ(2);
                    stream >> reflectance;

                    if(hasColors) {
                        stream >> r;
                        stream >> g;
                        stream >> b;

                        color.r() = r;
                        color.g() = g;
                        color.b() = b;

                        (*cc)[a] = color;
                    }

                    if(m_applyTransformation)
                        pXYZ = matrix * pXYZ;

                    pReaded(CT_Point::X) = pXYZ(0);
                    pReaded(CT_Point::Y) = pXYZ(1);
                    pReaded(CT_Point::Z) = pXYZ(2);

                    xmin = qMin(pReaded(CT_Point::X), xmin);
                    ymin = qMin(pReaded(CT_Point::Y), ymin);
                    zmin = qMin(pReaded(CT_Point::Z), zmin);
                    xmax = qMax(pReaded(CT_Point::X), xmax);
                    ymax = qMax(pReaded(CT_Point::Y), ymax);
                    zmax = qMax(pReaded(CT_Point::Z), zmax);
                    imin = qMin(reflectance, imin);
                    imax = qMax(reflectance, imax);

                    it.next();
                    it.replaceCurrentPoint(pReaded);

                    (*collection)[a] = reflectance;

                    ++a;
                    setProgress(a*100/nPoints);
                }

                CT_StandardItemGroup *group = new CT_StandardItemGroup(NULL, NULL);

                CT_Scene *scene = new CT_Scene(m_sceneModel, NULL, pcir);
                scene->setBoundingBox(xmin, ymin, zmin, xmax, ymax, zmax);

                // add the scene
                group->addItemDrawable(scene);

                CT_PointsAttributesScalarTemplated<double> *pas = new CT_PointsAttributesScalarTemplated<double>(m_intModel,
                                                                                                                   NULL,
                                                                                                                   pcir,
                                                                                                                   collection,
                                                                                                                   imin,
                                                                                                                   imax);

                // add attributes
                group->addItemDrawable(pas);

                if (hasColors) {
                    CT_PointsAttributesColor *colors = new CT_PointsAttributesColor(m_rgbModel,
                                                                                    NULL,
                                                                                    pcir,
                                                                                    cc);

                    addOutItemDrawable(DEF_CT_Reader_PTX_rgbOut, colors);
                }

                double hres = 150.0 / ((double)nRow);
                double vres = 360.0 / ((double)nColumn);

                Eigen::Vector3d scanPos;
                scanPos(0) = matrix(0,4);
                scanPos(1) = matrix(1,4);
                scanPos(2) = matrix(2,4);

                // add the scanner
                group->addItemDrawable(new CT_Scanner(m_scannerModel, NULL, 0, scanPos, Eigen::Vector3d(0,1,0), 360, 150, hres, vres, 0, 0, true, false));

                // add the matrix
                group->addItemDrawable(new CT_TransformationMatrix(m_matrixModel, NULL, matrix));

                addOutGroup(DEF_CT_Reader_PTX_groupOut, group);
            }

            f.close();

            return true;
        }
    }

    return false;
}

