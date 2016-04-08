#include "ct_genericconfigurablewidget.h"

#include "ct_spinbox.h"
#include "ct_doublespinbox.h"
#include "ct_checkbox.h"
#include "ct_asciifilechoicebutton.h"
#include "ct_combobox.h"
#include "ct_radiobutton.h"
#include "ct_buttongroup.h"
#include "ct_lineedit.h"
#include "ct_multilineedit.h"

CT_GenericConfigurableWidget::CT_GenericConfigurableWidget(QWidget *parent) : CT_AbstractConfigurableWidget(parent)
{
    setWindowTitle(tr("Configuration"));

    QVBoxLayout *lay = new QVBoxLayout(this);
    m_gridLayout = new QGridLayout();
    lay->addLayout(m_gridLayout);
    lay->addStretch();
    m_canEdit = true;
    m_nRow = 0;
}

CT_GenericConfigurableWidget::~CT_GenericConfigurableWidget()
{
    qDeleteAll(m_ctWidget.begin(), m_ctWidget.end());
}

bool CT_GenericConfigurableWidget::addText(const QString &firstColumnLabelText,
                                           const QString &secondColumnLabelText,
                                           const QString &thirdColumnLabelText,
                                           const QString &description)
{
    if(canEdit())
    {
        addLabel(nRows(), 0, firstColumnLabelText, 1, (secondColumnLabelText.isEmpty() ? (thirdColumnLabelText.isEmpty() ? 3 : 2) : 1));
        addLabel(nRows(), 1, secondColumnLabelText, 1, (thirdColumnLabelText.isEmpty() ? 2 : 1));
        addLabel(nRows(), 2, thirdColumnLabelText);
        addDescription(nRows(), description);

        addRow();

        return true;
    }

    return false;
}

bool CT_GenericConfigurableWidget::addTitle(const QString &text,
                                            const QString &description)
{
    return addText(text, "", "", description);
}

bool CT_GenericConfigurableWidget::addInt(const QString &beforeLabelText,
                                          const QString &afterLabelText,
                                          const int &minValue,
                                          const int &maxValue,
                                          int &valueToUpdate,
                                          const QString &description)
{
    if(canEdit())
    {
        CT_SpinBox *spinBox = new CT_SpinBox(minValue, maxValue, valueToUpdate, beforeLabelText);
        m_ctWidget.append(spinBox);

        addLabel(nRows(), 0, beforeLabelText);
        addWidget(nRows(), 1, spinBox->createWidget(*this));
        addLabel(nRows(), 2, afterLabelText);
        addDescription(nRows(), description);

        addRow();

        return true;
    }

    return false;
}

bool CT_GenericConfigurableWidget::addDouble(const QString &beforeLabelText,
                                             const QString &afterLabelText,
                                             const double &minValue,
                                             const double &maxValue,
                                             const int &nDecimals,
                                             double &valueToUpdate,
                                             const double &multValue,
                                             const QString &description)
{
    if(canEdit())
    {
        CT_DoubleSpinBox *spinBox = new CT_DoubleSpinBox(minValue, maxValue, nDecimals, valueToUpdate, multValue, beforeLabelText);
        m_ctWidget.append(spinBox);

        addLabel(nRows(), 0, beforeLabelText);
        addWidget(nRows(), 1, spinBox->createWidget(*this));
        addLabel(nRows(), 2, afterLabelText);
        addDescription(nRows(), description);

        addRow();

        return true;
    }

    return false;
}

bool CT_GenericConfigurableWidget::addBool(const QString &beforeLabelText,
                                           const QString &afterLabelText,
                                           const QString &checkBoxText,
                                           bool &valueToUpdate,
                                           const QString &description)
{
    if(canEdit())
    {
        CT_CheckBox *checkBox = new CT_CheckBox(checkBoxText, valueToUpdate, beforeLabelText);
        m_ctWidget.append(checkBox);

        addLabel(nRows(), 0, beforeLabelText);
        addWidget(nRows(), 1, checkBox->createWidget(*this));
        addLabel(nRows(), 2, afterLabelText);
        addDescription(nRows(), description);

        addRow();

        return true;
    }

    return false;
}

bool CT_GenericConfigurableWidget::addString(const QString &beforeLabelText,
                                             const QString &afterLabelText,
                                             QString &valueToUpdate,
                                             const QString &description)
{
    if(canEdit())
    {
        CT_LineEdit *lineEdit = new CT_LineEdit(valueToUpdate, beforeLabelText);
        m_ctWidget.append(lineEdit);

        addLabel(nRows(), 0, beforeLabelText);
        addWidget(nRows(), 1, lineEdit->createWidget(*this));
        addLabel(nRows(), 2, afterLabelText);
        addDescription(nRows(), description);

        addRow();

        return true;
    }

    return false;
}

