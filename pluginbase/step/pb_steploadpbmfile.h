#ifndef PB_STEPLOADPBMFILE_H
#define PB_STEPLOADPBMFILE_H

#include "ct_step/abstract/ct_abstractsteploadfileinscene.h"

/*!
 * \class PB_StepLoadPbmFile
 * \ingroup Steps_PB
 * \brief <b>Load a binary image from a pbm file</b>
 *
 * <b>Output Models:</b>
 *
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_Grid2D<int> \n
 *
 */
class PB_StepLoadPbmFile : public CT_AbstractStepLoadFileInScene
{
    Q_OBJECT

public:
    PB_StepLoadPbmFile(CT_StepInitializeData &data);
    ~PB_StepLoadPbmFile();

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
};
#endif // PB_STEPLOADPBMFILE_H
