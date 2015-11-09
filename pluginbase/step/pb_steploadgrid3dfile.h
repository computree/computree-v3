#ifndef PB_STEPLOADGRID3DFILE_H
#define PB_STEPLOADGRID3DFILE_H

#include "ct_step/abstract/ct_abstractsteploadfileinscene.h"

#include "ct_view/ct_stepconfigurabledialog.h"

class PB_StepLoadGrid3dFile : public CT_AbstractStepLoadFileInScene
{
    Q_OBJECT

public:
    PB_StepLoadGrid3dFile(CT_StepInitializeData &data);

    virtual void init();

    QString getStepDescription() const;

    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);
    QList<FileFormat> getFileExtensionAccepted() const;

protected:
    void createOutResultModelListProtected();

    int readHeaderFile(QFile &f);
    void readDataFile(QFile &f, int offset, bool little_endian = false);

private :
    size_t  _nCols;
    size_t  _nRows;
    size_t  _nZlev;
    double   _minX;
    double   _minY;
    double   _minZ;
    double   _cellSize;
    double   _noDataVal;
};

#endif // PB_STEPLOADGRID3DFILE_H
