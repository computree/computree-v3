#ifndef PB_ACTIONMANUALINVENTORY_H
#define PB_ACTIONMANUALINVENTORY_H


#include "views/actions/pb_actionmanualinventoryoptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/ct_circle.h"

#include <QRect>

class PB_ActionManualInventory : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    PB_ActionManualInventory(QMap<const CT_Scene*, const CT_Circle*> *selectedDbh,
                             QMultiMap<const CT_Scene*, const CT_Circle*> *availableDbh,
                             QMap<const CT_Scene*, QString> *species,
                             QMap<const CT_Scene*, QString> *ids,
                             const QStringList &speciesList);

    ~PB_ActionManualInventory();

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);

    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

    void draw(GraphicsViewInterface &view, PainterInterface &painter);
    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    CT_AbstractAction* copy() const;

    const CT_Circle *chooseForDbh(const QPoint &point);
    const CT_Circle *chooseForAttributes(const QPoint &point);
    void setAttributes(const CT_Scene *scene);

public slots:
    void redraw();
    void updateSpeciesList(const QString &species);

private:
    QPoint      _oldPos;
    Qt::MouseButtons  _buttonsPressed;
    const CT_Circle* _currentCircle;


    QMap<const CT_Scene*, const CT_Circle*>         *_selectedDbh;
    QMultiMap<const CT_Scene*, const CT_Circle*>    *_availableDbh;
    QMap<const CT_Scene*, QString>                  *_species;
    QMap<const CT_Scene*, QString>                  *_ids;
    QStringList                                     _speciesList;
};


#endif // PB_ACTIONMANUALINVENTORY_H
