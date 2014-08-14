#ifndef PBG_CSVDATAREFLISTWIDGET_H
#define PBG_CSVDATAREFLISTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

class IItemDataRefList;
class IItemDataRef;

namespace Ui {
class PBG_CSVDataRefListWidget;
}

/*!
 *  Classe héritant de QStandardItemModel afin de retourner une QMimeData
 *  utilisable pour le drag d'un QStandardItem
 */
class PBG_CSVDataRefListWidgetDragModel : public QStandardItemModel
{
public:
    QMimeData* mimeData(const QModelIndexList &indexes) const;
};

class PBG_CSVDataRefListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PBG_CSVDataRefListWidget(QWidget *parent = 0);
    ~PBG_CSVDataRefListWidget();
    
    void setListOfDataRefList(const QList<const IItemDataRefList*> *list);

private:
    Ui::PBG_CSVDataRefListWidget *ui;

    QList<const IItemDataRefList*>      _list;
    PBG_CSVDataRefListWidgetDragModel   _model;

    void createHeader();
    void initView();

    QList<QStandardItem*> createItemsForIItemDataRefList(const IItemDataRefList *list) const;
    QList<QStandardItem*> createItemsForIItemDataRef(const IItemDataRefList *refList, const IItemDataRef *ref) const;

signals:
    void attributeChoosed(const QString &data);

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);
    void on_pushButtonAddAll_clicked();
};

#endif // PBG_CSVDATAREFLISTWIDGET_H
