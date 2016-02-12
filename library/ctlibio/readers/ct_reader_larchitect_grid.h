#ifndef CT_READER_LARCHITECT_GRID_H
#define CT_READER_LARCHITECT_GRID_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_grid3d.h"
#include <QMap>

#include "ctlibio/ctlibio_global.h"
#include "ct_reader_larchitect_grid_def_models.h"

class CTLIBIO_EXPORT CT_Reader_LArchitect_Grid : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_LArchitect_Grid();

    QString GetReaderName() const;
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    bool setFilePath(const QString &filepath);

    CT_AbstractReader* copy() const;
    READER_COPY_FULL_IMP(CT_Reader_LArchitect_Grid)

protected:
    typedef Eigen::Matrix<size_t, 3, 1> EigenVector3ui64;


    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

    bool readHeader(QTextStream &stream, Eigen::Vector3d &min, Eigen::Vector3d &max, Eigen::Vector3d &res, EigenVector3ui64 &dim, int &nMat, QMap<QString, int>  &matNames) const;
    CT_Grid3D<float>* createGrid(Eigen::Vector3d &min, Eigen::Vector3d &res, EigenVector3ui64 &dim) const;

private:


};

#endif // CT_READER_LARCHITECT_GRID_H
