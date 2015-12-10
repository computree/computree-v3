#include "pb_pgmexporter.h"

#include "ct_itemdrawable/abstract/ct_virtualgrid2d.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "ct_global/ct_context.h"

PB_PgmExporter::PB_PgmExporter() : CT_AbstractExporter()
{
}

PB_PgmExporter::~PB_PgmExporter()
{
}

QString PB_PgmExporter::getExporterCustomName() const
{
    return tr("Images 2D, format PGM");
}

CT_StepsMenu::LevelPredefined PB_PgmExporter::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Raster;
}


void PB_PgmExporter::init()
{
    addNewExportFormat(FileFormat("pgm", tr("Fichiers Images 2D (pgm)")));

    setToolTip(tr("Portable GrayMap : Format de fichier image en niveaux de gris (NetPBM)"));
}

bool PB_PgmExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{    
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext()
            && myList.isEmpty())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_VirtualGrid2D<int>*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_VirtualGrid2D<int>"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_PgmExporter::configureExport()
{
    if(!errorMessage().isEmpty())
    {
        setErrorMessage(tr("Erreur"));
        return false;
    }

    return true;
}

CT_AbstractExporter* PB_PgmExporter::copy() const
{
    return new PB_PgmExporter();
}

bool PB_PgmExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "pgm";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(filePath);

    QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

    if( itemDrawableToExport().size() > 1 )
    {
        setErrorMessage(tr("Impossible d'enregistrer plusieurs images dans le meme fichier"));
        return false;
    }

    if(file.open(QFile::WriteOnly))
    {
        QTextStream stream(&file);

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            CT_VirtualGrid2D<int> *grid = dynamic_cast<CT_VirtualGrid2D<int>*>(item);
            int width = grid->xArraySize();
            int height = grid->yArraySize();

            size_t totalSize = width*height;
            size_t i = 0;

            if(grid != NULL)
            {
                stream << "P2\n";
                stream << "# Gray level image from Computree software\n";
                stream << width << " " << height << "\n";
                stream << grid->dataMax() << "\n";

                // write data
                for (int y = 0 ; y < height ; y++ )
                {
                    for (int x = 0 ; x < width ; x++ )
                    {
                        stream << grid->value( x, height - 1 - y ) << " ";
                    }

                    if( y != height - 1 )
                    {
                        stream << "\n";
                    }
                }

                ++i;
                setExportProgress( i*100.0/totalSize );
            }
        }

        file.close();
        return true;
    }

    return false;
}
