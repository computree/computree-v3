#ifndef CT_READER_XYB_H
#define CT_READER_XYB_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_itemdrawable/ct_scene.h"

#define DEF_CT_Reader_XYB_sceneOut     "sce"
#define DEF_CT_Reader_XYB_intensityOut "int"
#define DEF_CT_Reader_XYB_scannerOut   "sca"

class PLUGINSHAREDSHARED_EXPORT CT_Reader_XYB : public CT_AbstractReader
{
public:
    CT_Reader_XYB();

    bool setFilePath(const QString &filepath);
    void setRadiusFilter(const float &radius);

    CT_AbstractReader* copy() const;

private:
    int      _offset;
    double   _xc;
    double   _yc;
    double   _zc;
    int      _rows;
    int      _cols;
    float   _filterRadius;

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();
};

#endif // CT_READER_XYB_H
