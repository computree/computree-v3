#include "ct_reader_terrascanprj.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include <QDebug>

#include "ct_global/ct_context.h"

#include <limits>

CT_Reader_TerraScanPrj::CT_Reader_TerraScanPrj() : CT_AbstractReader()
{
}

bool CT_Reader_TerraScanPrj::setFilePath(const QString &filepath)
{
    m_filePath = filepath;

    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&f);

            QString line = stream.readLine();
            if (line == "[TerraScan project]")
            {
                    f.close();
                    return true;
            }
            f.close();
        }
    }

    return false;
}

CT_AbstractReader* CT_Reader_TerraScanPrj::copy() const
{
    return new CT_Reader_TerraScanPrj();
}

void CT_Reader_TerraScanPrj::protectedInit()
{
    addNewReadableFormat(FileFormat("prj", tr("Fichier TerraScan PRJ")));
}

void CT_Reader_TerraScanPrj::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    CT_OutStdGroupModel *grp = new CT_OutStdGroupModel(DEF_CT_Reader_TERRASCANPRJ_grpOut, new CT_StandardItemGroup(), tr("Dalle"));

    _headerModel = new CT_OutStdSingularItemModel(DEF_CT_Reader_TERRASCANPRJ_headerOut, new CT_FileHeader(), tr("Fichier de la dalle"));
    _boxModel = new CT_OutStdSingularItemModel(DEF_CT_Reader_TERRASCANPRJ_boxOut, new CT_Box2D(), tr("Emprise de la dalle"));

    grp->addItem(_headerModel);
    grp->addItem(_boxModel);
    addOutGroupModel(grp);
}

bool CT_Reader_TerraScanPrj::protectedReadFile()
{
    // Test File validity
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&f);
            QString line;

            int file = -1;
            QString fileName = "";
            Eigen::Vector2d min, max;

            while(!stream.atEnd() && !isStopped())
            {
                line = stream.readLine();
                QStringList values = line.split(" ", QString::SkipEmptyParts);

                if (values.size() > 1 && values.at(0) == "Block" && !values.at(1).isEmpty())
                {
                    file = 0;
                    min(0) = std::numeric_limits<double>::max();
                    min(1) = std::numeric_limits<double>::max();
                    max(0) = -std::numeric_limits<double>::max();
                    max(1) = -std::numeric_limits<double>::max();
                    fileName = values.at(1);

                } else if (file >= 0)
                {
                    if (values.size() == 2)
                    {
                        bool okX, okY;
                        double x = values.at(0).toDouble(&okX);
                        double y = values.at(1).toDouble(&okY);

                        if (okX && okY)
                        {
                            file++;
                            if (x < min(0)) {min(0) = x;}
                            if (y < min(1)) {min(1) = y;}
                            if (x > max(0)) {max(0) = x;}
                            if (y > max(1)) {max(1) = y;}
                        }
                    }  else {
                        file = -1;
                    }

                    if (file == 5)
                    {
                        file = -1;

                        CT_StandardItemGroup *group = new CT_StandardItemGroup(NULL, NULL);

                        CT_FileHeader *header = new CT_FileHeader(_headerModel, NULL);
                        header->setFile(fileName);

                        CT_Box2DData *data = new CT_Box2DData(min, max);
                        CT_Box2D *box = new CT_Box2D(_boxModel, NULL, data);

                        group->addItemDrawable(header);
                        group->addItemDrawable(box);

                        addOutGroup(DEF_CT_Reader_TERRASCANPRJ_grpOut, group);
                    }
                }
            }

            f.close();

            return true;
        }
    }

    return false;
}
