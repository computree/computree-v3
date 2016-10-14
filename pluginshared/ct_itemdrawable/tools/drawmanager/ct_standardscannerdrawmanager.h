#ifndef CT_STANDARDSCANNERDRAWMANAGER_H
#define CT_STANDARDSCANNERDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractitemdrawablewithoutpointclouddrawmanager.h"

#include <QObject>

class CT_Scanner;

class PLUGINSHAREDSHARED_EXPORT CT_StandardScannerDrawManager : public CT_StandardAbstractItemDrawableWithoutPointCloudDrawManager
{
public:
    CT_StandardScannerDrawManager(QString drawConfigurationName = QObject::tr("Scan position"));
    virtual ~CT_StandardScannerDrawManager();

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

protected :

    const static QString INDEX_CONFIG_FIELD_OF_VIEW;

    static QString staticInitConfigFieldOfView();

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

    /*!
    *  \brief Draws a part of sphere representing the scanner's field of view
    *
    *  \param painter : painter that manages the drawings
    */
    void drawFieldOfView(PainterInterface &painter, const CT_Scanner &scanner) const;

    /*!
    *  \brief Draws a line from the position of the scanner to the point on the unit sphere with spherical coordinates (theta, phi, 1)
    *
    *  \param painter : painter that manages the drawings
    */
    void drawLineToPosition(PainterInterface &painter, const CT_Scanner &scanner, double theta, double phi) const;
};

#endif // CT_STANDARDSCANNERDRAWMANAGER_H
