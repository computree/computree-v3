#include "ct_abstractstepserializable.h"

#include "ct_inresultmodelnotneedinputresult.h"

#include <QFile>
#include <QDir>

CT_AbstractStepSerializable::CT_AbstractStepSerializable(CT_StepInitializeData &dataInit) : CT_VirtualAbstractStep(dataInit)
{
}

CT_AbstractStepSerializable::~CT_AbstractStepSerializable()
{
}

bool CT_AbstractStepSerializable::serialize(const QString &dirPath, bool clearResultAfter)
{
#ifdef USE_BOOST_OLD

    bool ok;

    // on sauvegarde (et dtruit de la mmoire si
    // l'option est active) les rsultats
    #ifdef USE_BOOST_BINARY
        ok = writeResult<boost::archive::binary_oarchive>(dirPath, clearResultAfter);
    #else
        ok = writeResult<boost::archive::text_oarchive>(dirPath, clearResultAfter);
    #endif

    return ok;
#else
    Q_UNUSED(dirPath)
    Q_UNUSED(clearResultAfter)
    return false;
#endif
}

bool CT_AbstractStepSerializable::canBeDeserialized(const QString &dirPath) const
{
#ifdef USE_BOOST_OLD
    int n = getNumberResultSerialized();

    if((n <= 0)
            || !(QFile::exists(QString("%1/%2").arg(dirPath).arg(getRelativeFilepathOfSerializedResults()))))
    {
        return false;
    }

    QList<CT_VirtualAbstractStep *> stepChildList = getStepChildList();
    QListIterator<CT_VirtualAbstractStep*> it(stepChildList);

    while(it.hasNext())
    {
        CT_VirtualAbstractStep *nextStep = dynamic_cast<CT_VirtualAbstractStep*>(it.next());

        if(nextStep == NULL)
        {
            return false;
        }

        if(!nextStep->recursiveCanBeDeserialized(dirPath))
        {
            return false;
        }
    }

    return true;
#else
    Q_UNUSED(dirPath)
    return false;
#endif
}

bool CT_AbstractStepSerializable::deserialize(const QString &dirPath)
{
#ifdef USE_BOOST_OLD

    bool ok;

    #ifdef USE_BOOST_BINARY
        std::ifstream ifile(QString("%1/%2").arg(dirPath).arg(getRelativeFilepathOfSerializedResults()).toLatin1(), std::ios_base::binary);
        boost::archive::binary_iarchive archive(ifile);

        ok = recursiveLoadSerializedResult(archive);
    #else
        std::ifstream ifile(QString("%1/%2").arg(dirPath).arg(getRelativeFilepathOfSerializedResults()).toLatin1());
        boost::archive::text_iarchive archive(ifile);

        ok = recursiveLoadSerializedResult(archive);
    #endif

    return ok;
#else
    Q_UNUSED(dirPath)
    return false;
#endif
}

void CT_AbstractStepSerializable::createInResultModelListProtected()
{
    // pas besoin de rsultat en entre
    addInResultModel(new CT_InResultModelNotNeedInputResult());
}

#ifdef USE_BOOST_OLD

template <class Archive>
bool CT_AbstractStepSerializable::writeResult(QString dirPath,
                                              bool clearResultAfter)
{
    bool ok = false;

    // Si le dossier n'existe pas
    if(!QFile::exists(dirPath))
    {
        // on le cre
        QDir dir("/");
        dir.mkdir(dirPath);
    }

    // si il a bien t cr
    if(QFile::exists(dirPath))
    {
        // on recherche un sous dossier disponible
        int nDir = 0;

        while(QFile::exists((QString("%1/%2").arg(dirPath)).arg(nDir)))
        {
            ++nDir;
        }

        QString dirToMake = QString("%1").arg(nDir);

        QDir directory(dirPath);
        directory.mkdir(dirToMake);

        #ifdef USE_BOOST_BINARY
            QString filename = "save.bin";
        #else
            QString filename = "save.txt";
        #endif

        QString filePath = QString("%1/%2/%3").arg(dirPath).arg(nDir).arg(filename);
        QString filePathRelative = QString("%1/%2").arg(nDir).arg(filename);

        // cration du fichier de sauvegarde
        #ifdef USE_BOOST_BINARY
            std::ofstream *ofile = new std::ofstream(filePath.toUtf8().data(), std::ios_base::binary);
        #else
            std::ofstream *ofile = new std::ofstream(filePath.toUtf8().data());
        #endif

        if(ofile->is_open())
        {
            Archive *archive = new Archive(*ofile);

            // on sauvegarde les rsultats dans le fichier
            ok = recursiveSerializeResult(dirPath, filePathRelative, *archive, clearResultAfter);

            ofile->close();

            delete archive;
            delete ofile;
        }
        else
        {
            delete ofile;
        }
    }

    return ok;
}

#endif
