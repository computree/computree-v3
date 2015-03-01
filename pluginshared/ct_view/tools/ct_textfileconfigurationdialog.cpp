#include "ct_textfileconfigurationdialog.h"
#include "ui_ct_textfileconfigurationdialog.h"
#include "qfiledialog.h"

#include "qdebug.h"


CT_TextFileConfigurationDialog::CT_TextFileConfigurationDialog(QStringList neededFields, QWidget *parent, QString fileName, bool autoDetect) :
    QDialog(parent),
    ui(new Ui::CT_TextFileConfigurationDialog)
{
    ui->setupUi(this);

    // Modif michael : problème si on met a NULL ! si on ne clique pas sur le bouton filechoose
    _stream = NULL;
    _file = NULL;
    _autoDetect = autoDetect;

    if (_autoDetect) {ui->pb_detect->setVisible(false);}

    delete ui->fieldWidget->layout();
    _layout = new QGridLayout(ui->fieldWidget);

    _neededFields = neededFields;

    // paramétrage de la combo-box des séparateurs
    ui->separator->addItem("Tabulation");
    ui->separator->addItem("Virgule");
    ui->separator->addItem("Point-Virgule");
    ui->separator->addItem("Espace");
    _separator = "\t";
    ui->nbLines->setValue(10);
    ui->cb_noheader->setChecked(false);

    _filename = fileName;

    init();
    extractFieldsNames();
}

void CT_TextFileConfigurationDialog::init()
{
    if (!_filename.isEmpty())
    {
        // Lecture du fichier (entete)
        delete _file;
        _file = new QFile(_filename);
        if (_file->exists() && _file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            _stream = new QTextStream(_file);
            _file->close();

            if (_autoDetect) {on_pb_detect_clicked();}

            ui->filePath->setText(_filename);
            on_nbLines_valueChanged(ui->nbLines->value());
        } else {
            ui->filePath->setText("Aucun fichier valide choisi.");
            ui->fileExtract->setText("");
        }
    }
}

CT_TextFileConfigurationDialog::~CT_TextFileConfigurationDialog()
{
    delete _stream;
    delete _file;
    delete ui;
}

void CT_TextFileConfigurationDialog::on_fileChoose_clicked()
{
    QString filter = "Fichier ascii (*";

    QStringListIterator it(_extensions);

    if(it.hasNext())
    {
        while(it.hasNext())
        {
            filter += it.next();

            if(it.hasNext())
                filter += " *";
        }
    }
    else
    {
        filter += ".*";
    }

    filter += ")";

    // choix du fichier
    _filename = QFileDialog::getOpenFileName(this, "Choix du fichier ascii", "", filter);

    // nettoyage
    clearFieldCombos();

    init();
    extractFieldsNames();
}

void CT_TextFileConfigurationDialog::on_separator_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Virgule") {
        _separator = ",";
    } else if (arg1 == "Point-Virgule") {
        _separator = ";";
    } else if (arg1 == "Espace") {
        _separator = " ";
    } else {
        _separator = "\t";
    }

    on_nbLines_valueChanged(ui->nbLines->value());
    extractFieldsNames();
}

void CT_TextFileConfigurationDialog::on_nbLines_valueChanged(int arg1)
{
    QString result = "";
    if (_file!=NULL && _file->exists() && _file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        _stream->seek(0);

        for (int i = 0 ; i < arg1 ; i++)
        {
            QString line = _stream->readLine();
            if (ui->cb_showCols->isChecked() && _separator != "\t") {line.replace(_separator, QString("%1\t").arg(_separator));}
            line.append("\n");
            result.append(line);
        }
        _file->close();
    }

    ui->fileExtract->setText(result);
}

void CT_TextFileConfigurationDialog::clearFieldCombos()
{
    // nettoyage de la zone des champs
    QMapIterator<QLabel*, QComboBox*> it(_combos);
    while (it.hasNext())
    {
        it.next();
        _layout->removeWidget(it.key());
        _layout->removeWidget(it.value());
        delete it.key();
        delete it.value();
    }
    _combos.clear();
}

