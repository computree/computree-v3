#include "ct_reader_gdal.h"

#ifdef USE_GDAL
#include "gdal.h"
#endif

#include "ct_itemdrawable/ct_grid2dxy.h"

CT_Reader_GDAL::CT_Reader_GDAL()
{
}

#ifdef USE_GDAL
CT_Reader_GDAL::CT_Reader_GDAL(const GDALDriver *driver)
{
    m_driver = (GDALDriver*)driver;
}
#endif

CT_Reader_GDAL::~CT_Reader_GDAL()
{
}

bool CT_Reader_GDAL::setFilePath(const QString &filepath)
{
    #ifdef USE_GDAL
    if(m_driver != NULL) {
        if(canBeOpened(filepath))
            return CT_AbstractReader::setFilePath(filepath);
    }
    #endif

    return false;
}

CT_AbstractReader* CT_Reader_GDAL::copy() const
{
    #ifdef USE_GDAL
    return new CT_Reader_GDAL(m_driver);
    #else
    return new CT_Reader_GDAL();
    #endif
}

void CT_Reader_GDAL::protectedInit()
{
    #ifdef USE_GDAL
    if(m_driver != NULL) {
        QStringList ext = QString(m_driver->GetMetadataItem(GDAL_DMD_EXTENSION)).split("/");
        QString name = QString(m_driver->GetMetadataItem(GDAL_DMD_LONGNAME));
        name.remove(QRegExp("\\(\\..*\\)"));

        addNewReadableFormat(FileFormat(ext, name));
    }
    #endif
}

void CT_Reader_GDAL::protectedCreateOutItemDrawableModelList()
{
    #ifdef USE_GDAL
    GDALDataset *data = getDataSet(filepath());

    if(data != NULL) {
        int n = data->GetRasterCount();

        for(int i=0; i<n; ++i) {
            QString name = GDALGetColorInterpretationName(data->GetRasterBand(i+1)->GetColorInterpretation());
            addOutItemDrawableModel(new CT_OutStdSingularItemModel(QString("r%1").arg(i), new CT_Grid2DXY<float>(), (name.isEmpty() || (name == "Undefined")) ? QString("Raster %1").arg(i) : name));
        }

        GDALClose(data);
    }
    #endif
}

bool CT_Reader_GDAL::protectedReadFile()
{
    #ifdef USE_GDAL

    GDALDataset *data = getDataSet(filepath());

    if(data == NULL)
        return false;

    int count = data->GetRasterCount();

    for(int i=0; i<count; ++i) {

        GDALRasterBand *poBand = data->GetRasterBand(i+1);

        int nXSize = poBand->GetXSize();
        int nYSize = poBand->GetYSize();

        CT_Grid2DXY<float> *grid = new CT_Grid2DXY<float>(NULL,
                                   NULL,
                                   0,
                                   0,
                                   nXSize,
                                   nYSize,
                                   1,
                                   1,
                                   -1,
                                   0);

        float *pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);

        for(int y=0; y<nYSize; ++y) {
            poBand->RasterIO( GF_Read, 0, y, nXSize, 1, pafScanline, nXSize, 1, GDT_Float32, 0, 0 );

            for(int x=0; x<nXSize; ++x) {
                grid->setValueAtXY(x, y, pafScanline[x]);
            }
        }

        CPLFree(pafScanline);

        grid->computeMinMax();

        addOutItemDrawable(QString("r%1").arg(i), grid);
    }

    GDALClose(data);

    return true;
    #endif

    return false;
}

#ifdef USE_GDAL
bool CT_Reader_GDAL::canBeOpened(const QString &filepath) const
{
    GDALDataset *d = getDataSet(filepath);

    if(d == NULL)
        return false;

    GDALClose(d);
    return true;
}

GDALDataset* CT_Reader_GDAL::getDataSet(const QString &filepath) const
{
    GDALDataset *poDataset = (GDALDataset *) GDALOpen( filepath.toStdString().data(), GA_ReadOnly );

    if((poDataset != NULL) && (poDataset->GetDriver() == m_driver))
        return poDataset;

    if(poDataset != NULL)
        GDALClose(poDataset);

    return NULL;
}
#endif
