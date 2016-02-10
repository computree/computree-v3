#ifndef CT_READER_TERRASCANPRJ_H
#define CT_READER_TERRASCANPRJ_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_box2d.h"

#include "ctlibio/ctlibio_global.h"
#include "ct_reader_terrascanprj_def_models.h"

class CTLIBIO_EXPORT CT_Reader_TerraScanPrj : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_TerraScanPrj();

    QString GetReaderName() const;
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    bool setFilePath(const QString &filepath);

    CT_AbstractReader* copy() const;

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

private:
    CT_OutStdSingularItemModel* _headerModel;
    CT_OutStdSingularItemModel* _boxModel;
};

#endif // CT_READER_TERRASCANPRJ_H
