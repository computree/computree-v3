#ifndef CT_MIMEDATA_H
#define CT_MIMEDATA_H

#include <QMimeData>
#include <qstandarditemmodel.h>
#include "interfaces.h"

class CT_MimeData : public QMimeData
{
public:
    CT_MimeData();

    inline bool hasItemDrawable() const {return _itemDrawable!= NULL;}
    inline bool hasItemModel() const {return _itemModel!= NULL;}

    inline void setItemDrawable (CT_AbstractItemDrawable* itemDrawable) {_itemDrawable = itemDrawable;}
    inline void setItemModel (QStandardItemModel* itemModel) {_itemModel = itemModel;}

    inline CT_AbstractItemDrawable* itemDrawable() {return _itemDrawable;}
    inline QStandardItemModel* itemModel() {return _itemModel;}

private:
    CT_AbstractItemDrawable*   _itemDrawable;
    QStandardItemModel* _itemModel;

};

#endif // CT_MIMEDATA_H
