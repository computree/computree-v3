#include "pb_steploadpgmfile.h"

// Reading into files with qtextstream
#include <QTextStream>

// Using integher limit as NA value
#include <limits>

// Output is a 2d grid
#include "ct_itemdrawable/ct_grid2dxy.h"

// Output models
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

#define DEF_Grid2dItem "grid2ditem"
#define DEF_Grid2dGroup "grid2dgroup"
#define DEF_Grid2dResult "grid2dresult"

PB_StepLoadPgmFile::PB_StepLoadPgmFile(CT_StepInitializeData &data) : CT_AbstractStepLoadFileInScene(data)
{
    _width = 0;
    _height = 0;
    _maxGray = 0;
}

PB_StepLoadPgmFile::~PB_StepLoadPgmFile()
{
}

QString PB_StepLoadPgmFile::getStepDescription() const
{
    return tr("Fichier PGM");
}

CT_VirtualAbstractStep* PB_StepLoadPgmFile::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepLoadPgmFile(dataInit);
}

QList<FileFormat> PB_StepLoadPgmFile::getFileExtensionAccepted() const
{
    QList<FileFormat> list;
    list << FileFormat("pgm", tr("PGM Files"));

    return list;
}

void PB_StepLoadPgmFile::createOutResultModelListProtected()
{    
    CT_OutResultModelGroup *outRes = createNewOutResultModel(DEF_Grid2dResult, "CT_Grid2D");
    outRes->setRootGroup(DEF_Grid2dGroup);
    outRes->addItemModel(DEF_Grid2dGroup, DEF_Grid2dItem, new CT_Grid2DXY<int>());
}

int PB_StepLoadPgmFile::readHeaderFile(QFile &f)
{
    QTextStream stream ( &f );
    QString     currentLine;
    QStringList wordsOfLine;

    int currentSizeRead = 0;

    if ( !stream.atEnd()
         && !isStopped()
         && (getErrorCode() == 0) )
    {
        currentLine = stream.readLine();
        currentSizeRead += currentLine.size();
    }

    if(!currentLine.isEmpty())
    {
        // Read each word separately
        wordsOfLine = currentLine.split( QRegExp("\\s+"), QString::SkipEmptyParts );

        // Checking for a valid line
        if( wordsOfLine.size() != 1 || wordsOfLine.at(0) != "P2" )
        {
            setErrorCode(1);
            setErrorMessage(1, "Error while loading the file " + f.fileName() + " : first line should only contain magic number P2");
        }
    }

    // Skip comments
    do
    {
        // Read a new line
        currentLine = stream.readLine();
        currentSizeRead += currentLine.size();

        // Read each word separately
        wordsOfLine = currentLine.split( QRegExp("\\s+"), QString::SkipEmptyParts );
    }
    while( !stream.atEnd() && !isStopped() && (getErrorCode() == 0) && wordsOfLine.at(0) == "#" );

    // Read the image size
    _width = wordsOfLine.at(0).toUInt();
    _height = wordsOfLine.at(1).toUInt();

    // Skip comments
    do
    {
        // Read a new line
        currentLine = stream.readLine();
        currentSizeRead += currentLine.size();

        // Read each word separately
        wordsOfLine = currentLine.split( QRegExp("\\s+"), QString::SkipEmptyParts );
    }
    while( !stream.atEnd() && !isStopped() && (getErrorCode() == 0) && wordsOfLine.at(0) == "#" );

    // Read the maximum gray level
    _maxGray = wordsOfLine.at(0).toInt();

    // Why +4 works ?
    // +1 for \0 char
    // +1 for skipping the next \n char
    // +1 for positioning the reader
    // +1 for what ?!
    return currentSizeRead + 4;
}

void PB_StepLoadPgmFile::readDataFile(QFile &f, int offset, bool little_endian)
{
    Q_UNUSED(little_endian)

    // Get the output result
    CT_ResultGroup* out_res = getOutResultList().first();

    // Skip offset
    f.seek( offset );


    // Creating a 2d grid
    CT_Grid2DXY<int>* outputImage = new CT_Grid2DXY<int>(DEF_Grid2dItem, out_res, /*-_width/2, -_height/2,*/ 0,0, _width, _height, 1, 0, std::numeric_limits<int>::max(), 0 );

    QTextStream stream ( &f );
    QString currentvalue;
    for ( unsigned int i = 0 ; i < _height ; i++ )
    {
        for ( unsigned int j = 0 ; j < _width ; j++ )
        {
            stream >> currentvalue;

            if ( stream.atEnd() )
            {
                setErrorCode(2);
                setErrorMessage(2, "Error while loading the file " + f.fileName() + " incomplete image");
            }

            else
            {
                outputImage->setValue( j, _height - (i+1), currentvalue.toInt() );
            }
        }
    }

    outputImage->computeMinMax();

    CT_StandardItemGroup* gridGroup = new CT_StandardItemGroup(DEF_Grid2dGroup, out_res);
    gridGroup->addItemDrawable( outputImage );
    out_res->addGroup( gridGroup );
}
