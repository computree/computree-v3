#include "pb_grid3dastableexporter.h"
#include "ct_itemdrawable/abstract/ct_abstractgrid3d.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "ct_global/ct_context.h"

PB_Grid3DAsTableExporter::PB_Grid3DAsTableExporter() : CT_AbstractExporter()
{
}

PB_Grid3DAsTableExporter::~PB_Grid3DAsTableExporter()
{

}

QString PB_Grid3DAsTableExporter::getExporterCustomName() const
{
    return tr("Grilles 3D, table (X,Y,Z,Valeur)");
}

CT_StepsMenu::LevelPredefined PB_Grid3DAsTableExporter::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Voxels;
}


void PB_Grid3DAsTableExporter::init()
{
    addNewExportFormat(FileFormat("txt", tr("Fichiers txt")));

    setToolTip(tr("Exporte des grilles 3D au format table ASCII. Les champs suivants sont exportés :<br>"
                  "- X  : Coordonnée X de la cellule dans la grille (ligne)<br>"
                  "- Y  : Coordonnée Y de la cellule dans la grille (colonne)<br>"
                  "- Z  : Coordonnée Z de la cellule dans la grille (niveau Z)<br>"
                  "- Valeur  : Valeur dans la cellule<br>"));
}

bool PB_Grid3DAsTableExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();
        if(dynamic_cast<CT_AbstractGrid3D*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_AbstractGrid3D"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_Grid3DAsTableExporter::configureExport()
{
    if(!errorMessage().isEmpty())
    {
        QMessageBox::critical(NULL, tr("Erreur"), errorMessage(), QMessageBox::Ok);
        return false;
    }

    return true;
}

CT_AbstractExporter* PB_Grid3DAsTableExporter::copy() const
{
    return new PB_Grid3DAsTableExporter();
}

bool PB_Grid3DAsTableExporter::protectedExportToFile()
{
    bool ok = true;

    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "txt";

    QString indice = "";
    if (itemDrawableToExport().size() > 1) {indice = "_0";}
    int cpt = 0;

    QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());
    while (it.hasNext())
    {
        CT_AbstractGrid3D* item = dynamic_cast<CT_AbstractGrid3D*>(it.next());

        QString filePath = QString("%1/%2%3.%4").arg(path).arg(baseName).arg(indice).arg(suffix);

        QFile file(filePath);

        if(file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);

            // write header
            size_t xdim = item->xdim();
            size_t ydim = item->ydim();
            size_t zdim = item->zdim();

            stream << "Xcorner\tYcorner\tZcorner\tXcenter\tYcenter\tZcenter\tColX\tRowY\tLevZ\tValue\n";

            // write data
            for (size_t xx = 0 ; xx < xdim ; xx++)
            {
                for (size_t yy = 0 ; yy < ydim ; yy++)
                {
                    for (size_t zz = 0 ; zz < zdim ; zz++)
                    {

                        size_t sIndex;
                        item->index(xx, yy, zz, sIndex);

                        Eigen::Vector3d corner;
                        item->getCellBottomLeftCorner(xx, yy, zz, corner);

                        stream << corner(0) << "\t";
                        stream << corner(1) << "\t";
                        stream << corner(2) << "\t";

                        stream << item->getCellCenterX(xx) << "\t";
                        stream << item->getCellCenterY(yy) << "\t";
                        stream << item->getCellCenterZ(zz) << "\t";
                        stream << xx << "\t";
                        stream << yy << "\t";
                        stream << zz << "\t";
                        stream << item->valueAtIndexAsString(sIndex);
                        stream << "\n";
                    }
                }
            }

            file.close();
        } else {ok = false;}
        indice = QString("_%1").arg(++cpt);
    }

    return ok;
}
