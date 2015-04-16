#include "pb_actionmanualinventoryattributesdialog.h"
#include "ui_pb_actionmanualinventoryattributesdialog.h"

#include <QLabel>
#include <QComboBox>
#include <QLineEdit>

PB_ActionManualInventoryAttributesDialog::PB_ActionManualInventoryAttributesDialog(QMap<QString, QStringList> *paramData,
                                                                                   QMap<QString, QString> &attrValues,
                                                                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PB_ActionManualInventoryAttributesDialog)
{
    ui->setupUi(this);

    _paramData = paramData;
    _attrValues = &attrValues;

    int cpt = 2;
    QMapIterator<QString, QStringList> it(*paramData);
    while (it.hasNext())
    {
        it.next();
        const QString &name = it.key();
        const QStringList &list = it.value();

        const QString &value = _attrValues->value(name);

        QWidget *wid = NULL;

        if (list.size() > 0)
        {
            wid = new QComboBox(ui->wid_attr);
            ((QComboBox*) wid)->addItems(list);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            ((QComboBox*) wid)->setCurrentIndex(((QComboBox*) wid)->findText(value););
#else
            ((QComboBox*) wid)->setCurrentText(value);
#endif

        } else {
            wid = new QLineEdit(value, ui->wid_attr);
        }

        _widgets.insert(name, wid);

        ((QGridLayout*) ui->wid_attr->layout())->addWidget(new QLabel(name, ui->wid_attr), cpt, 0);
        ((QGridLayout*) ui->wid_attr->layout())->addWidget(wid, cpt++, 1);

        if (cpt == 3) {wid->setFocus();}
    }
}

PB_ActionManualInventoryAttributesDialog::~PB_ActionManualInventoryAttributesDialog()
{
    delete ui;
}

QString PB_ActionManualInventoryAttributesDialog::getValueForAttr(QString name) const
{
    QWidget* wid = _widgets.value(name);
    if (wid == NULL) {return "";}

    QComboBox* cb = dynamic_cast<QComboBox*>(wid);
    if (cb != NULL)
    {
        return cb->currentText();
    } else {
        QLineEdit* le = dynamic_cast<QLineEdit*>(wid);
        if (le != NULL)
        {
            return le->text();
        }
    }
    return "";
}

