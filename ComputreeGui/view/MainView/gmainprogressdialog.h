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


#ifndef GMAINPROGRESSDIALOG_H
#define GMAINPROGRESSDIALOG_H

#include "imainprogressdialog.h"
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

class GMainProgressDialog : public IMainProgressDialog
{
    Q_OBJECT

public:
    GMainProgressDialog(QWidget *parent = NULL, Qt::WindowFlags flags = 0);

    bool isCanceled() const;
    void setData(const void *data);
    void* data() const;

protected:

    void closeEvent(QCloseEvent *event);

private:

    bool            _canClose;
    bool            m_canceled;

    QLabel          *_firstLabel;
    QProgressBar    *_firstProgressBar;

    QLabel          *_secondLabel;
    QProgressBar    *_secondProgressBar;

    QPushButton     *m_cancelButton;
    void            *m_data;

public slots:

    void setCanceled(bool val = true);

    void setCanClose(bool enable);
    void enableCanClose();
    void disableCanClose();

    void setLabelText(QString text);
    void setValue(int progress);

    void setSecondLabelText(QString text);
    void setSecondValue(int progress);

    void enableCancelButton(bool enable);
    void setCancelButtonText(const QString &text);
};

#endif // GMAINPROGRESSDIALOG_H
