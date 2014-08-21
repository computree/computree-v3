#include "step/pb_steploadgrid3dfile.h"

#include "ct_global/ct_context.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

#include "ct_itemdrawable/ct_grid3d.h"

#include <QTextStream>
#include <QDebug>

#define DEF_Search_Grid3d "grid3d"
#define DEF_Search_Grid3dGroup "grid3dgroup"
#define DEF_Search_Grid3dResult "grid3dresult"

PB_StepLoadGrid3dFile::PB_StepLoadGrid3dFile(CT_StepInitializeData &data) : CT_AbstractStepLoadFileInScene(data)
{
}

void PB_StepLoadGrid3dFile::init()
{
    CT_AbstractStepLoadFileInScene::init();
}

QString PB_StepLoadGrid3dFile::getStepDescription() const
{
    return tr("Charge un fichier grid3d");
}

CT_VirtualAbstractStep* PB_StepLoadGrid3dFile::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepLoadGrid3dFile(dataInit);
}

QList<QString> PB_StepLoadGrid3dFile::getFileExtensionAccepted() const
{
    QList<QString> list;
    list << ".grid3d";
    list << ".grd3d";
    list << ".GRD3D";

    return list;
}

void PB_StepLoadGrid3dFile::createOutResultModelListProtected()
{
    CT_OutStdGroupModel* gridGroup = new CT_OutStdGroupModel(DEF_Search_Grid3dGroup);
    CT_OutStdSingularItemModel* grid = new CT_OutStdSingularItemModel(DEF_Search_Grid3d, new CT_Grid3D<float>());

    gridGroup->addItem(grid);

    addOutResultModel(new CT_OutResultModelGroup(DEF_Search_Grid3dResult, gridGroup, "CT_Grid3D"));
}

int PB_StepLoadGrid3dFile::readHeaderFile(QFile &f)
{
    // Outils de lecture
    QString     currentLine;
    QStringList splitline;
    bool*       read = new bool();

    // Initialise les parametres de la grille
    currentLine = f.readLine();
    splitline = currentLine.split('\t');
    _nCols = splitline.at(1).toInt(read);
    if( !read )
    {
        setErrorCode(2);
        setErrorMessage(2, QString(QString("Unable to convert the ascii format to an integer at line 1")));
        PS_LOG->addMessage( LogInterface::error, LogInterface::reader, tr("Unable to convert the ascii format to a integer at line 1") );
        qDebug() << QString(QString("Unable to convert the ascii format to a integer at line 1"));
    }

    currentLine = f.readLine();
    splitline = currentLine.split('\t');
    _nRows = splitline.at(1).toInt(read);
    if( !read )
    {
        setErrorCode(2);
        setErrorMessage(2, QString(QString("Unable to convert the ascii format to an integer at line 2")));
        PS_LOG->addMessage( LogInterface::error, LogInterface::reader, tr("Unable to convert the ascii format to a integer at line 2") );
        qDebug() << QString(QString("Unable to convert the ascii format to a integer at line 2"));
    }

    currentLine = f.readLine();
    splitline = currentLine.split('\t');
    _nZlev = splitline.at(1).toInt(read);
    if( !read )
    {
        setErrorCode(2);
        setErrorMessage(2, QString(QString("Unable to convert the ascii format to an integer at line 3")));
        PS_LOG->addMessage( LogInterface::error, LogInterface::reader, tr("Unable to convert the ascii format to a integer at line 3") );
        qDebug() << QString(QString("Unable to convert the ascii format to a integer at line 3"));
    }

    currentLine = f.readLine();
    splitline = currentLine.split('\t');
    _minX = splitline.at(1).toFloat(read);
    if( !read )
    {
        setErrorCode(2);
        setErrorMessage(2, QString(QString("Unable to convert the ascii format to a floatting point at line 4")));
        PS_LOG->addMessage( LogInterface::error, LogInterface::reader, tr("Unable to convert the ascii format to a floatting point at line 4") );
        qDebug() << QString(QString("Unable to convert the ascii format to a floatting point at line 4"));
    }

    currentLine = f.readLine();
    splitline = currentLine.split('\t');
    _minY = splitline.at(1).toFloat(read);
    if( !read )
    {
        setErrorCode(2);
        setErrorMessage(2, QString(QString("Unable to convert the ascii format to a floatting point at line 5")));
        PS_LOG->addMessage( LogInterface::error, LogInterface::reader, tr("Unable to convert the ascii format to a floatting point at line 5") );
        qDebug() << QString(QString("Unable to convert the ascii format to a floatting point at line 5"));
    }

    currentLine = f.readLine();
    splitline = currentLine.split('\t');
    _minZ = splitline.at(1).toFloat(read);
    if( !read )
    {
        setErrorCode(2);
        setErrorMessage(2, QString(QString("Unable to convert the ascii format to a floatting point at line 6")));
        PS_LOG->addMessage( LogInterface::error, LogInterface::reader, tr("Unable to convert the ascii format to a floatting point at line 6") );
        qDebug() << QString(QString("Unable to convert the ascii format to a floatting point at line 6"));
    }

    currentLine = f.readLine();
    splitline = currentLine.split('\t');
    _cellSize = splitline.at(1).toFloat(read);
    if( !read )
    {
        setErrorCode(2);
        setErrorMessage(2, QString(QString("Unable to convert the ascii format to a floatting point at line 7")));
        PS_LOG->addMessage( LogInterface::error, LogInterface::reader, tr("Unable to convert the ascii format to a floatting point at line 7") );
        qDebug() << QString(QString("Unable to convert the ascii format to a floatting point at line 7"));
    }

    currentLine = f.readLine();
    splitline = currentLine.split('\t');
    _noDataVal = splitline.at(1).toFloat(read);
    if( !read )
    {
        setErrorCode(2);
        setErrorMessage(2, QString(QString("Unable to convert the ascii format to a floatting point at line 8")));
        PS_LOG->addMessage( LogInterface::error, LogInterface::reader, tr("Unable to convert the ascii format to a floatting point at line 8") );
        qDebug() << QString(QString("Unable to convert the ascii format to a floatting point at line 8"));
    }

    return 0;
}

void PB_StepLoadGrid3dFile::readDataFile(QFile &f, int offset, bool little_endian)
{
    // Get the output result
    CT_ResultGroup* out_res = getOutResultList().first();

    // Creating a grid with the parameters read from the header
    CT_Grid3D<float>* loadedGrid = new CT_Grid3D<float>( getOutSingularItemModelForCreation(out_res, DEF_Search_Grid3d),
                                                         out_res,
                                                         _minX,
                                                         _minY,
                                                         _minZ,
                                                         _nCols,
                                                         _nRows,
                                                         _nZlev,
                                                         _cellSize,
                                                         _noDataVal,
                                                         0);

    QTextStream fileStream(&f);
    float currentValue;
    for ( int i = 0 ; i < _nZlev ; i++ )
    {
        for ( int j = _nRows - 1 ; j >= 0 ; j-- )
        {
            for ( int k = 0 ; k < _nCols ; k++ )
            {
                fileStream >> currentValue;
                loadedGrid->setValue( k, j, i, currentValue );
            }
        }
    }

    loadedGrid->computeMinMax();

    CT_StandardItemGroup* gridGroup = new CT_StandardItemGroup(getOutGroupModelForCreation(out_res, DEF_Search_Grid3dGroup), out_res);
    gridGroup->addItemDrawable( loadedGrid );
    out_res->addGroup( gridGroup );
}
