/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michaël KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/



#ifndef CT_STEPCONFIGURABLEDIALOG_H
#define CT_STEPCONFIGURABLEDIALOG_H

#include "pluginShared_global.h"
#include "ct_filechoicebutton.h"
#include "ct_view/tools/ct_textfileconfigurationdialog.h"


#include <QVariantList>
#include <QObject>
#include <QMouseEvent>

class CT_VirtualAbstractStep;

class QDialog;
class QWidget;
class QGridLayout;
class CT_WidgetWithValueReferenceInterface;
class CT_ButtonGroup;
class CT_FileChoiceButton;
class CT_AsciiFileChoiceButton;
class CT_ComboBox;
class SettingsNodeGroup;

class HelpButton : public QLabel {
    Q_OBJECT

public:
    HelpButton(QWidget *parent) : QLabel(parent) {}

    QString helpText;

protected:
    void mousePressEvent(QMouseEvent * event) {
        if(event->button() == Qt::LeftButton)
            emit clicked(helpText);

        QLabel::mousePressEvent(event);
    }

signals:
    void clicked(QString helpText);
};

/*!
 * \class CT_StepConfigurableDialog
 * \brief Automatic dialog for step configuration
 *
 * You just have to add parameters to the dialog with the addXxx() methods.
 * The graphic components will be automatically created, aligned in three columns.
 * The selected values will be automatically returned in specified variables (<b>value</b> parameters).
 *
 */
class PLUGINSHAREDSHARED_EXPORT CT_StepConfigurableDialog : public QObject
{
    Q_OBJECT
public:
    CT_StepConfigurableDialog();
    ~CT_StepConfigurableDialog();


    /*! \brief Set the dialog title ("configuration" by default)
     *
     * \param title Title to show
     */
    void setWindowTitle(QString title);

    QString windowTitle() const;

    int exec();

    bool setStep(CT_VirtualAbstractStep &step);


    /*! \brief Add a label line in the dialog
     *
     * \param firstColumnLabelText Text to show in the first column.
     * \param secondColumnLabelText Text to show in the second column.
     * \param thirdColumnLabelText Text to show in the third column.
     */
    bool addText(QString firstColumnLabelText,
                 QString secondColumnLabelText,
                 QString thirdColumnLabelText);

    /*! \brief Add a label line in the dialog (the text extends on the three columns)
     *
     * \param titleText Text to show.
     */
    bool addTitle(QString titleText);


    /*! \brief Add an int parameter input to a new line in the dialog
     *
     * The following graphic composants are automatically added to the next line of the dialog :\n
     * The <b>beforeLabelText</b> is added as a QLabel in the first column.\n
     * A QSpinBox is added in the second column for the user to enter the int value.\n
     * The <b>afterLabelText</b> is added as a QLabel in the third column.
     *
     * \param beforeLabelText Text to show in the first column. Generally the asking text for the parameter.
     * \param afterLabelText Text to show in the third column. Generally the unit of the value.
     * \param minValue Minimum allowed value
     * \param maxValue Maximum allowed value
     * \param[out] value Output variable for the value. Note that it's also used to fix input default value.
     */
    bool addInt(QString beforeLabelText,
                QString afterLabelText,
                int minValue,
                int maxValue,
                int &value,
                QString helpText = "");

    /*! \brief Add a double parameter input to a new line in the dialog
     *
     * The following graphic composants are automatically added to the next line of the dialog :\n
     * The <b>beforeLabelText</b> is added as a QLabel in the first column.\n
     * A QDoubleSpinBox is added in the second column for the user to enter the double value.\n
     * The <b>afterLabelText</b> is added as a QLabel in the third column.\n\n
     * Note : The output variable could be in one unit, and the user can give it in another one.
     * For example if the variable is in meter, and you wan't the dialog to be in centimeters, use
     * a <b>multValue</b> of 100. The conversion is automatic.
     *
     * \param beforeLabelText Text to show in the first column. Generally the asking text for the parameter.
     * \param afterLabelText Text to show in the third column. Generally the unit of the value.
     * \param minValue Minimum allowed value.
     * \param maxValue Maximum allowed value.
     * \param nDecimals Number of decimals allowed for the value.
     * \param[out] value Output variable for the value. Note that it's also used to fix input default value.
     * \param multValue Multiplicative factor to the value.
     *
     */
    bool addDouble(QString beforeLabelText,
                   QString afterLabelText,
                   double minValue,
                   double maxValue,
                   int nDecimals,
                   double &value,
                   double multValue = 1,
                   QString helpText = "");

