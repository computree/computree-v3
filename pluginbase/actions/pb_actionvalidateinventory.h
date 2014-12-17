#ifndef PB_ACTIONVALIDATEINVENTORY_H
#define PB_ACTIONVALIDATEINVENTORY_H


#include "views/actions/pb_actionvalidateinventoryoptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

#include <QRect>

class PB_ActionValidateInventory : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    PB_ActionValidateInventory(QMap<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*> *selectedItem,
                             QMultiMap<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*> *availableItem,
                             QMap<const CT_StandardItemGroup*, QString> *species,
                             QMap<const CT_StandardItemGroup*, QString> *ids,
                             const QStringList &speciesList);

    ~PB_ActionValidateInventory();

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

    const CT_AbstractSingularItemDrawable *chooseForItem(const QPoint &point);
    const CT_AbstractSingularItemDrawable *chooseForAttributes(const QPoint &point);
    void setAttributes(const CT_StandardItemGroup *scene);

public slots:
    void redraw();
    void updateSpeciesList(const QString &species);
    void resetCurrentItem();

private:
    QPoint      _oldPos;
    Qt::MouseButtons  _buttonsPressed;
    const CT_AbstractSingularItemDrawable* _currentItem;


    QMap<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*>           *_selectedItem;
    QMap<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*>           _initiallySelectedItem;
    QMultiMap<const CT_StandardItemGroup*, const CT_AbstractSingularItemDrawable*>      *_availableItem;
    QMap<const CT_StandardItemGroup*, QString>                                          *_species;
    QMap<const CT_StandardItemGroup*, QString>                                          *_ids;
    QStringList                                                                         _speciesList;
};


#endif // PB_ACTIONVALIDATEINVENTORY_H
