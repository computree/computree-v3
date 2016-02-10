#include "ct_reader_pbm.h"

// Using integher limit as NA value
#include <limits>

// Output is a 2d grid
#include "ct_itemdrawable/ct_grid2dxy.h"

CT_Reader_PBM::CT_Reader_PBM()
{
}

QString CT_Reader_PBM::GetReaderName() const
{
    return tr("Raster 2D, Fichier PGM");
}

CT_StepsMenu::LevelPredefined CT_Reader_PBM::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Raster;
}

bool CT_Reader_PBM::setFilePath(const QString &filepath)
{
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);
            CT_Reader_PBM::FileType type;
            size_t w, h;

            if(readHeader(stream, type, w, h)) {
                f.close();
                return CT_AbstractReader::setFilePath(filepath);
            } else {
                PS_LOG->addErrorMessage(LogInterface::reader, tr("Error while loading header of file ") + filepath);
            }

            f.close();
        }
    }

    return false;
}

CT_AbstractReader* CT_Reader_PBM::copy() const
{
    return new CT_Reader_PBM();
}

void CT_Reader_PBM::protectedInit()
{
    addNewReadableFormat(FileFormat("pbm", tr("Fichiers raster .pbm")));

    setToolTip(tr("Portable GrayMap : Format de fichier image en niveaux de gris (NetPBM)"));
}

void CT_Reader_PBM::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    addOutItemDrawableModel(DEF_CT_Reader_PBM_gridOut, new CT_Grid2DXY<quint8>(), tr("Raster 2D"));
}

bool CT_Reader_PBM::protectedReadFile()
{
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);
            CT_Reader_PBM::FileType type;
            size_t w, h;
            qint64 offset;

            if(readHeader(stream, type, w, h, &offset)) {

                CT_Grid2DXY<quint8>* grid = new CT_Grid2DXY<quint8>(NULL, NULL, 0, 0, w, h, 1, 0, std::numeric_limits<quint8>::max(), 0);
                bool ok = true;

                if(type == BINARY)
                {
                    f.seek(offset);
                    QDataStream binStream(&f);
                    quint8 value8;

                    for ( size_t i = 0 ; (i < h) && !isStopped(); i++ )
                    {
                        size_t lig = h - (i+1);

                        for ( size_t j = 0 ; (j < w) && !isStopped(); j+=8 )
                        {
                            if(!binStream.atEnd()) {
                                binStream >> value8;

                                if(value8 > 0)
                                    value8 = value8;

                                grid->setValue(j+7, lig, value8 & 1);
                                grid->setValue(j+6, lig, (value8 & 2) >> 1);
                                grid->setValue(j+5, lig, (value8 & 4) >> 2);
                                grid->setValue(j+4, lig, (value8 & 8) >> 3);
                                grid->setValue(j+3, lig, (value8 & 16) >> 4);
                                grid->setValue(j+2, lig, (value8 & 32) >> 5);
                                grid->setValue(j+1, lig, (value8 & 64) >> 6);
                                grid->setValue(j, lig, (value8 & 128) >> 7);
                            } else {
                                PS_LOG->addErrorMessage(LogInterface::reader, tr("Error while loading the file ") + filepath());

                                delete grid;
                                f.close();

                                return false;
                            }
                        }
                    }
                }
                else
                {
                    QString currentValue;
                    quint16 value;

                    for ( size_t i = 0 ; (i < h) && !isStopped(); i++ )
                    {
                        size_t lig = h - (i+1);

                        for ( size_t j = 0 ; (j < w) && !isStopped(); j++ )
                        {
                            ok = true;

                            if(stream.atEnd())
                                ok = false;

                            if(ok) {
                                stream >> currentValue;

                                value = currentValue.toUInt(&ok);

                                if(ok)
                                    grid->setValue(j, lig, value);
                            }

                            if(!ok) {
                                PS_LOG->addErrorMessage(LogInterface::reader, tr("Error while loading the file ") + filepath());

                                delete grid;
                                f.close();

                                return false;
                            }
                        }
                    }
                }

                if(!isStopped())
                    grid->computeMinMax();

                addOutItemDrawable(DEF_CT_Reader_PBM_gridOut, grid);

                f.close();

                return true;
            }

            f.close();
        }
    }

    return false;
}

QString CT_Reader_PBM::nextLineWithNoComment(QTextStream &stream) const
{
    QString line;

    if(!stream.atEnd()) {
        do
        {
            line = stream.readLine().simplified();
        } while((line.startsWith("#") || line.isEmpty()) && !stream.atEnd());

        if(stream.atEnd())
            line = "";
    }

    return line;
}

bool CT_Reader_PBM::readHeader(QTextStream &stream, CT_Reader_PBM::FileType &type, size_t &width, size_t &height, qint64 *outOffset) const
{
    stream.seek(0);

    QString line = nextLineWithNoComment(stream);

    if(line.startsWith("P4"))
        type = BINARY;
    else if(line.startsWith("P1"))
        type = ASCII;
    else
        return false;

    line = nextLineWithNoComment(stream);
    QStringList splitted = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    if(splitted.size() != 2)
        return false;

    bool ok;
    width = splitted.at(0).toULongLong(&ok);

    if(!ok)
        return false;

    height = splitted.at(1).toULongLong(&ok);

    if(outOffset != NULL)
        *outOffset = stream.pos();

    return ok;
}
