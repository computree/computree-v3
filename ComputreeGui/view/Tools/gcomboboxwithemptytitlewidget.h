#ifndef GCOMBOBOXWITHEMPTYTITLEWIDGET_H
#define GCOMBOBOXWITHEMPTYTITLEWIDGET_H

#include <QWidget>
#include <QComboBox>

namespace Ui {
class GComboBoxWithEmptyTitleWidget;
}

class GComboBoxWithEmptyTitleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GComboBoxWithEmptyTitleWidget(QWidget *parent = 0);
    ~GComboBoxWithEmptyTitleWidget();

    void setEmptyText(const QString &text);
    QString currentItemText() const;

    void setRemoveEmptyTextWhenAddNewItem(bool enable);
    bool isRemoveEmptyTextWhenAddNewItem() const;

    QComboBox* comboBox() const;

private:
    Ui::GComboBoxWithEmptyTitleWidget   *ui;
    QString                             m_emptyText;
    bool                                m_removeEmptyText;
    bool                                m_removeItem;

    void initEmptyText();

signals:
    void currentItemChanged(const QString &text, const QString displayableText = "");
    void currentItemActivated(const QString &text, const QString displayableText = "");

public slots:
    void addItem(const QString &text, const QString displayableText = "");
    void removeItem(const QString &text);
    void clear();

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_comboBox_activated(int index);
};

#endif // GCOMBOBOXWITHEMPTYTITLEWIDGET_H
