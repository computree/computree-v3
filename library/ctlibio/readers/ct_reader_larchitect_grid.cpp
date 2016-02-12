#include "ct_reader_larchitect_grid.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include "ct_colorcloud/ct_colorcloudstdvector.h"
#include "ct_global/ct_context.h"
#include <QDebug>

#include <limits>

#define CLEAR_CLOSE_FALSE(argFunc, argErrMsg) if(argFunc) { delete wood_surface; \
                                                            delete leaf_surface; \
                                                            delete wood_volume; \
                                                            delete leaf_volume; \
                                                            delete all_surface; \
                                                            delete all_volume;  \
                                                            f.close(); \
                                                            PS_LOG->addErrorMessage(LogInterface::reader, argErrMsg); \
                                                            return false; }

#define wood_surface_Exist (wood_surface!=NULL)
#define wood_volume_Exist (wood_volume!=NULL)
#define leaf_surface_Exist (leaf_surface!=NULL)
#define leaf_volume_Exist (leaf_volume!=NULL)
#define all_surface_Exist (all_surface!=NULL)
#define all_volume_Exist (all_volume!=NULL)

CT_Reader_LArchitect_Grid::CT_Reader_LArchitect_Grid() : CT_AbstractReader()
{
    setToolTip(tr("Charge des grilles 3D depuis un fichier au format LArchitect"));
}


QString CT_Reader_LArchitect_Grid::GetReaderName() const
{
    return tr("Grilles 3D, format LArchitect");
}

CT_StepsMenu::LevelPredefined CT_Reader_LArchitect_Grid::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Voxels;
}

bool CT_Reader_LArchitect_Grid::setFilePath(const QString &filepath)
{
    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&f);

            Eigen::Vector3d min;
            Eigen::Vector3d max;
            Eigen::Vector3d res;
            EigenVector3ui64 dim;
            int nMat;
            QMap<QString, int>  matNames;

            if(readHeader(stream, min, max, res, dim, nMat, matNames)) {
                f.close();
                return CT_AbstractReader::setFilePath(filepath);
            } else {
                PS_LOG->addErrorMessage(LogInterface::reader, tr("Error when read header of the file %1").arg(filepath));
            }

            f.close();
        }
    }

    return false;
}

CT_AbstractReader* CT_Reader_LArchitect_Grid::copy() const
{
    return new CT_Reader_LArchitect_Grid();
}

void CT_Reader_LArchitect_Grid::protectedInit()
{
    addNewReadableFormat(FileFormat("grid", tr("Fichiers de grilles 3D LArchitect")));
}

void CT_Reader_LArchitect_Grid::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    addOutItemDrawableModel(DEF_CT_Reader_LArchitect_Grid_wood_surface, new CT_Grid3D<float>());
    addOutItemDrawableModel(DEF_CT_Reader_LArchitect_Grid_leaf_surface, new CT_Grid3D<float>());
    addOutItemDrawableModel(DEF_CT_Reader_LArchitect_Grid_wood_volume, new CT_Grid3D<float>());
    addOutItemDrawableModel(DEF_CT_Reader_LArchitect_Grid_leaf_volume, new CT_Grid3D<float>());
    addOutItemDrawableModel(DEF_CT_Reader_LArchitect_Grid_all_surface, new CT_Grid3D<float>());
    addOutItemDrawableModel(DEF_CT_Reader_LArchitect_Grid_all_volume, new CT_Grid3D<float>());
}

