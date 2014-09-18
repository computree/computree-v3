#ifndef MYQSTANDARDITEM_H
#define MYQSTANDARDITEM_H

#include <QObject>
#include <QStandardItem>

class CT_VirtualAbstractStep;
class CT_AbstractResult;

/**
  * Classe permettant de grer les tapes et les rsultats dans le modele de l'arbre
  */
class MyQStandardItem : public QObject, public QStandardItem
{
    Q_OBJECT

public:

    /**
      * Type de colonne que reprsente cette item
      */
    enum ColumnType {
       StepName,                // Nom de l'tape
       StepProgress,            // Progression de l'tape
       StepElapsedTime,         // Temps coul de l'tape
       ResultName,              // Nom du rsultat
       ResultProgress,          // Progression du dchargement du rsultat de la mmoire
       Empty                    // Case vide pour un rsultat
    };

    enum MyItemDataRole {
        ColumnTypeRole = Qt::UserRole + 1,
        InvisibleBoolTypeRole = ColumnTypeRole + 1
    };

    MyQStandardItem(CT_VirtualAbstractStep *step,
                    CT_AbstractResult *res,
                    MyQStandardItem::ColumnType type,
                    const QString &text);

    MyQStandardItem(CT_VirtualAbstractStep *step,
                    CT_AbstractResult *res,
                    MyQStandardItem::ColumnType type,
                    const QVariant &value);

    CT_VirtualAbstractStep* step() { return _step; }
    CT_AbstractResult* result() { return _result; }
    void removeResult() { _result = NULL; }

    MyQStandardItem::ColumnType columnType() const { return _columnType; }

    void setData(const QVariant &value, int role);

private:

    CT_VirtualAbstractStep            *_step;
    CT_AbstractResult          *_result;

    ColumnType      _columnType;

public slots:

    /**
      * Des slots qui permettent d'tre connects au signaux d'tape ou de rsultat
      */
    void setIntData(int data, Qt::ItemDataRole role = Qt::DisplayRole);
    void setFloatData(float data, Qt::ItemDataRole role = Qt::DisplayRole);
    void setBoolData(bool data, Qt::ItemDataRole role = Qt::CheckStateRole);
    void setBoolDataInvisible(bool data);
    void setColorData(QColor data, Qt::ItemDataRole role = Qt::TextColorRole);
    void slotSetEnabled(bool enable);

signals:

    void dataChanged(QStandardItem *item);
    void checkStateChanged(bool checked);
};

#endif // MYQSTANDARDITEM_H