bool CT_GenericConfigurableWidget::addMultiString(const QString &beforeLabelText,
                                                  const QString &afterLabelText,
                                                  QString &valueToUpdate,
                                                  const QString &description)
{
    if(canEdit())
    {
        CT_MultiLineEdit *lineEdit = new CT_MultiLineEdit(valueToUpdate, beforeLabelText);
        m_ctWidget.append(lineEdit);

        addLabel(nRows(), 0, beforeLabelText);
        addWidget(nRows(), 1, lineEdit->createWidget(*this));
        addLabel(nRows(), 2, afterLabelText);
        addDescription(nRows(), description);

        addRow();

        return true;
    }

    return false;
}

CT_ComboBox* CT_GenericConfigurableWidget::addStringChoice(const QString &beforeLabelText,
                                                           const QString &afterLabelText,
                                                           const QStringList &valuesList,
                                                           QString &valueToUpdate,
                                                           const QString &description)
{
    CT_ComboBox *comboBox = NULL;

    if(canEdit())
    {
        comboBox = new CT_ComboBox(valuesList, valueToUpdate, beforeLabelText);
        m_ctWidget.append(comboBox);

        addLabel(nRows(), 0, beforeLabelText);
        addWidget(nRows(), 1, comboBox->createWidget(*this));
        addLabel(nRows(), 2, afterLabelText);
        addDescription(nRows(), description);

        addRow();

        if (!valueToUpdate.isEmpty())
            comboBox->setWidgetValue(valueToUpdate);
    }

    return comboBox;
}

CT_ButtonGroup& CT_GenericConfigurableWidget::addButtonGroup(int &valueToUpdate)
{
    CT_ButtonGroup *bg = new CT_ButtonGroup(*this, valueToUpdate);
    m_ctWidget.append(bg);

    return *bg;
}

bool CT_GenericConfigurableWidget::addExcludeValue(const QString &beforeLabelText,
                                                   const QString &afterLabelText,
                                                   const QString &radioButtonText,
                                                   CT_ButtonGroup &group,
                                                   const int &id,
                                                   const QString &description)
{
    if(canEdit())
    {
        CT_RadioButton *radioButton = new CT_RadioButton(radioButtonText, group.getValueReference(), id);

        if(!group.addRadioButton(radioButton))
        {
            delete radioButton;
            return false;
        }

        if(!m_ctWidget.contains(&group))
            m_ctWidget.append(&group);

        addLabel(nRows(), 0, beforeLabelText);
        addWidget(nRows(), 1, radioButton->createWidget(*this));
        addLabel(nRows(), 2, afterLabelText);
        addDescription(nRows(), description);

        addRow();

        return true;
    }

    return false;
}

void CT_GenericConfigurableWidget::addEmpty()
{
    if(canEdit())
    {
        addLabel(nRows(), 0, "");
        addRow();
    }
}

CT_FileChoiceButton* CT_GenericConfigurableWidget::addFileChoice(const QString &btLabel,
                                                                 const CT_FileChoiceButton::NeededFileType &filetype,
                                                                 const QString &fileFilter, QStringList &valueToUpdate,
                                                                 const QString &description,
                                                                 const QString &beforeLabelText,
                                                                 const QString &afterLabelText)
{
    CT_FileChoiceButton *fileChoiceButton = NULL;

    if(canEdit())
    {
        fileChoiceButton = new CT_FileChoiceButton(btLabel, filetype, fileFilter, valueToUpdate, btLabel);
        m_ctWidget.append(fileChoiceButton);

        addLabel(nRows(), 0, beforeLabelText);
        addWidget(nRows(), 1, fileChoiceButton->createWidget(*this));
        addLabel(nRows(), 2, afterLabelText);
        addDescription(nRows(), description);

        addRow();

        fileChoiceButton->setWidgetValue(valueToUpdate);
    }

    return fileChoiceButton;
}

CT_AsciiFileChoiceButton *CT_GenericConfigurableWidget::addAsciiFileChoice(QString btlab,
                                                                           QString fileFilter,
                                                                           bool autoDetect,
                                                                           const QStringList &neededFields,
                                                                           QString &fileName,
                                                                           bool &header,
                                                                           QString &separator,
                                                                           QString &decimal,
                                                                           QLocale &locale,
                                                                           int &skip,
                                                                           QMap<QString, int> &columns,
                                                                           QString description)
{
    CT_AsciiFileChoiceButton *fileChoiceButton = NULL;

    if(canEdit())
    {
        fileChoiceButton = new CT_AsciiFileChoiceButton(btlab,
                                                      fileFilter,
                                                      autoDetect,
                                                      neededFields,
                                                      fileName,
                                                      header,
                                                      separator,
                                                      decimal,
                                                      locale,
                                                      skip,
                                                      columns,
                                                      description);

        m_ctWidget.append(fileChoiceButton);

        addWidget(nRows(), 0, fileChoiceButton->createWidget(*this), 1, -1);

        addRow();

        fileChoiceButton->setWidgetValue(fileName);
    }

    return fileChoiceButton;
}

