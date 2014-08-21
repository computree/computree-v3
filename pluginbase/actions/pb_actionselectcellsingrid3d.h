#ifndef PB_ACTIONSELECTCELLSINGRID3D_H
#define PB_ACTIONSELECTCELLSINGRID3D_H


#include "views/actions/pb_actionselectcellsingrid3doptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"

#include "ct_itemdrawable/abstract/ct_abstractgrid3d.h"
#include "ct_itemdrawable/ct_grid3d.h"

#include <QRect>

class PB_ActionSelectCellsInGrid3D : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:
    PB_ActionSelectCellsInGrid3D(const CT_AbstractGrid3D* refGrid, CT_Grid3D<bool>* boolGrid);

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    void init();

    void initActiveGridForLevel();

    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);

    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

    void draw(GraphicsViewInterface &view, PainterInterface &painter);
    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    QList<size_t> computeExtends(size_t originColumn, size_t originRow);
    QList<size_t> computeColonize(size_t originColumn, size_t originRow);

    QList<size_t> computeExtends3D(size_t index);
    QList<size_t> computeColonize3D(size_t index);

    CT_AbstractAction* copy() const;

public slots:
    void updateLevel(int level);
    void updateThickness(int thickness);
    void updateDrawing();
    void updateCumulMode(PB_ActionSelectCellsInGrid3DOptions::CumulateMode mode);
    void updateSelectionMode(PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode mode);
    void copyUpwards();
    void copyFull();
    void change2D3DMode(bool mode3D);
    void changeCoef(double coef);
    void changeGridColor(QColor color);

private:
    int                                     m_status;
    size_t                                  _lastIndex;
    bool                                    _validLastIndex;
    PB_ActionSelectCellsInGrid3DOptions::GridSelectionMode  _previewMode;
    PB_ActionSelectCellsInGrid3DOptions::CumulateMode       _cumulMode;
    QList<size_t>                           _candidates;
    bool                                    _refState;
    bool                                    _mode3D;


    const CT_AbstractGrid3D*                _refGrid;
    CT_Grid3D<bool>*                        _boolGrid;
    std::vector<float>                      _active2DGrid;
    float                                   _activeMin;
    float                                   _activeMax;
    size_t                                  _level;
    size_t                                  _thickness;
    float                                   _reductionCoef;
    QColor                                  _gridColor;

    bool getCoordsInActiveLevelForMousePosition(const QMouseEvent *e, float &x, float &y, float &z);
    bool getIndexInGridForMousePosition(const QMouseEvent *e, size_t &index);
    void updateSelection(const float x, const float y, const float z);
    void updateSelection(const size_t index);
    void copyLevel(size_t refLevel, size_t first, size_t last);
    void draw2D(GraphicsViewInterface &view, PainterInterface &painter);
    void draw3D(GraphicsViewInterface &view, PainterInterface &painter);
    void appendIfNotNulValue2D(QList<size_t> &result, size_t col, size_t lin);
    void appendIfCoherentRefState(QList<size_t> &result, size_t col, size_t lin, size_t lev);
    void appendIfNotNulValue3D(QList<size_t> &result, size_t col, size_t lin, size_t lev);


};


#endif // PB_ACTIONSELECTCELLSINGRID3D_H
