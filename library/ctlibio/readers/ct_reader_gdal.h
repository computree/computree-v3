#ifndef CT_READER_GDAL_H
#define CT_READER_GDAL_H

#include "ct_reader/abstract/ct_abstractreader.h"

#include "ctlibio/ctlibio_global.h"
#include "ct_reader_gdal_def_models.h"

#ifdef USE_GDAL
#include "gdal_priv.h"
#include "ogr_geometry.h"
#endif

/**
 * @brief Read files that gdal can read
 */
class CTLIBIO_EXPORT CT_Reader_GDAL : public CT_AbstractReader
{
    Q_OBJECT
public:
    CT_Reader_GDAL();
    CT_Reader_GDAL(const CT_Reader_GDAL &other);

    #ifdef USE_GDAL
    CT_Reader_GDAL(const GDALDriver *driver);

    GDALDriver* getDriver() const { return m_driver; }
    #endif

    virtual ~CT_Reader_GDAL();

    virtual bool setFilePath(const QString &filepath);


    virtual QString GetReaderName() const;

    virtual QString GetReaderClassName() const;

    virtual CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    QString getTypeOfDriver() const;

    virtual CT_AbstractReader* copy() const;
    READER_COPY_FULL_IMP(CT_Reader_GDAL)

protected:

    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

#ifdef USE_GDAL
private:
    GDALDriver                                      *m_driver;
    QHash<QString, CT_OutStdSingularItemModel*>     m_models;
    QString                                         m_nameFromDriver;

    /**
     * @brief Return true if the file can be opened by this driver
     */
    bool canBeOpened(const QString &filepath) const;

    /**
     * @brief Get the dataset from the file.
     * @return NULL if the file can not be read or is was an error
     */
    GDALDataset* getDataSet(const QString &filepath) const;

    /**
     * @brief Convert the poGeometry to a CT_XXX object and add it to a new group for the layer 'layerIndex'
     */
    void convertGeometryToCT(OGRGeometry *poGeometry, int layerIndex);

    /**
     * @brief Update the name from the driver
     */
    void updateNameFromDriver();
#endif
};

#endif // CT_READER_GDAL_H
