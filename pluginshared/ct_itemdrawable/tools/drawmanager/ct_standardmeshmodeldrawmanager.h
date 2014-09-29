#ifndef CT_STANDARDMESHMODELDRAWMANAGER_H
#define CT_STANDARDMESHMODELDRAWMANAGER_H

#include "ct_itemdrawable/tools/drawmanager/abstract/ct_abstractitemdrawabledrawmanager.h"

class CT_Mesh;

class PLUGINSHAREDSHARED_EXPORT CT_StandardMeshModelDrawManager : public CT_AbstractItemDrawableDrawManager
{
public:
    CT_StandardMeshModelDrawManager(QString drawConfigurationName = "");

    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const;

    virtual void drawFaces(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh);
    virtual void beginDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh);
    virtual void drawFaceAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh);
    virtual void endDrawMultipleFace(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh);

    virtual void drawEdges(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh);
    virtual void beginDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh);
    virtual void drawEdgeAt(const size_t &index, GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh);
    virtual void endDrawMultipleEdge(GraphicsViewInterface &view, PainterInterface &painter, const CT_Mesh *mesh);

protected:

    const static QString INDEX_CONFIG_SHOW_FACES;
    const static QString INDEX_CONFIG_SHOW_EDGES;
    const static QString INDEX_CONFIG_SHOW_POINTS;

    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

    static QString staticInitConfigShowFaces();
    static QString staticInitConfigShowEdges();
    static QString staticInitConfigShowPoints();
};

#endif // CT_STANDARDMESHMODELDRAWMANAGER_H