    /*! \brief Add a bool parameter input to a new line in the dialog
     *
     * The following graphic composants are automatically added to the next line of the dialog :\n
     * The <b>beforeLabelText</b> is added as a QLabel in the first column.\n
     * A QCheckBox is added in the second column for the user to enter the bool value.\n
     * The <b>afterLabelText</b> is added as a QLabel in the third column.
     *
     * \param beforeLabelText Text to show in the first column. Generally the asking text for the parameter.
     * \param afterLabelText Text to show in the third column. Generally the unit of the value.
     * \param checkBoxText Text to show at the right of the checkBox, in the second column.
     * \param[out] value Output variable for the value. Note that it's also used to fix input default value.
     */
    bool addBool(QString beforeLabelText,
                 QString afterLabelText,
                 QString checkBoxText,
                 bool &value,
                 QString helpText = "");


    /*! \brief Add an QString parameter input to a new line in the dialog
     *
     * The following graphic composants are automatically added to the next line of the dialog :\n
     * The <b>beforeLabelText</b> is added as a QLabel in the first column.\n
     * A QLineEdit is added in the second column for the user to enter the QString value.\n
     * The <b>afterLabelText</b> is added as a QLabel in the third column.
     *
     * \param beforeLabelText Text to show in the first column. Generally the asking text for the parameter.
     * \param afterLabelText Text to show in the third column. Generally the unit of the value.
     * \param[out] value Output variable for the value. Note that it's also used to fix input default value.
     */
    bool addString(QString beforeLabelText,
                   QString afterLabelText,
                   QString &value,
                   QString helpText = "");


    /*! \brief Add a list of choice for a parameter to a new line in the dialog
     *
     * The following graphic composants are automatically added to the next line of the dialog :\n
     * The <b>beforeLabelText</b> is added as a QLabel in the first column.\n
     * A QComboBox is added in the second column for the user to enter the choosen value.\n
     * The <b>afterLabelText</b> is added as a QLabel in the third column.
     *
     * \param beforeLabelText Text to show in the first column. Generally the asking text for the parameter.
     * \param afterLabelText Text to show in the third column. Generally the unit of the value.
     * \param valuesList List of QString (QStringList) allowed values for the parameter.
     * \param[out] value Output variable for the value. Note that it's also used to fix input default value.
     */
    CT_ComboBox *addStringChoice(QString beforeLabelText,
                                 QString afterLabelText,
                                 QStringList valuesList,
                                 QString &value,
                                 QString helpText = "");

    /*! \brief Add a control allowing to choose af file to a new line in the dialog
     *
     * The following graphic composants are automatically added to the next line of the dialog :\n
     * A QPushButton, with label <b>btLabel</b>. On click it open a choose file dialog.\n
     * A QLabel
     *
     * \param btLabel Label of the button.
     * \param filetype Type of FileChooseDialog needed :\n
     * - CT_FileChoiceButton::OneExistingFile : to select only one an existing file.\n
     * - CT_FileChoiceButton::OneOrMoreExistingFiles : to select one or multiple an existing file(s).\n
     * - CT_FileChoiceButton::OneNewFile : to select a new file to create.\n
     * - CT_FileChoiceButton::OneExistingFolder : to select an existing folder.\n
     *
     * \param[out] value Output variable for the value. Note that it's also used to fix input default value.
     */
    CT_FileChoiceButton* addFileChoice(QString btLabel,
                                       CT_FileChoiceButton::NeededFileType filetype,
                                       QString fileFilter,
                                       QStringList &value,
                                       QString helpText = "");