void CT_TextFileConfigurationDialog::extractFieldsNames()
{
    _headers.clear();
    _headersNames.clear();


    if (_file!=NULL && _file->exists() && _file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        _stream->seek(0);
        for (int i = 0 ; i < ui->skipLines->value() ; i++)
        {
            _stream->readLine();
        }

        QString headerLine = _stream->readLine();
        QStringList headers = headerLine.split(_separator);
        int size = headers.size();
        for (int i = 0 ; i < size ; i++)
        {
            QString value = headers.at(i);

            if (hasHeader())
            {
                int cpt = 2;
                while (_headers.contains(value))
                {
                    value.append(QString("_%1").arg(cpt++));
                }
            } else {
                value = QString("COL_%1").arg(i+1);
            }

            _headers.insert(value, i);
            _headersNames.append(value);
        }
        _headers.insert("NODATA", -1);
        _headersNames.append("NODATA");

        _file->close();
    }

    //_headersNames.append("");
    clearFieldCombos();

    int sizeNeededFields = _neededFields.size();
    for (int i = 0 ; i < sizeNeededFields ; i++)
    {
        QString neededField = _neededFields.at(i);

        QLabel *label = new QLabel(QString("%1").arg(neededField));
        QComboBox *combo = new QComboBox();
        combo->addItems(_headersNames);

        if (i < _headersNames.count())
        {
            combo->setCurrentIndex(i);
        } else {
            combo->setCurrentIndex(_headersNames.count()-1);
        }

        _layout->addWidget(label, i, 0);
        _layout->addWidget(combo, i, 1);
        _combos.insert(label, combo);
    }
}

void CT_TextFileConfigurationDialog::on_buttonBox_accepted()
{
    _neededFieldsColumns.clear();
    QMapIterator<QLabel*, QComboBox*> it(_combos);
    while (it.hasNext())
    {
        it.next();
        QString needed = it.key()->text();
        QString header = it.value()->currentText();

        if (_neededFields.contains(needed) && _headers.contains(header))
        {
            _neededFieldsColumns.insert(needed, _headers.value(header));
        }
    }
}

int CT_TextFileConfigurationDialog::getNlinesToSkip()
{
    return ui->skipLines->value();
}

bool CT_TextFileConfigurationDialog::hasHeader()
{
    return !ui->cb_noheader->isChecked();
}

QString CT_TextFileConfigurationDialog::getQLocaleName()
{
    if (ui->rb_point->isChecked())
    {
        return QLocale(QLocale::English, QLocale::UnitedKingdom).name();
    } else {
        return QLocale(QLocale::French, QLocale::France).name();
    }
}

void CT_TextFileConfigurationDialog::setFileNameWithPath(const QString &path)
{
    _filename = path;
    ui->filePath->setText(_filename);
    init();
    extractFieldsNames();
}

void CT_TextFileConfigurationDialog::setNLinesToSkip(const int &n)
{
    ui->skipLines->setValue(n);
    extractFieldsNames();
}

void CT_TextFileConfigurationDialog::setFieldColumnsSelected(const QMap<QString, int> &map)
{
    extractFieldsNames();

    QMapIterator<QLabel*, QComboBox*> it(_combos);
    while (it.hasNext())
    {
        it.next();

        int headerSelected = map.value(it.key()->text(), 0);

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
        it.value()->setCurrentIndex(it.value()->findText(_headers.key(headerSelected)));
#else
        it.value()->setCurrentText(_headers.key(headerSelected));
#endif
    }
}

void CT_TextFileConfigurationDialog::setFieldColumnsSelectedFromString(QString mapAsString)
{
    QMap<QString, int> map;

    QStringList list = mapAsString.split("\n");
    for (int i = 0 ; i < list.size() ; i++)
    {
        QStringList list2 = list.at(i).split("\t");
        if (list2.size() >= 2)
        {
            bool ok;
            int n = list2.at(1).toInt(&ok);
            if (ok)
            {
                map.insert(list2.at(0), n);
            }
        }
    }
    setFieldColumnsSelected(map);
}

