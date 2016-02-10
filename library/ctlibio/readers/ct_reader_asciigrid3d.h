#ifndef CT_READER_ASCIIGRID3D_H
#define CT_READER_ASCIIGRID3D_H

#include "ct_reader/abstract/ct_abstractreader.h"

#include "ctlibio/ctlibio_global.h"
#include "ct_reader_asciigrid3d_def_models.h"

/**
 * @brief Reader that can load ascii file that represent Grid 3D (show AsciiGrid3D exporter) (*.grid3d *.grd3d)
 */
class CTLIBIO_EXPORT CT_Reader_AsciiGrid3D : public CT_AbstractReader
{
public:
    CT_Reader_AsciiGrid3D();

    /**
     * @brief Returns a displayable name of the reader
     */
    QString GetReaderName() const;

    /**
     * @brief Returns
     */
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    /**
     * @brief Set the filepath to the reader. The file will be opened to search faces, if no face was found
     *        the reader will load a PointCloud otherwise it will load a Mesh.
     */
    bool setFilePath(const QString &filepath);

    CT_AbstractReader* copy() const;

private:

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

    bool readHeader(QTextStream &stream, size_t &nCols, size_t &nRows, size_t &nZLev, Eigen::Vector3d &min, double &cellSize, double &noDataValue);
};

#endif // CT_READER_ASCIIGRID3D_H