    /*! \brief Add a control allowing to choose an ASCII file to a new line in the dialog
     *
     * The following graphic composants are automatically added to the next line of the dialog :\n
     * A QPushButton, with label <b>btLabel</b>. On click it open a choose file dialog.\n
     * A QLabel
     *
     * \param btLabel Label of the button.
     * \param fileFilter File filter.
     * \param autoDetect Fields separator automatic detection.
     * \param neededFields List of needed fields from the ascii file.
     *
     * \param[out] fileName Choosen file name. Note that it's also used to fix input default value.
     * \param[out] header Does the file have a header. Note that it's also used to fix input default value.
     * \param[out] separator Fields separator. Note that it's also used to fix input default value.
     * \param[out] decimal Decimal separator. Note that it's also used to fix input default value.
     * \param[out] locale Locale corresponding to decimal separator. Note that it's also used to fix input default value.
     * \param[out] skip Number of lines to skip before fields header line. Note that it's also used to fix input default value.
     * \param[out] columns correspondance between needed fields and corresponding columns numbers in the file. Note that it's also used to fix input default value.
     * \param[out] description Description (for the script export). Note that it's also used to fix input default value.
     */
    CT_AsciiFileChoiceButton *addAsciiFileChoice(QString btlab,
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
                                                 QString description = "");

    CT_AsciiFileChoiceButton *addAsciiFileChoice(QString btlab,
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
                                                 QString description = "");


    /*! \brief Create a CT_ButtonGroup associated to an ibdex value
     *
     * This method don't add any graphic component to dialog.
     * It only manage the mutual exclusion of associated exclude Values.
     * The <b>value</b> parameter store the selected case index.
     * The index corresponding to each case is specified with corresponding addExcludeValue() call.
     *
     * \param[out] value Output variable for the index referencing selected value. Note that it's also used to fix input default value.
     */
    CT_ButtonGroup& addButtonGroup(int &value);

    /*! \brief Add an exclude value to a new line in the dialog
     *
     * This has to be used in combination with addButtonGroup().
     * You should add one exclude value for each case.
     * All of these exclusives values have to be in the same CT_ButtonGroup,
     * to ensure that only one can be select at one time.
     *
     * The following graphic composants are automatically added to the next line of the dialog :\n
     * The <b>beforeLabelText</b> is added as a QLabel in the first column.\n
     * A QRadioButton is added in the second column for the user to choose a case\n
     * The <b>afterLabelText</b> is added as a QLabel in the third column.
     *
     * \param beforeLabelText Text to show in the first column. Generally the asking text for the parameter.
     * \param afterLabelText Text to show in the third column. Generally the unit of the value.
     * \param radioButtonText Text to show at the right of the radioButton, in the second column.
     * \param group Group of exclusives values
     * \param id Id of this case
     */
    bool addExcludeValue(QString beforeLabelText,
                         QString afterLabelText,
                         QString radioButtonText,
                         CT_ButtonGroup &group,
                         int id,
                         QString helpText = "");

    /*! \brief Add a new empty line in the dialog
     *
     * For separating differents parts in configurations dialog
     *
     */
    void addEmpty();

    void endEdit();

    QList<SettingsNodeGroup*> getAllValues() const;
    bool setAllValues(const QList<SettingsNodeGroup*> &list);

    bool isSettingsModified() const;

signals:

    void openned();

private:

    QList<CT_WidgetWithValueReferenceInterface*> _listWidgetWithValueReference;

    QDialog                 *_dialog;
    QWidget                 *_wid;

    CT_VirtualAbstractStep  *_step;

    bool                    _canEdit;
    int                     _nRow;

    QGridLayout* getLayout(QWidget *wid) const;
    void addWidget(int row, int column, QWidget *wid, int rowSpan = 1, int colSpan = 1);
    void addLabel(int row, int column, QString text);
    void addHelpButton(int row, int column, QString helpText);

private slots:
    void showHelpMessage(QString helpText);
};

#endif // CT_STEPCONFIGURABLEDIALOG_H
