#ifndef DM_SINGLEITEMDRAWABLETOOSGWORKER_H
#define DM_SINGLEITEMDRAWABLETOOSGWORKER_H

#include <QRunnable>
#include <QObject>

#include <osg/Group>

#include "view/DocumentView/GraphicsViews/3D/gosggraphicsview.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_paintertoosgelements.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_geometriesconfiguration.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"

/**
 * @brief Runnable class to convert a item drawable to osg elements
 */
class DM_SingleItemDrawableToOsgWorker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit DM_SingleItemDrawableToOsgWorker(GOsgGraphicsView &view, QObject *parent = 0);

    /**
     * @brief Set the itemdrawable to convert
     */
    void setItemDrawable(CT_AbstractItemDrawable *item);

    /**
     * @brief Set the color to used for this itemdrawable
     */
    void setColor(const QColor &color);

    /**
     * @brief Set the configuration (shaders, vertex attrib, etc...) of geometries
     */
    void setGeometriesConfiguration(const DM_GeometriesConfiguration &config);

    /**
     * @brief Return the itemdrawable to convert
     */
    CT_AbstractItemDrawable* itemDrawable() const;

    /**
     * @brief Return the result of conversion
     */
    DM_PainterToOsgElementsResult result() const;

public slots:
    void compute();

protected:
    void run();

private:
    DM_PainterToOsgElements                             m_painter;
    DM_GeometriesConfiguration                          m_geometriesConfiguration;

    CT_AbstractItemDrawable                             *m_item;
    GOsgGraphicsView                                    &m_view;
    DM_PainterToOsgElementsResult                       m_result;

    static osgFX::Effect* staticCreateGlobalEffect(osg::PrimitiveSet::Mode mode, void *context);
    static osgFX::Effect* staticCreateLocalEffect(osg::PrimitiveSet::Mode mode, void *context);

signals:

    void finished(DM_SingleItemDrawableToOsgWorker *w);

public slots:

};

#endif // DM_SINGLEITEMDRAWABLETOOSGWORKER_H
