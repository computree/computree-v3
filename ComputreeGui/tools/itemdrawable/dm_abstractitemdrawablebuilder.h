#ifndef DM_ABSTRACTITEMDRAWABLEBUILDER_H
#define DM_ABSTRACTITEMDRAWABLEBUILDER_H

#include <QObject>

class DM_AbstractItemDrawableBuilder : public QObject
{
    Q_OBJECT

public:
    virtual ~DM_AbstractItemDrawableBuilder() {}

signals:
    void listChanged();
    void added();
    void removed();
};

#endif // DM_ABSTRACTITEMDRAWABLEBUILDER_H
