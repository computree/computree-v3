#ifndef MODELQSTANDARDITEM_H
#define MODELQSTANDARDITEM_H

#include <QStandardItem>

class IModel;

class ModelQStandardItem : public QStandardItem
{
public:
    ModelQStandardItem() : QStandardItem() { setModel(NULL); }

    void setModel(const IModel *model) { _model = (IModel*)model; }
    IModel* getModel() const { return _model; }

    bool isChecked() const { return (checkState() == Qt::Checked); }

private:

    IModel  *_model;
};

#endif // MODELQSTANDARDITEM_H