QString CT_TextFileConfigurationDialog::getFieldColumnsSelectedAsString(const QMap<QString, int> &map)
{
    QString result = "";
    QMapIterator<QString, int> it(map);
    while (it.hasNext())
    {
        it.next();
        result.append(it.key());
        result.append("\t");
        result.append(QString("%1").arg(it.value()));
        if (it.hasNext()) {result.append("\n");}
    }
    return result;
}


void CT_TextFileConfigurationDialog::setSeparator(const QString &separator)
{

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)

    if (separator == ",") {
        ui->separator->setCurrentIndex(ui->separator->findText("Virgule"));
    } else if (separator == ";") {
        ui->separator->setCurrentIndex(ui->separator->findText("Point-Virgule"));
    } else if (separator == " ") {
        ui->separator->setCurrentIndex(ui->separator->findText("Espace"));
    }else {
        ui->separator->setCurrentIndex(ui->separator->findText("Tabulation"));
    }
#else

    if (separator == ",") {
        ui->separator->setCurrentText("Virgule");
    } else if (separator == ";") {
        ui->separator->setCurrentText("Point-Virgule");
    } else if (separator == " ") {
        ui->separator->setCurrentText("Espace");
    } else {
        ui->separator->setCurrentText("Tabulation");
    }
#endif
}

void CT_TextFileConfigurationDialog::setFileExtensionAccepted(const QStringList &extensions)
{
    _extensions = extensions;
}

void CT_TextFileConfigurationDialog::setHeader(bool header)
{
    ui->cb_noheader->setChecked(!header);
    extractFieldsNames();
}

void CT_TextFileConfigurationDialog::setQLocale(QString locale)
{
    if (QLocale(locale).decimalPoint() == '.')
    {
        ui->rb_point->setChecked(true);
    } else {
        ui->rb_comma->setChecked(true);
    }
}

void CT_TextFileConfigurationDialog::setDecimal(QString decimal)
{
    if (decimal == ".")
    {
        ui->rb_point->setChecked(true);
    } else {
        ui->rb_comma->setChecked(true);
    }
}




void CT_TextFileConfigurationDialog::on_cb_showCols_clicked()
{
    on_nbLines_valueChanged(ui->nbLines->value());
}

void CT_TextFileConfigurationDialog::on_sb_tabSize_valueChanged(int arg1)
{
    ui->fileExtract->setTabStopWidth(arg1);
    on_nbLines_valueChanged(ui->nbLines->value());
}


void CT_TextFileConfigurationDialog::on_skipLines_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    extractFieldsNames();
}

void CT_TextFileConfigurationDialog::on_cb_noheader_clicked()
{
    extractFieldsNames();
}

void CT_TextFileConfigurationDialog::on_pb_detect_clicked()
{

    // Detect fieds separator automatically
    if (_file!=NULL && _file->exists() && _file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        _stream->seek(0);

        for (int i = 0 ; i < getNlinesToSkip() && !_stream->atEnd(); i++) {_stream->readLine();}

        if (hasHeader() && !_stream->atEnd()) {_stream->readLine();}

        if (!_stream->atEnd())
        {
            QString line = _stream->readLine();
            _file->close();

            int nbTab = line.split("\t").size();
            int nbComa = line.split(",").size();
            int nbSemicolon = line.split(";").size();
            int nbSpace = line.split(" ").size();

            if (nbTab >= nbComa && nbTab >= nbSemicolon && nbTab >= nbSpace) {
                setSeparator("\t");
            } else if (nbSemicolon >= nbComa && nbSemicolon >= nbTab && nbSemicolon >= nbSpace) {
                setSeparator(";");
            } else if (nbComa >= nbTab && nbComa >= nbSemicolon && nbComa >= nbSpace) {
                setSeparator(",");
            } else if (nbSpace >= nbComa && nbSpace >= nbTab && nbSpace >= nbSemicolon) {
                setSeparator(" ");
            }
        }
    }
}
