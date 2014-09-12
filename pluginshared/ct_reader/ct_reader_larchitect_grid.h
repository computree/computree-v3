#ifndef CT_READER_LARCHITECT_GRID_H
#define CT_READER_LARCHITECT_GRID_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_grid3d.h"
#include <QMap>

#define DEF_CT_Reader_LArchitect_Grid_wood_surface    "ws"
#define DEF_CT_Reader_LArchitect_Grid_leaf_surface    "ls"
#define DEF_CT_Reader_LArchitect_Grid_wood_volume     "wv"
#define DEF_CT_Reader_LArchitect_Grid_leaf_volume     "lv"
#define DEF_CT_Reader_LArchitect_Grid_all_surface     "as"
#define DEF_CT_Reader_LArchitect_Grid_all_volume      "av"

class PLUGINSHAREDSHARED_EXPORT CT_Reader_LArchitect_Grid : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_LArchitect_Grid();

    bool setFilePath(const QString &filepath);

    inline int getOffset() {return _nbLinesToSkip;}

    CT_AbstractReader* copy() const;

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

private:
    float               _xmin;
    float               _ymin;
    float               _zmin;
    float               _xmax;
    float               _ymax;
    float               _zmax;
    float               _resx;
    float               _resy;
    float               _resz;
    int                 _nmat;
    QMap<QString, int>  _matNames;
    size_t                 _dimx;
    size_t                 _dimy;
    size_t                 _dimz;
    int                 _nbLinesToSkip;


};

#endif // CT_READER_LARCHITECT_GRID_H
