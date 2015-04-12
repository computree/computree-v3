#ifndef PB_ACTIONMANUALINVENTORYATTRIBUTESDIALOG_H
#define PB_ACTIONMANUALINVENTORYATTRIBUTESDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QMap>

namespace Ui {
class PB_ActionManualInventoryAttributesDialog;
}

class PB_ActionManualInventoryAttributesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PB_ActionManualInventoryAttributesDialog(QMap<QString, QStringList> *paramData,
                                                      QMap<QString, QString> &attrValues,
                                                      QWidget *parent = 0);
    ~PB_ActionManualInventoryAttributesDialog();

    QString getValueForAttr(QString name) const;

private:
    Ui::PB_ActionManualInventoryAttributesDialog *ui;

    QMap<QString, QStringList>                      *_paramData;
    QMap<QString, QString>                          *_attrValues;
    QMap<QString, QWidget*>                         _widgets;

};

#endif // PB_ACTIONMANUALINVENTORYATTRIBUTESDIALOG_H
