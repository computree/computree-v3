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

// Qt includes
#include <QPainter>
#include <QMouseEvent>

// local includes
#include "gcolorgradientview.h"

//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	ctor
/*!
//
//  \param
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  20.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
GColorGradientView::GColorGradientView(QWidget *parent)
   :QWidget(parent),
   arrowMoving(false),
   backgroundVerSpace(5),
   backgroundHorSpace(5),
   background(NULL)
{
   qRegisterMetaType<GradientArrow>("GradientArrow");
   setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
   setFocusPolicy(Qt::StrongFocus);
   viewRect = QRect(QPoint(0,0), QPoint(0,0));
   setMinimumSize(parent->width() - 10, 100);
   setMouseTracking(true);
   update();
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	dtor
/*!
//
//  \param  N/A
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  20.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
GColorGradientView::~GColorGradientView()
{
   delete background;
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	createBackground
/*!
//
//  \param  int vertLineSpace - default 3
//          int horLineSpace  - default 3
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  22.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::createBackground(int vertLineSpace, int horLineSpace)
{
   backgroundVerSpace = vertLineSpace;
   backgroundHorSpace = horLineSpace;
   if(NULL != background)
   {
      delete background;
      background = NULL;
   }
   background = new QPixmap(QSize(800,800));
   background->fill(Qt::white);
   QPainter painter(background);

   // draw vertical lines
   int x = 0;
   while(x < background->width())
   {
      const QPoint lineStart(x,0);
      const QPoint lineStop(x, background->height());
      painter.drawLine(lineStart, lineStop);
      x += vertLineSpace;
   }

   // draw horizontal lines
   int y = 0;
   while(y < background->height())
   {
      const QPoint lineStart(0, y);
      const QPoint lineStop (background->width(), y);
      painter.drawLine(lineStart, lineStop);
      y += horLineSpace;
   }
   painter.end();
   update();
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	getBackGround
/*!
//
//  \param  N/A
//
//  \return QPixmap* -
//
//  \author Stian Broen
//
//  \date  27.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
QPixmap* GColorGradientView::getBackGround()
{
   if(NULL == background)
   {
      createBackground(backgroundVerSpace, backgroundHorSpace);
   }
   return background;
}

GradientArrow GColorGradientView::currentArrow() const
{
    GradientArrow invalid;
    invalid.setIndex(-1);

    if(m_arrows.isEmpty())
        return invalid;

    QHashIterator<int, GradientArrow> it(m_arrows);

    while(it.hasNext())
    {
        it.next();

        if(it.value().hasFocus())
            return it.value();
    }

    return invalid;
}

QList<GradientArrow> GColorGradientView::orderedArrows() const
{
    QMap<qreal, GradientArrow> map;

    QHashIterator<int, GradientArrow> it(m_arrows);

    while(it.hasNext())
    {
        it.next();

        map.insert(it.value().position(), it.value());
    }

    return map.values();
}

int GColorGradientView::indexUnused() const
{
    int index = -1;

    QHashIterator<int, GradientArrow> it(m_arrows);

    while(it.hasNext())
    {
        it.next();

        if(it.value().index() > index)
            index = it.value().index();
    }

    return index+1;
}

qreal GColorGradientView::positionUnused() const
{
    QList<GradientArrow> l = orderedArrows();

    if(l.size() > 1)
    {
        return l.first().position() + ((l.first().position() + l.at(1).position())/2.0);
    }
    else if(!l.isEmpty())
    {
        return (1-l.first().position())/2.0;
    }

    return 0;
}

//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	toggleBackground
/*!
//
//  \param  bool onOrOff -
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  23.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::toggleBackground(bool onOrOff)
{
   if(onOrOff == false)
   {
      if(NULL == background)
      {
         background = new QPixmap(viewRect.size());
      }
      background->fill(Qt::transparent);
      update();
   }
   else
   {
      createBackground(backgroundVerSpace, backgroundHorSpace);
   }
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	removeArrow
/*!
//
//  \param  int index -
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  23.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::removeArrow(int index)
{
   if(!m_arrows.contains(index) || m_arrows.size() <= 2)
   {
      return;
   }
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      if(it.key() == index)
      {
         it.remove();
         break;
      }
   }
   it.toBack();
   if(it.hasPrevious())
   {
      it.previous();
      GradientArrow newFocus = it.value();
      newFocus.setHasFocus(true);
      it.setValue(newFocus);
   }
   update();
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	clearArrows
/*!
//
//  \param  N/A
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  24.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::clearArrows()
{
    m_arrows.clear();
    update();
}

int GColorGradientView::nArrows() const
{
    return m_arrows.size();
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	setFocusColor
/*!
//
//  \param  const QColor &col -
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  24.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::setFocusColor(const QColor &col)
{
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {
      it.next();
      GradientArrow anArrow = it.value();
      if(anArrow.hasFocus())
      {
         anArrow.setColor(col);
         it.setValue(anArrow);
      }
   }
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	paintEvent
/*!
//
//  \param  QPaintEvent * -
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  20.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::paintEvent(QPaintEvent *)
{
   QRect newViewRect = QRect(QPoint(0,0), QPoint(this->width() - 10, this->height()/3*2 - 10));
   newViewRect.translate(5,5);

   if(viewRect.size().isNull()  ||
      viewRect.size().isEmpty() ||
      (viewRect.topLeft() == viewRect.bottomRight()) ||
      (viewRect != newViewRect))
   {
      viewRect = QRect(QPoint(0,0), QPoint(this->width() - 10, this->height()/3*2 - 10));
      viewRect.translate(5,5);
      createBackground();
   }
   QPainter painter(this);
   if(NULL != background)
   {
      painter.drawPixmap(viewRect, *background, viewRect);
   }
   painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
   painter.setRenderHint(QPainter::Antialiasing);

   QPoint gradStart = QPoint(viewRect.topLeft ().x() ,viewRect.bottomLeft ().y()/2);
   QPoint gradStop  = QPoint(viewRect.topRight().x(), viewRect.bottomRight().y()/2);
   QLinearGradient grad(gradStart, gradStop);

   QPen lastPen = painter.pen();
   QHashIterator<int, GradientArrow> it(m_arrows);
   while(it.hasNext())
   {

      it.next();
      GradientArrow m_arrow = it.value();
      grad.setColorAt(m_arrow.position(), m_arrow.color());
      QPolygon arrowPolygon = m_arrow.area();
      arrowPolygon.translate(m_arrow.position() * (this->width() - 10), this->height()/3*2);
      QPainterPath paintPath;
      paintPath.addPolygon(arrowPolygon);
      painter.setBrush(QBrush(m_arrow.color()));
      painter.drawPath(paintPath);
      painter.setBrush(QBrush(Qt::NoBrush));

      if(m_arrow.hasFocus())
      {
         QPen boldPen;
         boldPen.setWidth(2);
         boldPen.setColor(Qt::red);
         painter.setPen(boldPen);
         painter.drawPolygon(arrowPolygon);
         painter.setPen(lastPen);
      }
      else
      {
         QPen boldPen;
         boldPen.setWidth(2);
         painter.setPen(boldPen);
         painter.drawPolygon(arrowPolygon);
         painter.setPen(lastPen);
      }
   }

   QBrush brush(grad);
   painter.fillRect(viewRect, brush);
   painter.drawRect(viewRect);
   painter.end();
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	mousePressEvent
/*!
//
//  \param  QMouseEvent *event -
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  20.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::mousePressEvent(QMouseEvent *event)
{
   dragStart = event->pos();
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   it.toBack();
   while(it.hasPrevious())
   {
      it.previous();
      GradientArrow arrow = it.value();
      QPolygon poly = arrow.area();
      poly.translate(arrow.position() * (this->width() - 10), this->height()/3*2);
      if(poly.containsPoint(dragStart, Qt::OddEvenFill))
      {
         arrowMoving = true;
         arrow.setHasFocus(true);
         emit newFocusColor(arrow.color(), arrow.index());
      }
      else
      {
         arrow.setHasFocus(false);
      }
      it.setValue(arrow);
   }
   update();
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	mouseMoveEvent
/*!
//
//  \param  QMouseEvent *event -
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  20.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::mouseMoveEvent(QMouseEvent *event)
{
   if(!arrowMoving) return;
   QMutableHashIterator<int, GradientArrow> it(m_arrows);
   int count = 0;
   while(it.hasNext())
   {
      it.next();
      GradientArrow arrow = it.value();
      if(arrow.hasFocus())
      {
         count++;
         qreal dPos = (qreal)(event->pos().x() - dragStart.x())/((qreal)this->width() - 10);
         if(arrow.position() + dPos > 1 || arrow.position() + dPos < 0)
         {
            return;
         }
         qreal lastPos = arrow.position();
         arrow.setPosition(lastPos + dPos);
         emit arrowMove(lastPos, arrow);
         it.setValue(arrow);
      }
   }
   dragStart = event->pos();
   update();
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	mouseReleaseEvent
/*!
//
//  \param  QMouseEvent * -
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  20.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::mouseReleaseEvent(QMouseEvent *)
{
   arrowMoving = false;
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	changeArrow
/*!
//
//  \param  const GradientArrow &arrow -
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  20.05.2012
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::changeArrow(const GradientArrow &arrow)
{
   if(arrow.hasFocus())
   {
      QMutableHashIterator<int, GradientArrow> it(m_arrows);
      while(it.hasNext())
      {
         it.next();
         GradientArrow anArrow = it.value();
         anArrow.setHasFocus(false);
         it.setValue(anArrow);
      }
   }
   m_arrows.insert(arrow.index(), arrow);

   if(arrow.hasFocus())
       emit newFocusColor(arrow.color(), arrow.index());

   update();
}
//f+//////////////////////////////////////////////////////////////////////////
//
//  Name:	updateView
/*!
//
//  \param  N/A
//
//  \return N/A
//
//  \author Stian Broen
//
//  \date  20.05.2013
//
*/
//f-//////////////////////////////////////////////////////////////////////////
void GColorGradientView::updateView()
{
   update();
}