bool CT_Reader_LArchitect_Grid::protectedReadFile()
{
    // Test File validity
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&f);

            Eigen::Vector3d min;
            Eigen::Vector3d max;
            Eigen::Vector3d res;
            EigenVector3ui64 dim;
            int nMat;
            QMap<QString, int>  matNames;

            if(readHeader(stream, min, max, res, dim, nMat, matNames)) {

                CT_Grid3D<float> *wood_surface = NULL;
                CT_Grid3D<float> *leaf_surface = NULL;
                CT_Grid3D<float> *wood_volume = NULL;
                CT_Grid3D<float> *leaf_volume = NULL;
                CT_Grid3D<float> *all_surface = NULL;
                CT_Grid3D<float> *all_volume = NULL;
                CT_Grid3D<float> *refGrid = NULL;

                if (1)
                {
                    wood_surface = createGrid(min, res, dim);
                    refGrid = wood_surface;
                }

                if (1)
                {
                    leaf_surface = createGrid(min, res, dim);
                    refGrid = leaf_surface;
                }

                if (1)
                {
                    wood_volume = createGrid(min, res, dim);
                    refGrid = wood_volume;
                }

                if (1)
                {
                    leaf_volume = createGrid(min, res, dim);
                    refGrid = leaf_volume;
                }

                if (1)
                {
                    all_surface = createGrid(min, res, dim);
                    refGrid = all_surface;
                }

                if (1)
                {
                    all_volume = createGrid(min, res, dim);
                    refGrid = all_volume;
                }

                int wood_index = matNames.value("WOOD", -1);
                int leaf_index = matNames.value("FOLIAGE", -1);

                CLEAR_CLOSE_FALSE ((wood_surface_Exist || wood_volume_Exist) && wood_index==-1, tr("Pas d'index bois dans le fichier"))
                CLEAR_CLOSE_FALSE((leaf_surface_Exist|| leaf_volume_Exist) && leaf_index==-1, tr("Pas d'index feuille dans le fichier"))

                qint64 fileSize = f.size();
                qint64 readedSize = 0;

                bool ok;
                bool okSurf, okVol;
                float surface, volume;
                int pos, code;
                size_t index;
                QString line;
                QStringList list;
                int lSize;

                for (size_t xx = 0 ; (xx < dim[0]) && !isStopped(); ++xx)
                {
                    for (size_t yy = 0 ; (yy < dim[1]) && !isStopped(); ++yy)
                    {
                        for (size_t zz = 0 ; (zz < dim[2]) && !stream.atEnd() && !isStopped(); ++zz)
                        {
                            line = stream.readLine();
                            readedSize += line.size();

                            list = line.split(" ");
                            lSize = list.size();

                            if (refGrid->index(xx, yy, zz, index))
                            {
                                for (int i = 0 ; i < nMat ; ++i)
                                {
                                    pos = 3*i;
                                    if (lSize >= (pos + 1))
                                    {
                                        code = list.at(pos).toInt(&ok);
                                        if (ok)
                                        {
                                            if (code == wood_index && list.size() >= pos + 3)
                                            {
                                                surface = list.at(pos+1).toFloat(&okSurf);
                                                volume = list.at(pos+2).toFloat(&okVol);

                                                if (wood_surface_Exist && okSurf) {wood_surface->setValueAtIndex(index, surface);}
                                                if (wood_volume_Exist && okVol)  {wood_volume->setValueAtIndex(index, volume);}
                                                if (all_surface_Exist && okSurf)  {all_surface->addValueAtIndex(index, surface);}
                                                if (all_volume_Exist && okVol)   {all_volume->addValueAtIndex(index, volume);}


                                            } else if (code == leaf_index && list.size() >= pos + 3)
                                            {
                                                surface = list.at(pos+1).toFloat(&okSurf);
                                                volume = list.at(pos+2).toFloat(&okVol);

                                                if (leaf_surface_Exist && okSurf) {leaf_surface->setValueAtIndex(index, surface);}
                                                if (leaf_volume_Exist && okVol)  {leaf_volume->setValueAtIndex(index, volume);}
                                                if (all_surface_Exist && okSurf)  {all_surface->addValueAtIndex(index, surface);}
                                                if (all_volume_Exist && okVol)   {all_volume->addValueAtIndex(index, volume);}

                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                PS_LOG->addWarningMessage(LogInterface::reader, tr("Indice hors grille"));
                            }

                            list.clear();
                        }
                    }

                    setProgress((readedSize*100)/fileSize);
                }

                f.close();

                if (wood_surface_Exist) {wood_surface->computeMinMax();}
                if (wood_volume_Exist) {wood_volume->computeMinMax();}
                if (leaf_surface_Exist) {leaf_surface->computeMinMax();}
                if (leaf_volume_Exist) {leaf_volume->computeMinMax();}
                if (all_surface_Exist) {all_surface->computeMinMax();}
                if (all_volume_Exist) {all_volume->computeMinMax();}

                addOutItemDrawable(DEF_CT_Reader_LArchitect_Grid_all_surface, all_surface);
                addOutItemDrawable(DEF_CT_Reader_LArchitect_Grid_all_volume, all_volume);
                addOutItemDrawable(DEF_CT_Reader_LArchitect_Grid_leaf_surface, leaf_surface);
                addOutItemDrawable(DEF_CT_Reader_LArchitect_Grid_leaf_volume, leaf_volume);
                addOutItemDrawable(DEF_CT_Reader_LArchitect_Grid_wood_surface, wood_surface);
                addOutItemDrawable(DEF_CT_Reader_LArchitect_Grid_wood_volume, wood_volume);

                return true;
            }

            f.close();
        }
    }

    return false;
}

bool CT_Reader_LArchitect_Grid::readHeader(QTextStream &stream, Eigen::Vector3d &min, Eigen::Vector3d &max, Eigen::Vector3d &res, EigenVector3ui64 &dim, int &nMat, QMap<QString, int> &matNames) const
{
    if(stream.atEnd())
        return false;

    // Read (x,y,z) min and max limits of the grid
    QString line = stream.readLine();

    QStringList values = line.split(" ");

    if(values.size() < 6)
        return false;

    bool ok[6];
    min[0] = values.at(0).toDouble(&ok[0]);
    min[1] = values.at(1).toDouble(&ok[1]);
    min[2] = values.at(2).toDouble(&ok[2]);
    max[0] = values.at(3).toDouble(&ok[3]);
    max[1] = values.at(4).toDouble(&ok[4]);
    max[2] = values.at(5).toDouble(&ok[5]);

    if (!ok[0] || !ok[1] || !ok[2] || !ok[3] || !ok[4] || !ok[5])
        return false;

    // read resolution of the grid
    line = stream.readLine();

    values = line.split(" ");

    if (values.size() < 3)
        return false;

    res[0] = values.at(0).toDouble(&ok[0]);
    res[1] = values.at(1).toDouble(&ok[1]);
    res[2] = values.at(2).toDouble(&ok[2]);

    if (!ok[0] || !ok[1] || !ok[2])
        return false;

    if ((res[0] != res[1]) || (res[0] != res[2]))
        return false;

    // read dimensions of the grid
    line = stream.readLine();

    values = line.split(" ");

    if (values.size() < 3)
        return false;

    dim[0] = values.at(0).toUInt(&ok[0]);
    dim[1] = values.at(1).toUInt(&ok[1]);
    dim[2] = values.at(2).toUInt(&ok[2]);

    if (!ok[0] || !ok[1] || !ok[2])
        return false;

    // read number of materials
    line = stream.readLine();

    values = line.split(" ");

    int size = values.size();

    if(size > 0)
    {
        nMat = values.at(0).toInt(&ok[0]);

        if (!ok[0] || (nMat <= 0) || (size < (2*nMat+3)))
            return false;

        int matid;
        QString matName;

        // read materials ids and names
        for (int i = 0 ; i < nMat; ++i)
        {
            matid = values.at(2*i+1).toInt(&ok[0]);
            matName = values.at(2*i+2);

            if (!ok[0] || matName.size() <= 0)
                return false;

            matNames.insert(matName.toUpper(), matid);
        }
    }

    return true;
}

CT_Grid3D<float>* CT_Reader_LArchitect_Grid::createGrid(Eigen::Vector3d &min, Eigen::Vector3d &res, CT_Reader_LArchitect_Grid::EigenVector3ui64 &dim) const
{
    return new CT_Grid3D<float>(NULL, NULL, min[0], min[1], min[2], dim[0], dim[1], dim[2], res[0], -1, 0);
}
