#ifndef DM_OSGUNIFORMWITHSLOT_H
#define DM_OSGUNIFORMWITHSLOT_H

#include <osg/Uniform>
#include <QColor>
#include <QObject>

class DM_OsgUniformWithSlot : public QObject, public osg::Uniform
{
    Q_OBJECT

public:
    DM_OsgUniformWithSlot(const char *name, float f);
    DM_OsgUniformWithSlot(const char *name, const osg::Vec4 &v4);

public slots:
    void setBySlot(float f);
    void setBySlot(const osg::Vec4 &v4);
    void setBySlot(const QColor &c);
};

#endif // DM_OSGUNIFORMWITHSLOT_H
