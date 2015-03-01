/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#ifndef CT_TEXTFILECONFIGURATIONDIALOG_H
#define CT_TEXTFILECONFIGURATIONDIALOG_H

#include "qmap.h"
#include "qlabel.h"
#include "qcombobox.h"
#include "qgridlayout.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qfileinfo.h"

#include <QDialog>
#include "pluginShared_global.h"

namespace Ui {
class CT_TextFileConfigurationDialog;
}

class PLUGINSHAREDSHARED_EXPORT CT_TextFileConfigurationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CT_TextFileConfigurationDialog(QStringList neededFields, QWidget *parent = 0, QString fileName="", bool autoDetect = true);
    ~CT_TextFileConfigurationDialog();

    void init();

    inline QStringList getHeadersNames() {return _headersNames;}
    inline int getFieldNumber() {return _headers.size();}
    inline QString getFileNameWithPath() {return _filename;}
    inline QString getFileNameWithoutPath() {return QFileInfo(_filename).fileName();}
    inline QString getSeparator() {return _separator;}
    int getNlinesToSkip();
    bool hasHeader();
    QString getQLocaleName();

    inline int getColumnForField(QString neededField) {return _neededFieldsColumns.value(neededField);}
    inline QMap<QString, int> getNeededFieldColumns() {return _neededFieldsColumns;}

    void setFieldColumnsSelectedFromString(QString mapAsString);
    QString getFieldColumnsSelectedAsString(const QMap<QString, int> &map);

    void setFileNameWithPath(const QString &path);
    void setNLinesToSkip(const int &n);
    void setFieldColumnsSelected(const QMap<QString, int> &map);
    void setSeparator(const QString &separator);
    void setFileExtensionAccepted(const QStringList &extensions);
    void setHeader(bool header);
    void setQLocale(QString locale);    
    void setDecimal(QString decimal);

public slots:
    void extractFieldsNames();

private slots:

    void clearFieldCombos();
    void on_fileChoose_clicked();
    void on_separator_currentIndexChanged(const QString &arg1);
    void on_nbLines_valueChanged(int arg1);
    void on_buttonBox_accepted();

    void on_cb_showCols_clicked();

    void on_sb_tabSize_valueChanged(int arg1);

    void on_skipLines_valueChanged(int arg1);

    void on_cb_noheader_clicked();

    void on_pb_detect_clicked();

private:
    Ui::CT_TextFileConfigurationDialog *ui;
    QGridLayout*                _layout;
    QMap<QLabel*, QComboBox*>   _combos;

    QString                     _filename;
    QFile*                      _file;
    QTextStream*                _stream;
    QMap<QString, int>          _headers;
    QStringList                 _headersNames;
    QMap<QString, int>          _neededFieldsColumns;
    QStringList                 _neededFields;
    QString                     _separator;
    QStringList                 _extensions;
    bool                        _autoDetect;
};

#endif // CT_TEXTFILECONFIGURATIONDIALOG_H
