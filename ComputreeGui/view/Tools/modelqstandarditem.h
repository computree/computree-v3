#ifndef MODELQSTANDARDITEM_H
#define MODELQSTANDARDITEM_H

#include <QStandardItem>

class CT_AbstractModel;

class ModelQStandardItem : public QStandardItem
{
public:
    ModelQStandardItem() : QStandardItem() { setModel(NULL); }

    void setModel(const CT_AbstractModel *model) { _model = (CT_AbstractModel*)model; }
    CT_AbstractModel* getModel() const { return _model; }

    bool isChecked() const { return (checkState() == Qt::Checked); }

private:

    CT_AbstractModel  *_model;
};

#endif // MODELQSTANDARDITEM_H
