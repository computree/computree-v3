/****************************************************************************/
// This file is part of the gradLib library originally made by Stian Broen
//
// For more free libraries, please visit <http://broentech.no>
//
// gradLib is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library.  If not, see <http://www.gnu.org/licenses/>
/****************************************************************************/

#ifndef GCOLORGRADIENTVIEW_H
#define GCOLORGRADIENTVIEW_H

// Qt includes
#include <QWidget>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QHash>

// solution includes
#include "view/Tools/gradient/gradientarrow.h"

// Qt forward declarations
class QPaintEvent;
class QMouseEvent;
class QPixmap;

class GColorGradientView : public QWidget
{
    Q_OBJECT

public:
    GColorGradientView(QWidget *parent = NULL);
    ~GColorGradientView();
    void toggleBackground(bool onOrOff);
    void createBackground(int vertLineSpace = 5, int horLineSpace = 5);
    void setFocusColor(const QColor &col);
    void removeArrow(int index);
    void clearArrows();
    int nArrows() const;
    QPixmap *getBackGround();
    GradientArrow currentArrow() const;
    QList<GradientArrow> orderedArrows() const;
    int indexUnused() const;
    qreal positionUnused() const;

 protected:
    void paintEvent       (QPaintEvent *);
    void mousePressEvent  (QMouseEvent *);
    void mouseMoveEvent   (QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

 public slots:
    void changeArrow(const GradientArrow &arrow);
    void updateView();

 signals:
    void arrowMove(qreal lastPos, const GradientArrow &arrow);
    void newFocusColor(const QColor &col, int index);

 private:
    bool arrowMoving;
    int backgroundVerSpace;
    int backgroundHorSpace;
    QRect viewRect;
    QSize viewSize;
    QPixmap *background;
    QPoint dragStart;
    QHash<int, GradientArrow> m_arrows;
};

#endif // GCOLORGRADIENTVIEW_H
