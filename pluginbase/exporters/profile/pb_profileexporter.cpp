#include "pb_profileexporter.h"
#include "ct_itemdrawable/abstract/ct_abstractprofile.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "ct_global/ct_context.h"

PB_ProfileExporter::PB_ProfileExporter() : CT_AbstractExporter()
{
}

PB_ProfileExporter::~PB_ProfileExporter()
{

}

QString PB_ProfileExporter::getExporterCustomName() const
{
    return tr("Profils, ACSII");
}

CT_StepsMenu::LevelPredefined PB_ProfileExporter::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Items;
}

void PB_ProfileExporter::init()
{
    addNewExportFormat(FileFormat("hist", tr("Fichiers Profile (ASCII)")));

    setToolTip(tr("Export de profils au format ASCII (1 fichier par profil).<br>"
                  "Le fichier comporte d'abord une entête inspirée du format ASCII ESRI GRID pour les rasters, décrivant l'origine, la direction et la résolution du profil.<br>"
                  "Ensuite, une table fournit les champs suivants :<br>"
                  "- Level : Numéro de la cellule dans le profil<br>"
                  "- Length : Longueur correspondant au centre de la cellule le long du profil<br>"
                  "- Xc : Coordonnée X du centre de la cellule<br>"
                  "- Yc : Coordonnée Y du centre de la cellule<br>"
                  "- Zc : Coordonnée Z du centre de la cellule<br>"
                  "- Value : Valeur de la cellule"));
}

bool PB_ProfileExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);
    int nProfiles = 0;

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();
        if(dynamic_cast<CT_AbstractProfile*>(item) != NULL)
        {
            myList.append(item);
            ++nProfiles;
        }
    }

    if(nProfiles == 0)
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_AbstractProfile"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

bool PB_ProfileExporter::configureExport()
{
    if(!errorMessage().isEmpty())
    {
        QMessageBox::critical(NULL, tr("Erreur"), errorMessage(), QMessageBox::Ok);
        return false;
    }

    return true;
}

CT_AbstractExporter* PB_ProfileExporter::copy() const
{
    return new PB_ProfileExporter();
}

bool PB_ProfileExporter::protectedExportToFile()
{
    bool ok = true;

    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "hist";

    QString indice = "";
    if (itemDrawableToExport().size() > 1) {indice = "_0";}
    int cpt = 0;

    QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());
    while (it.hasNext())
    {
        CT_AbstractProfile* item = dynamic_cast<CT_AbstractProfile*>(it.next());

        QString filePath = QString("%1/%2%3.%4").arg(path).arg(baseName).arg(indice).arg(suffix);

        QFile file(filePath);

        if(file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);

            // write header
            size_t dim = item->nCells();

            stream << "Nlevels\t" << dim << "\n";

            stream << "Xorigin\t" << item->minX() << "\n";
            stream << "Yorigin\t" << item->minY() << "\n";
            stream << "Zorigin\t" << item->minZ() << "\n";

            const Eigen::Vector3d &direction = item->getDirection();

            stream << "Xdirection\t" << direction(0) << "\n";
            stream << "Ydirection\t" << direction(1) << "\n";
            stream << "Zdirection\t" << direction(2) << "\n";

            stream << "Resolution\t" << item->resolution() << "\n";
            stream << "NODATA_value\t" << item->NAAsString() << "\n";

            stream << "Level\tLength\tXc\tYc\tZc\tValue\n";

            // write data
            for (size_t index = 0 ; index < dim ; index++)
            {
                Eigen::Vector3d cellCenter;
                item->getCellCenterXYZ(index, cellCenter);

                stream << index << "\t";
                stream << item->lengthForIndex(index) << "\t";
                stream << cellCenter(0) << "\t";
                stream << cellCenter(1) << "\t";
                stream << cellCenter(2) << "\t";
                stream << item->valueAtIndexAsString(index) << "\n";
            }

            file.close();
        } else {ok = false;}
        indice = QString("_%1").arg(++cpt);
    }

    return ok;
}
