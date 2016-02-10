#include "ct_reader_pgm.h"

// Using integher limit as NA value
#include <limits>

// Output is a 2d grid
#include "ct_itemdrawable/ct_grid2dxy.h"

CT_Reader_PGM::CT_Reader_PGM()
{
}

QString CT_Reader_PGM::GetReaderName() const
{
    return tr("Raster 2D, Fichier PGM");
}

CT_StepsMenu::LevelPredefined CT_Reader_PGM::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Raster;
}

bool CT_Reader_PGM::setFilePath(const QString &filepath)
{
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);
            CT_Reader_PGM::FileType type;
            size_t w, h;
            quint16 maxGray;

            if(readHeader(stream, type, w, h, maxGray)) {
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

CT_AbstractReader* CT_Reader_PGM::copy() const
{
    return new CT_Reader_PGM();
}

void CT_Reader_PGM::protectedInit()
{
    addNewReadableFormat(FileFormat("pgm", tr("Fichiers raster .pgm")));

    setToolTip(tr("Portable GrayMap : Format de fichier image en niveaux de gris (NetPGM)"));
}

void CT_Reader_PGM::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    addOutItemDrawableModel(DEF_CT_Reader_PGM_gridOut, new CT_Grid2DXY<quint16>(), tr("Raster 2D"));
}

bool CT_Reader_PGM::protectedReadFile()
{
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if(f.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&f);
            CT_Reader_PGM::FileType type;
            size_t w, h;
            quint16 maxGray;
            qint64 offset;

            if(readHeader(stream, type, w, h, maxGray, &offset)) {

                CT_Grid2DXY<quint16>* grid = new CT_Grid2DXY<quint16>(NULL, NULL, 0, 0, w, h, 1, 0, std::numeric_limits<quint16>::max(), 0);
                bool ok = true;

                if(type == BINARY)
                {
                    f.seek(offset);
                    QDataStream binStream(&f);
                    quint16 value16;
                    quint8 value8;

                    for ( size_t i = 0 ; (i < h) && !isStopped(); i++ )
                    {
                        size_t lig = h - (i+1);

                        for ( size_t j = 0 ; (j < w) && !isStopped(); j++ )
                        {
                            if(!binStream.atEnd()) {

                                if(maxGray < 256) {
                                    binStream >> value8;
                                    grid->setValue(j, lig, value8);
                                } else {
                                    binStream >> value16;
                                    grid->setValue(j, lig, value16);
                                }
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

                addOutItemDrawable(DEF_CT_Reader_PGM_gridOut, grid);

                f.close();

                return true;
            }

            f.close();
        }
    }

    return false;
}

QString CT_Reader_PGM::nextLineWithNoComment(QTextStream &stream) const
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

bool CT_Reader_PGM::readHeader(QTextStream &stream, CT_Reader_PGM::FileType &type, size_t &width, size_t &height, quint16 &maxGray, qint64 *outOffset) const
{
    stream.seek(0);

    QString line = nextLineWithNoComment(stream);

    if(line.startsWith("P5"))
        type = BINARY;
    else if(line.startsWith("P2"))
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

    if(!ok)
        return false;

    line = nextLineWithNoComment(stream);
    splitted = splitted = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    if(splitted.size() != 1)
        return false;

    maxGray = splitted.at(0).toUInt(&ok);

    if(outOffset != NULL)
        *outOffset = stream.pos();

    return ok;
}
