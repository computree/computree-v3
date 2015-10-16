#ifndef PB_STEPLOADPGMFILE_H
#define PB_STEPLOADPGMFILE_H

#include "ct_step/abstract/ct_abstractsteploadfileinscene.h"

/*!
 * \class PB_StepLoadPgmFile
 * \ingroup Steps_PB
 * \brief <b>Load a gray level image from a pgm file</b>
 *
 * <b>Output Models:</b>
 *
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_Grid2D<int> \n
 *
 */
class PB_StepLoadPgmFile : public CT_AbstractStepLoadFileInScene
{
    Q_OBJECT

public:
    PB_StepLoadPgmFile(CT_StepInitializeData &data);
    ~PB_StepLoadPgmFile();

    QString getStepDescription() const;

    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);
    QList<FileFormat> getFileExtensionAccepted() const;

protected:
    void createOutResultModelListProtected();

    int readHeaderFile(QFile &f);

    void readDataFile(QFile &f, int offset, bool little_endian = false);

private :
    unsigned int    _width;
    unsigned int    _height;
    int             _maxGray;
};

#endif // PB_STEPLOADPGMFILE_H
