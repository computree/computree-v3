#ifndef CT_READER_GDAL_H
#define CT_READER_GDAL_H

#include "ct_reader/abstract/ct_abstractreader.h"

#ifdef USE_GDAL
#include "gdal_priv.h"
#endif

/**
 * @brief Read files that gdal can read
 */
class PLUGINSHAREDSHARED_EXPORT CT_Reader_GDAL : public CT_AbstractReader
{
public:
    CT_Reader_GDAL();

    #ifdef USE_GDAL
    CT_Reader_GDAL(const GDALDriver *driver);
    #endif

    virtual ~CT_Reader_GDAL();

    virtual bool setFilePath(const QString &filepath);

    virtual CT_AbstractReader* copy() const;

protected:

    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

#ifdef USE_GDAL
private:
    GDALDriver *m_driver;

    /**
     * @brief Return true if the file can be opened by this driver
     */
    bool canBeOpened(const QString &filepath) const;

    /**
     * @brief Get the dataset from the file.
     * @return NULL if the file can not be read or is was an error
     */
    GDALDataset* getDataSet(const QString &filepath) const;
#endif
};

#endif // CT_READER_GDAL_H
