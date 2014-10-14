/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
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


#ifndef GDOCUMENTVIEW_H
#define GDOCUMENTVIEW_H

#include "dm_documentview.h"
#include "dm_context.h"

#include "gdocumentmanagerview.h"

#include <QMdiSubWindow>
#include <QVBoxLayout>

class MyQMdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT

public:

    MyQMdiSubWindow(DM_DocumentView &docView, QWidget *parent = 0) : QMdiSubWindow(parent)
    {
        _docView = &docView;

        setWindowIcon(QIcon(":/Icones/Icones/transparent.png"));
        setAttribute(Qt::WA_DeleteOnClose);
    }

    ~MyQMdiSubWindow()
    {
        delete _docView;
    }

    DM_DocumentView* getDocumentView()
    {
        return _docView;
    }

protected:

    void closeEvent(QCloseEvent *closeEvent)
    {
        emit toBeClosed(closeEvent);
    }

private:

    DM_DocumentView *_docView;

signals:

    void toBeClosed(QCloseEvent *closeEvent);
};

class GDocumentView : public DM_DocumentView
{    
    Q_OBJECT

private:

    class DM_ContextExport : public DM_Context
    {
    public:

        DM_ContextExport(QMap<QString, QList<CT_AbstractItemDrawable*> > *map,
                         QString currentKey,
                         QObject *parent = 0) : DM_Context(parent)
        {
            _map = map;
            _currentKey = currentKey;
        }

        QMap<QString, QList<CT_AbstractItemDrawable*> >    *_map;
        QString                                 _currentKey;
    };

public:
    explicit GDocumentView(GDocumentManagerView &manager, QString title);
    virtual ~GDocumentView();

    virtual void init();

    MyQMdiSubWindow *getSubWindow() const;

    QByteArray saveGeometry() const;
    bool restoreGeometry(const QByteArray &geometry);

    void addActionOptions(ActionOptionsInterface *options);
    void removeActionOptions(ActionOptionsInterface *options);

    bool isVisible() const;

private:

    MyQMdiSubWindow             *_subWindow;

    QWidget                     *_centralWidget;
    QVBoxLayout                 *_layoutItemDrawableWidget;

protected:

    QWidget* getCentralWidget() const;

private:

    void createAndAddItemDrawableWidgetContainer(QWidget *parent);

signals:

    void closed(DM_DocumentView *view);

protected slots:

    virtual void closeEvent(QCloseEvent *closeEvent);
};

#endif // GDOCUMENTVIEW_H
