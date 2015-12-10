#include "pb_ascrgbexporter.h"

#include <math.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <QApplication>
#include <QProgressDialog>
#include <QFileInfo>

#include "ct_global/ct_context.h"
#include "ct_tools/ct_numerictostringconversiont.h"
#include "ct_iterator/ct_pointiterator.h"

PB_ASCRGBExporter::PB_ASCRGBExporter() : CT_AbstractExporterPointAttributesSelection()
{
    setCanExportWithColors(true);
    setCanExportWithNormals(true);
}

PB_ASCRGBExporter::~PB_ASCRGBExporter()
{

}

QString PB_ASCRGBExporter::getExporterCustomName() const
{
    return tr("Points, ASCII(X,Y,Z,R,G,B,NX,NY,NZ)");
}

CT_StepsMenu::LevelPredefined PB_ASCRGBExporter::getExporterSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}

void PB_ASCRGBExporter::init()
{
    addNewExportFormat(FileFormat("asc", tr("Fichier asc")));

    setToolTip(tr("Exporte les points au format ASCII, avec les champs suivants :<br>"
                  "- X  : Coordonnée X<br>"
                  "- Y  : Coordonnée Y<br>"
                  "- Z  : Coordonnée Z<br>"
                  "- R  : Composante rouge<br>"
                  "- V  : Composante verte<br>"
                  "- B  : Composante Bleue<br>"
                  "- NX : Coordonnée X de la normale au point<br>"
                  "- NY : Coordonnée X de la normale au point<br>"
                  "- NZ : Coordonnée X de la normale au point<br><br>"));
}


bool PB_ASCRGBExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable*> &list)
{
    clearErrorMessage();

    QList<CT_AbstractItemDrawable*> myList;
    QListIterator<CT_AbstractItemDrawable*> it(list);

    while(it.hasNext())
    {
        CT_AbstractItemDrawable *item = it.next();

        if(dynamic_cast<CT_IAccessPointCloud*>(item) != NULL)
            myList.append(item);
    }

    if(myList.isEmpty())
    {
        setErrorMessage(tr("Aucun ItemDrawable du type CT_IAccessPointCloud"));
        return false;
    }

    return CT_AbstractExporter::setItemDrawableToExport(myList);
}

CT_AbstractExporter* PB_ASCRGBExporter::copy() const
{
    return new PB_ASCRGBExporter();
}

bool PB_ASCRGBExporter::protectedExportToFile()
{
    QFileInfo exportPathInfo = QFileInfo(exportFilePath());
    QString path = exportPathInfo.path();
    QString baseName = exportPathInfo.baseName();
    QString suffix = "asc";
    QString filePath = QString("%1/%2.%4").arg(path).arg(baseName).arg(suffix);

    QFile file(filePath);

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream txtStream(&file);

        txtStream << "X Y Z R G B NX NY NZ\n";

        CT_AbstractColorCloud *cc = createColorCloudBeforeExportToFile();
        CT_AbstractNormalCloud *nn = createNormalCloudBeforeExportToFile();

        float r = 1;
        float g = 1;
        float b = 1;

        int totalToExport = itemDrawableToExport().size();
        int nExported = 0;

        // write data
        QListIterator<CT_AbstractItemDrawable*> it(itemDrawableToExport());

        while(it.hasNext())
        {
            CT_AbstractItemDrawable *item = it.next();

            CT_PointIterator itP(dynamic_cast<CT_IAccessPointCloud*>(item)->getPointCloudIndex());

            size_t totalSize = itP.size();
            size_t i = 0;

            while(itP.hasNext())
            {
                const CT_Point &point = itP.next().currentPoint();

                txtStream << CT_NumericToStringConversionT<double>::toString(point(0)) << " ";
                txtStream << CT_NumericToStringConversionT<double>::toString(point(1)) << " ";
                txtStream << CT_NumericToStringConversionT<double>::toString(point(2));

                if(cc == NULL)
                {
                    txtStream << " 0 0 0";
                }
                else
                {
                    const CT_Color &co = cc->constColorAt(itP.cIndex());
                    r = (quint16)co.r() / 255.0;
                    g = (quint16)co.g() / 255.0;
                    b = (quint16)co.b() / 255.0;

                    txtStream << " " << r << " ";
                    txtStream << g << " ";
                    txtStream << b;
                }

                if(nn == NULL)
                {
                    txtStream << " 0 0 0\n";
                }
                else
                {
                    const CT_Normal &no = nn->constNormalAt(itP.cIndex());

                    txtStream << " " << no[0] << " ";
                    txtStream << no[1] << " ";
                    txtStream << no[2] << "\n";
                }

                ++i;

                setExportProgress((((i*100)/totalSize)+nExported)/totalToExport);
            }

            nExported += 100;
        }

        file.close();
        return true;
    }

    return false;
}
