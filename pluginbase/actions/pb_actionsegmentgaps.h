#ifndef PB_ACTIONSEGMENTGAPS_H
#define PB_ACTIONSEGMENTGAPS_H


#include "views/actions/pb_actionsegmentgapsoptions.h"
#include "ct_actions/abstract/ct_abstractactionforgraphicsview.h"
#include "ct_itemdrawable/ct_grid2dxy.h"

#include <QRect>

class PB_ActionSegmentGaps : public CT_AbstractActionForGraphicsView
{
    Q_OBJECT
public:

    class UndoRedoContent
    {
    public:
        UndoRedoContent();
        UndoRedoContent(const int newCluster, const QMap<size_t, int> &indices);
        UndoRedoContent(const int newCluster);
        void addIndice(const size_t indice, const int oldCluster);
        void addIndice(const size_t indice, const int oldCluster, const int newCluster);
        int                 _newCluster;
        QMap<size_t, int>   _oldClusters;
        QMap<size_t, int>   _newClusters;
        bool                _useNewClustersMap;
    };

    PB_ActionSegmentGaps(const CT_Grid2DXY<int>* densityGrid, const CT_Grid2DXY<float>* mnsGrid, CT_Grid2DXY<int>* clustersGrid);

    ~PB_ActionSegmentGaps();

    QString uniqueName() const;
    QString title() const;
    QString description() const;
    QIcon icon() const;
    QString type() const;

    const QColor *getNextColor();

    void init();    
    void initClusters();

    void drawLimit(PB_ActionSegmentGapsOptions *option, size_t maxCol, size_t maxRow);

    void fillCellsInList(QList<size_t> &liste, const int cluster, bool record = true);

    QList<size_t> computeColonize(size_t originColumn, size_t originRow);
    void appendIfNotNulValue(QList<size_t> &result, size_t col, size_t lin);

    void fillCluter(const size_t col, const size_t row, const int fillingCluster);
    void fillAllCluterPixels(const size_t col, const size_t row, const int fillingCluster);
    void appendIfSameCluster(QList<size_t> &result, size_t col, size_t lin, int cluster);

    bool getCoordsForMousePosition(const QMouseEvent *e, float &x, float &y);

    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);

    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

    void draw(GraphicsViewInterface &view, PainterInterface &painter);
    void drawOverlay(GraphicsViewInterface &view, QPainter &painter);

    void drawPencil(PainterInterface &painter, PB_ActionSegmentGapsOptions *option, const float &resolution, const double &z_val);

    CT_AbstractAction* copy() const;

    void addUndoContent(UndoRedoContent* content);

public slots:
    void redraw();
    void addCluster(bool updateUi = true);
    void changeActiveClusterColor();
    void undo();
    void redo();
    void mergeClusters(int pixelNumber);

private:
    int                                     m_status;
    QList<UndoRedoContent*>                  _undoList;
    QList<UndoRedoContent*>                  _redoList;

    size_t                                  _lastIndex;
    const CT_Grid2DXY<int>*                 _densityGrid;
    const CT_Grid2DXY<float>*               _mnsGrid;
    CT_Grid2DXY<int>*                       _clustersGrid;
    size_t                                  _activeCol;
    size_t                                  _activeRow;

    QMap<int, const QColor*>                _clusters;
    int                                     _lastCluster;
    QList<QColor>                           _colorList;
    int                                     _nextColor;

    QColor*                                 _whiteColor;


};


#endif // PB_ACTIONSEGMENTGAPS_H
