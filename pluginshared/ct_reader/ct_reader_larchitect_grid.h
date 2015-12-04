#ifndef CT_READER_LARCHITECT_GRID_H
#define CT_READER_LARCHITECT_GRID_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_grid3d.h"
#include <QMap>

#include "ct_reader/ct_reader_larchitect_grid_def_models.h"

class PLUGINSHAREDSHARED_EXPORT CT_Reader_LArchitect_Grid : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_LArchitect_Grid();

    QString GetReaderName() const;
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    bool setFilePath(const QString &filepath);

    inline int getOffset() {return _nbLinesToSkip;}

    CT_AbstractReader* copy() const;


protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

private:
    double               _xmin;
    double               _ymin;
    double               _zmin;
    double               _xmax;
    double               _ymax;
    double               _zmax;
    double               _resx;
    double               _resy;
    double               _resz;
    int                 _nmat;
    QMap<QString, int>  _matNames;
    size_t                 _dimx;
    size_t                 _dimy;
    size_t                 _dimz;
    int                 _nbLinesToSkip;


};

#endif // CT_READER_LARCHITECT_GRID_H
