/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#ifndef GITEMDRAWABLEMANAGEROPTIONSCOLOR_H
#define GITEMDRAWABLEMANAGEROPTIONSCOLOR_H

#include <QDialog>
#include <QListWidgetItem>
#include <QColorDialog>

#include "dm_itemdrawablemanageroptions.h"

namespace Ui {
    class GItemDrawableManagerOptionsColor;
}

class GItemDrawableManagerOptionsColor : public QDialog {
    Q_OBJECT
public:
    GItemDrawableManagerOptionsColor(QWidget *parent = 0);
    ~GItemDrawableManagerOptionsColor();

    void setOptions(DM_ItemDrawableManagerOptions option);
    QList<QColor> getColorList();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::GItemDrawableManagerOptionsColor *ui;
    DM_ItemDrawableManagerOptions       _option;
    QList<QColor>                       _colorList;

    QColorDialog                        *_colorDialog;

    QString                             _defaultOpenDir;
    QString                             _defaultSaveDir;

    void setColorList(QList<QColor> colorList);
    QListWidgetItem* createItemForColor(QColor color);

    QList<QColor> createColorListFromListWidget();

    QString createExtensionStringForColorFile();
    QString createFilePathWithExtension(QString filePath);

    void loadSettings();
    void saveSettings();

private slots:

    void on_pushButtonAjouter_clicked();
    void on_pushButtonSupprimer_clicked();
    void on_pushButtonOuvrir_clicked();
    void on_pushButtonSauvegarder_clicked();
    void on_pushButtonSauvegarderDefaut_clicked();
};

#endif // GITEMDRAWABLEMANAGEROPTIONSCOLOR_H
