#ifndef CT_READER_XYB_H
#define CT_READER_XYB_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_itemdrawable/ct_scene.h"

#include "ctlibio/ctlibio_global.h"
#include "ct_reader_xyb_def_models.h"
#include "headers/ct_xybheader.h"

/**
 * @brief Reader that can load a xyb file (*.xyb) that represent a PointCloud
 */
class CTLIBIO_EXPORT CT_Reader_XYB : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_XYB();

    /**
     * @brief Returns a displayable name of the reader
     */
    QString GetReaderName() const;

    /**
     * @brief Returns the sub menu level where we can store this reader
     */
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    /**
     * @brief Set the filepath to the reader. The file will be opened to check is validity.
     */
    bool setFilePath(const QString &filepath);

    /**
     * @brief Configure the reader
     */
    bool configure();

    CT_FileHeader* createHeaderPrototype() const;

    /**
     * @brief Set the radius if you want to filter points
     */
    void setFilterRadius(const double &radius);
    void setFilterRadius(const double &radius, const double &zmin, const double &zmax);

    /**
     * @brief Returns the filter radius
     */
    double filterRadius() const;

    SettingsNodeGroup* getAllSettings() const;
    bool setAllSettings(const SettingsNodeGroup *settings);

    CT_AbstractReader* copy() const;
    READER_COPY_FULL_IMP(CT_Reader_XYB)

private:
    struct Params {
        int                 _offset;
        Eigen::Vector3d     m_center;
        int                 _rows;
        int                 _cols;
    };

    Params  m_current;
    float   _filterRadius;
    double   _zminFilter;
    double   _zmaxFilter;

    bool isInsideRadius(const CT_Point &point);

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();
    CT_FileHeader* protectedReadHeader(const QString &filepath, QString &error) const;

};

#endif // CT_READER_XYB_H
