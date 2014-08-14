#include "gcomboboxwithemptytitlewidget.h"
#include "ui_gcomboboxwithemptytitlewidget.h"

#include <QLineEdit>

GComboBoxWithEmptyTitleWidget::GComboBoxWithEmptyTitleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GComboBoxWithEmptyTitleWidget)
{
    ui->setupUi(this);
    m_removeEmptyText = true;
    m_removeItem = true;

    clear();
}

GComboBoxWithEmptyTitleWidget::~GComboBoxWithEmptyTitleWidget()
{
    delete ui;
}

void GComboBoxWithEmptyTitleWidget::setEmptyText(const QString &text)
{
    m_emptyText = text;

    if((m_removeEmptyText
            && ui->comboBox->isEditable())
            || !m_removeEmptyText)
    {
        ui->comboBox->setItemText(0, m_emptyText);
    }
}

QString GComboBoxWithEmptyTitleWidget::currentItemText() const
{
    return ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();
}

void GComboBoxWithEmptyTitleWidget::setRemoveEmptyTextWhenAddNewItem(bool enable)
{
    if(enable != m_removeEmptyText)
    {
        m_removeEmptyText = enable;

        int ci = ui->comboBox->currentIndex();

        if(enable && (ui->comboBox->count() > 1))
            ui->comboBox->removeItem(0);
        else if((ci == 0) && !ui->comboBox->isEditable())
            ui->comboBox->insertItem(0, m_emptyText);
    }
}

bool GComboBoxWithEmptyTitleWidget::isRemoveEmptyTextWhenAddNewItem() const
{
    return m_removeEmptyText;
}

QComboBox* GComboBoxWithEmptyTitleWidget::comboBox() const
{
    return ui->comboBox;
}

void GComboBoxWithEmptyTitleWidget::initEmptyText()
{
    ui->comboBox->addItem(m_emptyText);
    ui->comboBox->setEditable(true);
    ui->comboBox->lineEdit()->setAlignment(Qt::AlignCenter);
    ui->comboBox->lineEdit()->setReadOnly(true);
}

void GComboBoxWithEmptyTitleWidget::addItem(const QString &text, const QString displayableText)
{
    QString dispText = displayableText;

    if(displayableText.isEmpty())
        dispText = text;

    if(ui->comboBox->findText(dispText) != -1)
        return;

    ui->comboBox->addItem(dispText, text);

    if(m_removeEmptyText
            && ui->comboBox->isEditable())
    {
        ui->comboBox->removeItem(0);
        ui->comboBox->setEditable(false);
    }
}

void GComboBoxWithEmptyTitleWidget::removeItem(const QString &text)
{
    int count = ui->comboBox->count();

    for(int i=0; i<count; ++i)
    {
        if(ui->comboBox->itemData(i).toString() == text)
        {
            ui->comboBox->removeItem(i);

            if(m_removeEmptyText && (ui->comboBox->count() == 0))
                initEmptyText();
        }
    }
}

void GComboBoxWithEmptyTitleWidget::clear()
{
    ui->comboBox->clear();

    initEmptyText();
}

void GComboBoxWithEmptyTitleWidget::on_comboBox_currentIndexChanged(int index)
{
    if(!m_removeEmptyText
            && (index == 0))
    {
        ui->comboBox->setEditable(true);
        ui->comboBox->lineEdit()->setAlignment(Qt::AlignCenter);
        ui->comboBox->lineEdit()->setReadOnly(true);
    }
    else
    {
        ui->comboBox->setEditable(false);
    }

    emit currentItemChanged(ui->comboBox->itemData(index).toString(), ui->comboBox->itemText(index));
}

void GComboBoxWithEmptyTitleWidget::on_comboBox_activated(int index)
{
    emit currentItemActivated(ui->comboBox->itemData(index).toString(), ui->comboBox->itemText(index));
}
