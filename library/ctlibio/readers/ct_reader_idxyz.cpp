#include "ct_reader_idxyz.h"
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include <QDebug>

#include "ct_global/ct_context.h"
#include "ct_point.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

#include <limits>

CT_Reader_IDXYZ::CT_Reader_IDXYZ() : CT_AbstractReader()
{
}

QString CT_Reader_IDXYZ::GetReaderName() const
{
    return tr("Clusters de Points, fichier ASCII (ID, X, Y, Z)");
}

CT_StepsMenu::LevelPredefined CT_Reader_IDXYZ::getReaderSubMenuName() const
{
    return CT_StepsMenu::LP_Points;
}

bool CT_Reader_IDXYZ::setFilePath(const QString &filepath)
{
    // Test File validity
    if(QFile::exists(filepath))
    {
        QFile f(filepath);

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&f);

            stream.readLine();
            QString line = stream.readLine();
            if (!line.isNull())
            {
                QStringList values = line.split("\t");
                if (values.size() >= 4) {
                    f.close();
                    return CT_AbstractReader::setFilePath(filepath);
                } else {
                    PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 n'a pas le bon format (colonnes manquantes).")).arg(filepath));
                }
            } else {
                PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 est vide.")).arg(filepath));
            }

            f.close();
        } else {
            PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 n'est pas accessible.")).arg(filepath));
        }
    } else {
        PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 n'existe pas.")).arg(filepath));
    }

    return false;
}

CT_AbstractReader* CT_Reader_IDXYZ::copy() const
{
    return new CT_Reader_IDXYZ();
}

void CT_Reader_IDXYZ::protectedInit()
{
    addNewReadableFormat(FileFormat("asc", tr("Fichiers de points ASCII (XYZRGB, sans entête, RGB [0;1])")));

    setToolTip(tr("Charge un fichier de clusters de points au format ASCII, avec les champs suivants :<br>"
                  "- ID : ID du cluster<br>"
                  "- X  : Coordonnée X<br>"
                  "- Y  : Coordonnée Y<br>"
                  "- Z  : Coordonnée Z<br>"));
}

void CT_Reader_IDXYZ::protectedCreateOutItemDrawableModelList()
{
    CT_AbstractReader::protectedCreateOutItemDrawableModelList();

    addOutItemDrawableModel(DEF_CT_Reader_IDXYZ_sceneOut, new CT_Scene(), tr("Scène fusionnée"));

    CT_OutStdGroupModel *grp = new CT_OutStdGroupModel(DEF_CT_Reader_IDXYZ_groupOut, new CT_StandardItemGroup(), tr("Groupe"));
    _clusterSceneModel = new CT_OutStdSingularItemModel(DEF_CT_Reader_IDXYZ_sceneClusterOut, new CT_Scene(), tr("Scènes individuelles"));
    grp->addItem(_clusterSceneModel);
    addOutGroupModel(grp);

}

bool CT_Reader_IDXYZ::protectedReadFile()
{
    // Test File validity
    if(QFile::exists(filepath()))
    {
        QFile f(filepath());

        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&f);

            CT_AbstractUndefinedSizePointCloud* pointCloud = PS_REPOSITORY->createNewUndefinedSizePointCloud();

            QString line;
            CT_Point pReaded;

            // Getting the file size to set progress
            qint64 fileSize = f.size();
            qint64 currentSizeRead = 0;

            QList<size_t> indices;
            QMap<size_t, CT_PointCloudIndexVector*> indexVectors;

            stream.readLine();

            while(!stream.atEnd()
                  && !isStopped())
            {
                line = stream.readLine();
                currentSizeRead += line.size();
                QStringList values = line.split("\t", QString::SkipEmptyParts);

                if (values.size() >= 4)
                {
                    bool okID = false;
                    bool okX = false;
                    bool okY = false;
                    bool okZ = false;

                    size_t id = values.at(0).toDouble(&okID);
                    double x  = values.at(1).toDouble(&okX);
                    double y = values.at(2).toDouble(&okY);
                    double z = values.at(3).toDouble(&okZ);

                    if (okID && okX && okY && okZ)
                    {
                            pReaded(0) = x;
                            pReaded(1) = y;
                            pReaded(2) = z;

                            pointCloud->addPoint(pReaded);
                            indices.append(id);
                            if (! indexVectors.contains(id)) {indexVectors.insert(id, new CT_PointCloudIndexVector());}
                    }
                }

                setProgress((currentSizeRead*100)/fileSize);
            }

            f.close();

            if (pointCloud->size() > 0)
            {
                CT_Scene *scene = new CT_Scene(NULL, NULL, PS_REPOSITORY->registerUndefinedSizePointCloud(pointCloud));
                scene->updateBoundingBox();

                addOutItemDrawable(DEF_CT_Reader_IDXYZ_sceneOut, scene);

                const CT_AbstractPointCloudIndex* sceneIndexVector =  scene->getPointCloudIndex();

                for (int i = 0 ; i < indices.size() ; i++)
                {
                    size_t indice = indices.at(i);
                    CT_PointCloudIndexVector* indexVector = indexVectors.value(indice, NULL);

                    if (indexVector != NULL)
                    {
                        size_t pointIndex = sceneIndexVector->indexAt(i);
                        indexVector->addIndex(pointIndex);
                    }
                }

                QMapIterator<size_t, CT_PointCloudIndexVector*> it(indexVectors);
                while (it.hasNext())
                {
                    it.next();
                    size_t index = it.key();
                    CT_PointCloudIndexVector* indexVector = it.value();

                    if (indexVector != NULL)
                    {
                        if (indexVector->size() > 0)
                        {
                            CT_Scene *sceneId = new CT_Scene(_clusterSceneModel, NULL, PS_REPOSITORY->registerPointCloudIndex(indexVector));
                            sceneId->updateBoundingBox();
                            sceneId->setDisplayableName(QString("Scene_%1").arg(QVariant::fromValue(index).toString()));

                            CT_StandardItemGroup* group = new CT_StandardItemGroup(NULL, NULL);
                            group->addItemDrawable(sceneId);
                            addOutGroup(DEF_CT_Reader_IDXYZ_groupOut, group);
                        } else {
                            delete indexVector;
                        }
                    }

                }

                return true;

            } else
            {
                delete pointCloud;
            }
        } else {
            PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 n'est pas accessible.")).arg(filepath()));
        }
    } else {
        PS_LOG->addMessage(LogInterface::error, LogInterface::reader, QString(tr("Le fichier %1 n'existe pas.")).arg(filepath()));
    }

    return false;
}
