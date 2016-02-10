#include "ct_reader_asciigrid3d.h"

#include "ct_itemdrawable/ct_grid3d.h"

CT_Reader_AsciiGrid3D::CT_Reader_AsciiGrid3D()
{
}

QString CT_Reader_AsciiGrid3D::GetReaderName() const
{
    return tr("Grille 3D, Fichiers ascii GRD3D");
}

CT_StepsMenu::LevelPredefined CT_Reader_AsciiGrid3D::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Raster;
}

bool CT_Reader_AsciiGrid3D::setFilePath(const QString &filepath)
{
    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);

            size_t          nCols;
            size_t          nRows;
            size_t          nZLev;
            Eigen::Vector3d min;
            double          cellSize;
            double          noDataValue;

            if(readHeader(stream, nCols, nRows, nZLev, min, cellSize, noDataValue)) {
                f.close();

                if(CT_AbstractReader::setFilePath(filepath)) {
                    return true;
                }
            }

            f.close();
        }
    }

    return false;
}

CT_AbstractReader *CT_Reader_AsciiGrid3D::copy() const
{
    return new CT_Reader_AsciiGrid3D();
}

void CT_Reader_AsciiGrid3D::protectedInit()
{
    addNewReadableFormat(FileFormat(QStringList() << "grid3d" << "grd3d" << "GRD3D", tr("Fichiers grille 3D")));

    setToolTip(tr("Chargement d'une grille 3D depuis un fichier ASCII (format inspiré du format raster ESRI GRID)."));
}

void CT_Reader_AsciiGrid3D::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    addOutItemDrawableModel(DEF_CT_Reader_ASCIIGRD3D_grid3DOut, new CT_Grid3D<float>(), tr("Grille 3D"));
}

bool CT_Reader_AsciiGrid3D::protectedReadFile()
{
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);

            size_t          nCols;
            size_t          nRows;
            size_t          nZLev;
            Eigen::Vector3d min;
            double          cellSize;
            double          noDataValue;

            if(readHeader(stream, nCols, nRows, nZLev, min, cellSize, noDataValue)) {

                CT_Grid3D<float>* loadedGrid = new CT_Grid3D<float>( NULL,
                                                                     NULL,
                                                                     min[0],
                                                                     min[1],
                                                                     min[2],
                                                                     nCols,
                                                                     nRows,
                                                                     nZLev,
                                                                     cellSize,
                                                                     noDataValue,
                                                                     0.0);

                float currentValue;
                for ( size_t i = 0 ; (i < nZLev) && !isStopped() ; ++i )
                {
                    for ( size_t j = nRows - 1 ; (j >= 0) && !isStopped(); --j )
                    {
                        for ( size_t k = 0 ; (k < nCols) && !isStopped() ; ++k )
                        {
                            stream >> currentValue;
                            loadedGrid->setValue( k, j, i, currentValue );
                        }
                    }
                }

                if(!isStopped()) {
                    loadedGrid->computeMinMax();
                    addOutItemDrawable(DEF_CT_Reader_ASCIIGRD3D_grid3DOut, loadedGrid);
                } else {
                    delete loadedGrid;
                }

                f.close();

                return true;
            }

            f.close();
        }
    }

    return false;
}

bool CT_Reader_AsciiGrid3D::readHeader(QTextStream &stream,
                                       size_t &nCols,
                                       size_t &nRows,
                                       size_t &nZLev,
                                       Eigen::Vector3d &min,
                                       double &cellSize,
                                       double &noDataValue)
{
    QString     currentLine;
    QStringList splitline;
    bool        read = false;
    int line = 1;

    // Initialise les parametres de la grille
    currentLine = stream.readLine();
    splitline = currentLine.split('\t');

    if(splitline.size() > 1)
        nCols = splitline.at(1).toULongLong(&read);

    if( !read )
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Unable to convert the ascii format to a integer in header at line %1").arg(line).arg(line) );
        return false;
    }

    read = false;
    ++line;
    currentLine = stream.readLine();
    splitline = currentLine.split('\t');

    if(splitline.size() > 1)
        nRows = splitline.at(1).toULongLong(&read);

    if( !read )
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Unable to convert the ascii format to a integer in header at line %1").arg(line) );
        return false;
    }

    read = false;
    ++line;
    currentLine = stream.readLine();
    splitline = currentLine.split('\t');

    if(splitline.size() > 1)
        nZLev = splitline.at(1).toULongLong(&read);

    if( !read )
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Unable to convert the ascii format to a integer in header at line %1").arg(line) );
        return false;
    }

    read = false;
    ++line;
    currentLine = stream.readLine();
    splitline = currentLine.split('\t');

    if(splitline.size() > 1)
        min[0] = splitline.at(1).toDouble(&read);

    if( !read )
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Unable to convert the ascii format to a real number at line %1").arg(line) );
        return false;
    }

    read = false;
    ++line;
    currentLine = stream.readLine();
    splitline = currentLine.split('\t');

    if(splitline.size() > 1)
        min[1] = splitline.at(1).toDouble(&read);

    if( !read )
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Unable to convert the ascii format to a real number at line %1").arg(line) );
        return false;
    }

    read = false;
    ++line;
    currentLine = stream.readLine();
    splitline = currentLine.split('\t');

    if(splitline.size() > 1)
        min[2] = splitline.at(1).toDouble(&read);

    if( !read )
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Unable to convert the ascii format to a real number at line %1").arg(line) );
        return false;
    }

    read = false;
    ++line;
    currentLine = stream.readLine();
    splitline = currentLine.split('\t');

    if(splitline.size() > 1)
        cellSize = splitline.at(1).toDouble(&read);

    if( !read )
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Unable to convert the ascii format to a real number at line %1").arg(line) );
        return false;
    }

    read = false;
    ++line;
    currentLine = stream.readLine();
    splitline = currentLine.split('\t');

    if(splitline.size() > 1)
        noDataValue = splitline.at(1).toDouble(&read);

    if( !read )
    {
        PS_LOG->addErrorMessage(LogInterface::reader, tr("Unable to convert the ascii format to a real number at line %1").arg(line) );
        return false;
    }

    return true;
}