CT_AsciiFileChoiceButton *CT_GenericConfigurableWidget::addAsciiFileChoice(QString btlab,
                                                                           QString fileFilter,
                                                                           bool autoDetect,
                                                                           QList<CT_TextFileConfigurationFields> &neededFields,
                                                                           QString &fileName,
                                                                           bool &header,
                                                                           QString &separator,
                                                                           QString &decimal,
                                                                           QLocale &locale,
                                                                           int &skip,
                                                                           QMap<QString, int> &columns,
                                                                           QString description)
{
    CT_AsciiFileChoiceButton *fileChoiceButton = NULL;

    if(canEdit())
    {
        fileChoiceButton = new CT_AsciiFileChoiceButton(btlab,
                                                      fileFilter,
                                                      autoDetect,
                                                      neededFields,
                                                      fileName,
                                                      header,
                                                      separator,
                                                      decimal,
                                                      locale,
                                                      skip,
                                                      columns,
                                                      description);
        m_ctWidget.append(fileChoiceButton);

        addWidget(nRows(), 0, fileChoiceButton->createWidget(*this), 1, -1);

        addRow();

        fileChoiceButton->setWidgetValue(fileName);
    }

    return fileChoiceButton;
}

QList<SettingsNodeGroup*> CT_GenericConfigurableWidget::getAllValues() const
{
    QList<SettingsNodeGroup*> retList;

    SettingsNodeGroup *gr = new SettingsNodeGroup("Version");
    gr->addValue(new SettingsNodeValue("V", "1"));
    retList.append(gr);

    gr = new SettingsNodeGroup("Widget");
    retList.append(gr);

    QListIterator<CT_WidgetWithValueReferenceInterface*> it(m_ctWidget);

    while(it.hasNext())
    {
        CT_WidgetWithValueReferenceInterface *wid = it.next();

        SettingsNodeGroup *type = new SettingsNodeGroup(wid->type());
        QList<SettingsNodeGroup*> groups = wid->getAllValues();

        while(!groups.isEmpty())
            type->addGroup(groups.takeFirst());

        gr->addGroup(type);
    }

    return retList;
}

bool CT_GenericConfigurableWidget::setAllValues(const QList<SettingsNodeGroup*> &list)
{
    if((list.size() != 2)
            || (list.at(1)->name() != "Widget"))
        return false;


    const QList<SettingsNodeGroup*> &groups = list.at(1)->groups();

    if(groups.size() != m_ctWidget.size())
        return false;

    QListIterator<CT_WidgetWithValueReferenceInterface*> it(m_ctWidget);
    QListIterator<SettingsNodeGroup*> it2(groups);

    while(it.hasNext())
    {
        CT_WidgetWithValueReferenceInterface *wid = it.next();
        SettingsNodeGroup *type = it2.next();

        if(type->name() != wid->type())
            return false;

        if(!wid->setAllValues(type->groups()))
            return false;

        wid->updateValue();
    }

    return true;
}

bool CT_GenericConfigurableWidget::isSettingsModified() const
{
    // TODO : is method "isValueAndWidgetValueDifferent" ok for all element ? to verify and after you can uncomment it.
    /*QListIterator<CT_WidgetWithValueReferenceInterface*> it(m_ctWidget);
    while(it.hasNext()) {
        if(it.next()->isValueAndWidgetValueDifferent())
            return true;
    }

    return false;*/

    return true;
}

QGridLayout* CT_GenericConfigurableWidget::getLayout() const
{
    return m_gridLayout;
}

void CT_GenericConfigurableWidget::addWidget(int row,
                                             int column,
                                             QWidget *wid,
                                             int rowSpan,
                                             int colSpan)
{
    getLayout()->addWidget(wid, row, column, rowSpan, colSpan);
}

void CT_GenericConfigurableWidget::addLabel(int row,
                                            int column,
                                            const QString &text,
                                            int rowSpan,
                                            int colSpan)
{
    addWidget(row, column, new QLabel(text, this), rowSpan, colSpan);
}

void CT_GenericConfigurableWidget::addDescription(int row, const QString &description, int column)
{
    if(!description.isEmpty()) {
        HelpButton *wid = new HelpButton(this);
        wid->setPixmap(QPixmap(":/Icones/Icones/help.png").scaledToHeight(25,Qt::SmoothTransformation));
        wid->setToolTip(description);

        getLayout()->addWidget(wid, row, column);
    }
}

bool CT_GenericConfigurableWidget::canEdit() const
{
    return m_canEdit;
}

int CT_GenericConfigurableWidget::nRows() const
{
    return m_nRow;
}

void CT_GenericConfigurableWidget::addRow()
{
    ++m_nRow;
}

void CT_GenericConfigurableWidget::endEdit()
{
    m_canEdit = false;
}

bool CT_GenericConfigurableWidget::updateElement(QString *err)
{
    Q_UNUSED(err)

    QListIterator<CT_WidgetWithValueReferenceInterface*> it(m_ctWidget);

    while(it.hasNext())
        it.next()->updateValue();

    return true;
}
