#ifndef CT_READER_ASCRGB_H
#define CT_READER_ASCRGB_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"

#define DEF_CT_Reader_ASCRGB_sceneOut    "sce"
#define DEF_CT_Reader_ASCRGB_colorOut    "co"

class PLUGINSHAREDSHARED_EXPORT CT_Reader_ASCRGB : public CT_AbstractReader
{
public:
    CT_Reader_ASCRGB();

    bool setFilePath(const QString &filepath);
    void setRadiusFilter(const float &radius);

    CT_AbstractReader* copy() const;

private:
    float   m_filterRadius;

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();
};

#endif // CT_READER_ASCRGB_H
