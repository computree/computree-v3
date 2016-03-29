#ifndef CT_READER_IDXYZ__H
#define CT_READER_IDXYZ__H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_scene.h"

#include "ctlibio/ctlibio_global.h"
#include "ct_reader_idxyz_def_models.h"

class CTLIBIO_EXPORT CT_Reader_IDXYZ : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_IDXYZ();

    virtual QString GetReaderName() const;

    virtual CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    bool setFilePath(const QString &filepath);

    CT_AbstractReader* copy() const;
    READER_COPY_FULL_IMP(CT_Reader_IDXYZ)


protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

private:
    CT_OutStdSingularItemModel* _clusterSceneModel;

};

#endif // CT_READER_IDXYZ__H
