#include "dm_paintertoosgelements.h"

#include "ct_global/ct_context.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_itemdrawable/abstract/ct_abstractmeshmodel.h"
#include "ct_pointcloud/ct_internalpointcloud.h"
#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregisteredt.h"

#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Depth>
#include <osg/Version>

#include "view/DocumentView/GraphicsViews/3D/Painting/dm_geometriesconfiguration.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_drawelementsuintsynchronized.h"
#include "view/DocumentView/GraphicsViews/3D/Painting/dm_fakepainter.h"

#define MIN_DOUBLE_VALUE_IN_METERS 0.0001

#define WAS_RESULT_GROUP_KEY "DM_PainterToOsgElements"
#define WAS_RESULT_GROUP_VALUE_OK 1

QVector< QPair<double, double> > DM_PainterToOsgElements::VECTOR_CIRCLE_FASTEST = DM_PainterToOsgElements::staticInitCircleVector(DM_PainterToOsgElements::VECTOR_CIRCLE_FASTEST_SIZE);
QVector< QPair<double, double> > DM_PainterToOsgElements::VECTOR_CIRCLE_NORMAL = DM_PainterToOsgElements::staticInitCircleVector(DM_PainterToOsgElements::VECTOR_CIRCLE_NORMAL_SIZE);

DM_PainterToOsgElements::DM_OSGCylinder DM_PainterToOsgElements::CYLINDER_FASTEST = DM_PainterToOsgElements::staticInitCylinder(DM_PainterToOsgElements::staticInitCircleVector(DM_PainterToOsgElements::VECTOR_CIRCLE_FASTEST_SIZE));
DM_PainterToOsgElements::DM_OSGCylinder DM_PainterToOsgElements::CYLINDER_NORMAL = DM_PainterToOsgElements::staticInitCylinder(DM_PainterToOsgElements::staticInitCircleVector(DM_PainterToOsgElements::VECTOR_CIRCLE_NORMAL_SIZE));

std::string DM_PainterToOsgElements::NAME_GLOBAL_GEOMETRIES_GROUP = "G";
std::string DM_PainterToOsgElements::NAME_LOCAL_GEOMETRIES_GROUP = "L";

DM_PainterToOsgElements::DM_PainterToOsgElements()
{
    m_globalCoordinateSystemsLastUsedKey = 0;
    m_globalCoordinateSystemsLastUsedValue = NULL;

    m_localCoordinateSystemsLastUsedKey = 0;
    m_localCoordinateSystemsLastUsedValue = NULL;

    m_result.m_rootGroup = new osg::Group();
    m_result.m_rootGroup->setUserValue(WAS_RESULT_GROUP_KEY, WAS_RESULT_GROUP_VALUE_OK);

    m_cancel = false;
    m_useDisplayList = true;

    m_nCallEnableSetColor = 0;

    m_localVertexArray = NULL;
    m_globalVertexArray = ((CT_InternalPointCloud*)PS_REPOSITORY->internalConstPointCloud())->osgArray();
    m_localGeometriesColorArray = NULL;
    m_geometriesConfiguration = NULL;

    setColor(QColor(255, 255, 255, 255));
}

DM_PainterToOsgElements::~DM_PainterToOsgElements()
{
    qDeleteAll(m_localEffects.begin(), m_localEffects.end());
    qDeleteAll(m_globalEffects.begin(), m_globalEffects.end());
}

void DM_PainterToOsgElements::cancel()
{
    m_cancel = true;
}

bool DM_PainterToOsgElements::isCanceled() const
{
    return m_cancel;
}

void DM_PainterToOsgElements::finish()
{
    adjustSizeOfLocalVertexAttribArray();
}

DM_PainterToOsgElementsResult DM_PainterToOsgElements::results() const
{
    return m_result;
}

void DM_PainterToOsgElements::setGeometriesConfiguration(DM_GeometriesConfiguration *config)
{
    m_geometriesConfiguration = config;
}

void DM_PainterToOsgElements::setUseDisplayList(bool enable)
{
    m_useDisplayList = enable;
}

void DM_PainterToOsgElements::staticChangeColorOfItemDrawableInResult(CT_AbstractItemDrawable *item, GraphicsViewInterface *gv, osg::Group *result, const QColor &color)
{
    if(result == NULL)
        return;

    int s;
    bool localOK = false;
    bool globalOK = false;

    LocalColorArrayType::value_type osgColor = staticQColorToInternalColor(color);

    if((s = result->getNumChildren()) > 0)
    {
        for(int i=0; i<s; ++i)
        {
            osg::Group *g = result->getChild(i)->asGroup();

            if(g != NULL) {
                if((g->getName() == NAME_LOCAL_GEOMETRIES_GROUP) && !localOK) {
                    staticRecursiveChangeColorOfFirstColorArrayInGroup(g, osgColor);
                } else if((g->getName() == NAME_GLOBAL_GEOMETRIES_GROUP) && !globalOK) {
                    DM_FakePainter p;
                    p.setComputingMode(DM_FakePainter::BackupPointCloudIndex
                                       | DM_FakePainter::BackupPointCloudIndexIfEdge
                                       | DM_FakePainter::BackupPointCloudIndexIfFace);
                    item->draw(*gv, p);

                    staticChangeColorOfCloudsOfFirstColorArrayInGroup(p.pointCloudIndexBackup(), osgColor, g);
                }

                if(localOK && globalOK)
                    return;
            }

        }
    }
}

void DM_PainterToOsgElements::staticDirtyDisplayListOfGlobalElements(osg::Group *result)
{
    if(result == NULL)
        return;

    int s;

    if((s = result->getNumChildren()) > 0)
    {
        for(int i=0; i<s; ++i)
        {
            osg::Group *g = result->getChild(i)->asGroup();

            if((g->getName() == NAME_GLOBAL_GEOMETRIES_GROUP))
                staticRecursiveDirtyDisplayListOfDrawableInGroup(g);
        }
    }
}

void DM_PainterToOsgElements::staticDirtyDisplayListOfLocalElements(osg::Group *result)
{
    if(result == NULL)
        return;

    int s;

    if((s = result->getNumChildren()) > 0)
    {
        for(int i=0; i<s; ++i)
        {
            osg::Group *g = result->getChild(i)->asGroup();

            if((g->getName() == NAME_LOCAL_GEOMETRIES_GROUP))
                staticRecursiveDirtyDisplayListOfDrawableInGroup(g);
        }
    }
}

void DM_PainterToOsgElements::staticDirtyDisplayListOfAllElements(osg::Group *result)
{
    if(result == NULL)
        return;

    int s;

    if((s = result->getNumChildren()) > 0)
    {
        for(int i=0; i<s; ++i)
        {
            osg::Group *g = result->getChild(i)->asGroup();

            if((g->getName() == NAME_GLOBAL_GEOMETRIES_GROUP) ||
                    (g->getName() == NAME_LOCAL_GEOMETRIES_GROUP))
                staticRecursiveDirtyDisplayListOfDrawableInGroup(g);
        }
    }
}

osg::Group *DM_PainterToOsgElements::staticGetLocalGeometriesGroup(osg::Group *result)
{
    int s;

    if((s = result->getNumChildren()) > 0)
    {
        for(int i=0; i<s; ++i)
        {
            osg::Group *g = result->getChild(i)->asGroup();

            if(g->getName() == NAME_LOCAL_GEOMETRIES_GROUP)
                return g;
        }
    }

    return NULL;
}

osg::Group *DM_PainterToOsgElements::staticGetGlobalGeometriesGroup(osg::Group *result)
{
    int s;

    if((s = result->getNumChildren()) > 0)
    {
        for(int i=0; i<s; ++i)
        {
            osg::Group *g = result->getChild(i)->asGroup();

            if(g->getName() == NAME_GLOBAL_GEOMETRIES_GROUP)
                return g;
        }
    }

    return NULL;
}

osg::Array* DM_PainterToOsgElements::staticGetLocalVertexAttribArray(osg::Group *result, const uint &locationIndex)
{
    int s;

    if((s = result->getNumChildren()) > 0)
    {
        for(int i=0; i<s; ++i)
        {
            osg::Group *g = result->getChild(i)->asGroup();

            if(g->getName() == NAME_LOCAL_GEOMETRIES_GROUP) {
                return staticRecursiveGetVertexAttribArray(g, locationIndex);
            }
        }
    }

    return NULL;
}

osg::Group* DM_PainterToOsgElements::staticGetResultFromDrawable(osg::Drawable *drawable)
{
    osg::Node *n = drawable->getParent(0);
    int value;

    while(n != NULL) {
        if(n->getUserValue(WAS_RESULT_GROUP_KEY, value)) {
            if(value == WAS_RESULT_GROUP_VALUE_OK)
                return n->asGroup();
            else
                return NULL;
        }

        n = n->getParent(0);
    }

    return NULL;
}

void DM_PainterToOsgElements::save()
{
    m_configStack.push(m_currentConfig);
}

void DM_PainterToOsgElements::restore()
{
    m_currentConfig = m_configStack.pop();
}

void DM_PainterToOsgElements::setColor(QColor color)
{
    if(isSetColorEnable())
        m_currentConfig.m_color = staticIntColorToInternalColor(color.red(), color.green(), color.blue(), color.alpha());
}

QColor DM_PainterToOsgElements::getColor()
{
    return staticInternalColorToQColor(m_currentConfig.m_color);
}

void DM_PainterToOsgElements::setColor(int r, int g, int b)
{
    setColor(QColor(r, g, b, 255));
}

void DM_PainterToOsgElements::enableSetColor(bool enable)
{
    m_nCallEnableSetColor += (enable ? 1 : -1);
}

bool DM_PainterToOsgElements::isSetColorEnable() const
{
    return m_nCallEnableSetColor == 0;
}

void DM_PainterToOsgElements::drawPoint(const double &x, const double &y, const double &z)
{
    if(!isCanceled())
        addNewPoint_Local(x, y, z);
}

void DM_PainterToOsgElements::drawPoint(double *p)
{
    if(!isCanceled())
        addNewPoint_Local(p[0], p[1], p[2]);
}

void DM_PainterToOsgElements::drawPoint(const size_t &globalIndex)
{
    if(!isCanceled())
        addNewPoint_Global(globalIndex);
}

void DM_PainterToOsgElements::drawPointCloud(const CT_AbstractCloudIndex *pci)
{
    if(!isCanceled()) {
        size_t s = pci->size();
        size_t i = 0;

        while(i<s) {
            addNewPoint_Global(pci->constIndexAt(i));
            ++i;
        }
    }
}

void DM_PainterToOsgElements::drawOctreeOfPoints(const OctreeInterface *octree, PainterInterface::DrawOctreeModes modes)
{
    // TODO : to remove if octree was not used
}

void DM_PainterToOsgElements::drawCube(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2)
{
    if(!isCanceled())
        drawCube(x1, y1, z1, x2, y2, z2, GL_FRONT_AND_BACK, GL_LINE);
}

void DM_PainterToOsgElements::drawCube(const double &x1, const double &y1, const double &z1,
                                       const double &x2, const double &y2, const double &z2,
                                       GLenum faces, GLenum mode)
{
    Q_UNUSED(faces)

    if(!isCanceled()) {
        bool filled = (mode != GL_LINE);

        // Bottom
        addNewQuadFace_Local(x1, y1, z1,
                             x2, y1, z1,
                             x2, y2, z1,
                             x1, y2, z1,
                             filled);

        // Top
        addNewQuadFace_Local(x1, y1, z2,
                             x2, y1, z2,
                             x2, y2, z2,
                             x1, y2, z2,
                             filled);

        // Left
        addNewQuadFace_Local(x1, y1, z1,
                             x1, y2, z1,
                             x1, y2, z2,
                             x1, y1, z2,
                             filled);

        // Right
        addNewQuadFace_Local(x2, y1, z1,
                             x2, y2, z1,
                             x2, y2, z2,
                             x2, y1, z2,
                             filled);

        // Front
        addNewQuadFace_Local(x1, y1, z1,
                             x2, y1, z1,
                             x2, y1, z2,
                             x1, y1, z2,
                             filled);

        // Back
        addNewQuadFace_Local(x1, y2, z1,
                             x2, y2, z1,
                             x2, y2, z2,
                             x1, y2, z2,
                             filled);
    }
}

void DM_PainterToOsgElements::drawQuadFace(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2, const double &x3, const double &y3, const double &z3, const double &x4, const double &y4, const double &z4)
{
    if(!isCanceled()) {
        addNewQuadFace_Local(x1, y1, z1,
                             x2, y2, z2,
                             x3, y3, z3,
                             x4, y4, z4,
                             false);
    }
}

void DM_PainterToOsgElements::fillQuadFace(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2, const double &x3, const double &y3, const double &z3, const double &x4, const double &y4, const double &z4)
{

    if(!isCanceled()) {
        addNewQuadFace_Local(x1, y1, z1,
                             x2, y2, z2,
                             x3, y3, z3,
                             x4, y4, z4,
                             true);
    }
}

void DM_PainterToOsgElements::drawQuadFace(const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                                           const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                                           const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                                           const double &x4, const double &y4, const double &z4, int r4, int g4, int b4)
{
    if(!isCanceled()) {
        addNewQuadFace_Local(x1, y1, z1,
                             r1, g1, b1, 255,
                             x2, y2, z2,
                             r2, g2, b2, 255,
                             x3, y3, z3,
                             r3, g3, b3, 255,
                             x4, y4, z4,
                             r4, g4, b4, 255,
                             false);
    }
}

void DM_PainterToOsgElements::fillQuadFace(const double &x1, const double &y1, const double &z1, int r1, int g1, int b1,
                                           const double &x2, const double &y2, const double &z2, int r2, int g2, int b2,
                                           const double &x3, const double &y3, const double &z3, int r3, int g3, int b3,
                                           const double &x4, const double &y4, const double &z4, int r4, int g4, int b4)
{
    if(!isCanceled()) {
        addNewQuadFace_Local(x1, y1, z1,
                             r1, g1, b1, 255,
                             x2, y2, z2,
                             r2, g2, b2, 255,
                             x3, y3, z3,
                             r3, g3, b3, 255,
                             x4, y4, z4,
                             r4, g4, b4, 255,
                             true);
    }
}

void DM_PainterToOsgElements::drawRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z)
{    
    if(!isCanceled())
        drawQuadFace(topLeft[0], topLeft[1], z, bottomRight[0], topLeft[1], z, bottomRight[0], bottomRight[1], z, topLeft[0], bottomRight[1], z);
}

void DM_PainterToOsgElements::fillRectXY(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &z)
{
    if(!isCanceled())
        fillQuadFace(topLeft[0], topLeft[1], z, bottomRight[0], topLeft[1], z, bottomRight[0], bottomRight[1], z, topLeft[0], bottomRight[1], z);
}

void DM_PainterToOsgElements::drawRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y)
{
    if(!isCanceled())
        drawQuadFace(topLeft[0], y, topLeft[1], bottomRight[0], y, topLeft[1], bottomRight[0], y, bottomRight[1], topLeft[0], y, bottomRight[1]);
}

void DM_PainterToOsgElements::fillRectXZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &y)
{
    if(!isCanceled())
        fillQuadFace(topLeft[0], y, topLeft[1], bottomRight[0], y, topLeft[1], bottomRight[0], y, bottomRight[1], topLeft[0], y, bottomRight[1]);
}

void DM_PainterToOsgElements::drawRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x)
{
    if(!isCanceled())
        drawQuadFace(x, topLeft[0], topLeft[1], x, bottomRight[0], topLeft[1], x, bottomRight[0], bottomRight[1], x, topLeft[0], bottomRight[1]);
}

void DM_PainterToOsgElements::fillRectYZ(const Eigen::Vector2d &topLeft, const Eigen::Vector2d &bottomRight, const double &x)
{
    if(!isCanceled())
        fillQuadFace(x, topLeft[0], topLeft[1], x, bottomRight[0], topLeft[1], x, bottomRight[0], bottomRight[1], x, topLeft[0], bottomRight[1]);
}

void DM_PainterToOsgElements::drawLine(const double &x1, const double &y1, const double &z1, const double &x2, const double &y2, const double &z2)
{
    if(!isCanceled())
        addNewLine_Local(x1, y1, z1, x2, y2, z2);
}

void DM_PainterToOsgElements::drawLine(const size_t &p1GlobalIndex, const size_t &p2GlobalIndex)
{
    if(!isCanceled())
        addNewLine_Global(p1GlobalIndex, p2GlobalIndex);
}

void DM_PainterToOsgElements::drawTriangle(const double &x1, const double &y1, const double &z1,
                                           const double &x2, const double &y2, const double &z2,
                                           const double &x3, const double &y3, const double &z3)
{
    if(!isCanceled())
        addNewTriangle_Local(x1, y1, z1, x2, y2, z2, x3, y3, z3);
}

void DM_PainterToOsgElements::drawTriangle(const size_t &p1GlobalIndex, const size_t &p2GlobalIndex, const size_t &p3GlobalIndex)
{
    if(!isCanceled())
        addNewTriangle_Global(p1GlobalIndex, p2GlobalIndex, p3GlobalIndex);
}

void DM_PainterToOsgElements::drawCircle(const double &x, const double &y, const double &z, const double &radius)
{
    if(!isCanceled())
        addNewArc_Local(x, y, z, radius, 0, M_PI*2, Eigen::Vector3d(0,0,1));
}

void DM_PainterToOsgElements::drawCircle3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius)
{
    if(!isCanceled())
        addNewArc_Local(center[0], center[1], center[2], radius, 0, M_PI*2, direction);
}

void DM_PainterToOsgElements::drawEllipse(const double &x, const double &y, const double &z, const double &radiusA, const double &radiusB)
{
    if(!isCanceled())
        addNewEllipse_Local(x, y, z, radiusA, radiusB, Eigen::Vector3d(0, 0, 1));
}

void DM_PainterToOsgElements::beginPolygon()
{
    m_atLeastOnePointAddedForPolygon = false;
}

void DM_PainterToOsgElements::addPointToPolygon(const double &x, const double &y, const double &z)
{
    if(!isCanceled()) {
        if(m_atLeastOnePointAddedForPolygon)
            addNewLine_Local(m_polygonPreviousPointAdded.x(),
                               m_polygonPreviousPointAdded.y(),
                               m_polygonPreviousPointAdded.z(),
                               x, y, z);

        m_polygonPreviousPointAdded.set(x ,y, z);
        m_atLeastOnePointAddedForPolygon = true;
    }
}

void DM_PainterToOsgElements::endPolygon()
{
    m_atLeastOnePointAddedForPolygon = false;
}

void DM_PainterToOsgElements::drawCylinder(const double &x, const double &y, const double &z, const double &radius, const double &height)
{
    if(!isCanceled()) {
        if(height < MIN_DOUBLE_VALUE_IN_METERS || radius < MIN_DOUBLE_VALUE_IN_METERS)
            return;

        osg::Geometry *geo = CYLINDER_NORMAL.draw(*this);

        if(geo == NULL)
            return;

        osg::Group *g = getOrCreateGroup(DM_PainterToOsgElements::NAME_LOCAL_GEOMETRIES_GROUP);

        osg::Geode *geode = new osg::Geode;
        geode->addDrawable(geo);

        osg::PositionAttitudeTransform *tr = new osg::PositionAttitudeTransform();
        tr->setPosition(osg::Vec3d(x, y, z));
        tr->setScale(osg::Vec3(radius, radius, height));

        tr->addChild(geode);
        g->addChild(tr);
    }
}

void DM_PainterToOsgElements::drawCylinder3D(const Eigen::Vector3d &center, const Eigen::Vector3d &direction, const double &radius, const double &height)
{
    if(!isCanceled()) {
        if(height < MIN_DOUBLE_VALUE_IN_METERS || radius < MIN_DOUBLE_VALUE_IN_METERS)
            return;

        osg::Geometry *geo = CYLINDER_NORMAL.draw(*this);

        if(geo == NULL)
            return;

        Eigen::Vector3d dn = direction.normalized();

        osg::Quat quat;
        quat.makeRotate(osg::Vec3d(0, 0, 1), osg::Vec3d(dn[0], dn[1], dn[2]));

        osg::Group *g = getOrCreateGroup(DM_PainterToOsgElements::NAME_LOCAL_GEOMETRIES_GROUP);

        osg::Geode *geode = new osg::Geode;
        geode->addDrawable(geo);

        osg::PositionAttitudeTransform *tr = new osg::PositionAttitudeTransform();
        tr->setPosition(osg::Vec3d(center[0], center[1], center[2]));
        tr->setScale(osg::Vec3(radius, radius, height));
        tr->setAttitude(quat);

        tr->addChild(geode);
        g->addChild(tr);
    }
}

void DM_PainterToOsgElements::drawPyramid(const double &topX, const double &topY, const double &topZ,
                                          const double &base1X, const double &base1Y, const double &base1Z,
                                          const double &base2X, const double &base2Y, const double &base2Z,
                                          const double &base3X, const double &base3Y, const double &base3Z,
                                          const double &base4X, const double &base4Y, const double &base4Z)
{
    if(!isCanceled()) {
        addNewTriangle_Local(topX, topY, topZ, base1X, base1Y, base1Z, base2X, base2Y, base2Z);
        addNewTriangle_Local(topX, topY, topZ, base2X, base2Y, base2Z, base3X, base3Y, base3Z);
        addNewTriangle_Local(topX, topY, topZ, base3X, base3Y, base3Z, base4X, base4Y, base4Z);
        addNewTriangle_Local(topX, topY, topZ, base4X, base4Y, base4Z, base1X, base1Y, base1Z);
        addNewQuadFace_Local(base1X, base1Y, base1Z,
                             base2X, base2Y, base2Z,
                             base3X, base3Y, base3Z,
                             base4X, base4Y, base4Z,
                             true);
    }
}

void DM_PainterToOsgElements::drawPartOfSphere(const double &centerX, const double &centerY, const double &centerZ,
                                               const double &radius,
                                               const double &initTheta, const double &endTheta,
                                               const double &initPhi, const double &endPhi,
                                               bool radians)
{
    if(!isCanceled()) {

        osg::Geometry *geo = createGeometry(NULL, osg::PrimitiveSet::LINE_STRIP, &m_localEffects);

        if(geo == NULL)
            return;

        if(initTheta == endTheta)
            return;

        if(initPhi == endPhi)
            return;

        double iTheta = initTheta;
        double eTheta = endTheta;
        double iPhi = initPhi;
        double ePhi = endPhi;

        if ( !radians )
        {
            iTheta *= M_PI/180.0;
            eTheta *= M_PI/180.0;
            iPhi *= M_PI/180.0;
            ePhi *= M_PI/180.0;
        }

        osg::Vec3Array *vertexArray = (osg::Vec3Array*)geo->getVertexArray();
        LocalColorArrayType *colorArray = (LocalColorArrayType*)geo->getColorArray();

        size_t nSteps = VECTOR_CIRCLE_NORMAL_SIZE;
        size_t nVertex = nSteps*nSteps;
        size_t currentIndex = vertexArray->size();
        size_t backupBeginIndex = currentIndex;

        osg::DrawArrays *fSet = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, currentIndex, nVertex);
        osg::DrawElementsUInt *sSet = new osg::DrawElementsUInt(osg::PrimitiveSet::LINE_STRIP);

        DM_PainterToOsgElements::LocalColorArrayType::value_type color = DM_PainterToOsgElements::staticQColorToInternalColor(getColor());

        geo->addPrimitiveSet(fSet);
        geo->addPrimitiveSet(sSet);

        vertexArray->resize(currentIndex+nVertex);
        colorArray->resize(currentIndex+nVertex);
        sSet->resize(nVertex);

        double stepTheta = fabs(eTheta-iTheta) / ((double)nSteps);
        double stepPhi = fabs(ePhi-iPhi) / ((double)nSteps);

        double cosPhi, sinPhi, cosTheta, sinTheta;

        double currentTheta = iTheta;

        for ( int i=0; i<nSteps; ++i )
        {
            sinTheta = sin (currentTheta);
            cosTheta = cos (currentTheta);

            double currentPhi = iPhi;
            for ( int j=0; j<nSteps; ++j )
            {
                    sinPhi = sin (currentPhi);
                    cosPhi = cos (currentPhi);

                    (*vertexArray)[currentIndex] = osg::Vec3f(sinPhi*cosTheta,
                                                              sinPhi*sinTheta,
                                                              cosPhi);
                    (*colorArray)[currentIndex++] = color;
                    currentPhi += stepPhi;
            }

            currentTheta += stepTheta;
        }

        int k = 0;

        for ( int i=0; i<nSteps; ++i )
        {
            currentIndex = backupBeginIndex+i;

            for ( int j=0; j<nSteps; ++j )
            {
                (*sSet)[k] = currentIndex;
                currentIndex += nSteps;
                ++k;
            }
        }

        osg::Group *g = getOrCreateGroup(DM_PainterToOsgElements::NAME_LOCAL_GEOMETRIES_GROUP);

        osg::Geode *geode = new osg::Geode;
        geode->addDrawable(geo);

        osg::PositionAttitudeTransform *tr = new osg::PositionAttitudeTransform();
        tr->setPosition(osg::Vec3d(centerX, centerY, centerZ));
        tr->setScale(osg::Vec3(radius, radius, radius));

        tr->addChild(geode);
        g->addChild(tr);
    }
}

void DM_PainterToOsgElements::drawMesh(const CT_AbstractMeshModel *mesh)
{
    drawFaces(mesh);
}

void DM_PainterToOsgElements::drawFaces(const CT_AbstractMeshModel *mesh)
{
    if(!isCanceled()) {
        const CT_AbstractCloudIndex *fIndex = mesh->getFaceCloudIndex();

        if(fIndex == NULL)
            return;

        size_t globalIndex;
        size_t size = fIndex->size();

        for(size_t i=0; i<size; ++i)
        {
            fIndex->indexAt(i, globalIndex);

            const CT_Face &face = m_faceAccessor.constFaceAt(globalIndex);
            addNewTriangle_Global(face.iPointAt(0), face.iPointAt(1), face.iPointAt(2));
        }
    }
}

void DM_PainterToOsgElements::drawEdges(const CT_AbstractMeshModel *mesh)
{
    if(!isCanceled()) {
        const CT_AbstractCloudIndex *eIndex = mesh->getEdgeCloudIndex();

        if(eIndex == NULL)
            return;

        size_t globalIndex;
        size_t size = eIndex->size();

        for(size_t i=0; i<size; ++i)
        {
            eIndex->indexAt(i, globalIndex);

            const CT_Edge &edge = m_edgeAccessor.constEdgeAt(globalIndex);
            addNewLine_Global(edge.iPointAt(0), edge.iPointAt(1));
        }
    }
}

void DM_PainterToOsgElements::drawPoints(const CT_AbstractMeshModel *mesh)
{
    drawPointCloud(mesh->getPointCloudIndex());
}

QVector<QPair<double, double> > DM_PainterToOsgElements::staticInitCircleVector(int size)
{
    QVector< QPair<double, double> > vector(size);

    float inc = (osg::PI*2.0)/((double)size);
    float a = 0;

    for(int i=0; i<size; ++i)
    {
        QPair<double, double> &pair = vector[i];
        pair.first = cos(a);
        pair.second = sin(a);

        a += inc;
    }

    return vector;
}

DM_PainterToOsgElements::DM_OSGCylinder DM_PainterToOsgElements::staticInitCylinder(const QVector<QPair<double, double> > &cosSinAlpha)
{
    return DM_PainterToOsgElements::DM_OSGCylinder(cosSinAlpha);
}

osg::Group* DM_PainterToOsgElements::getOrCreateGroup(const std::string &name)
{
    osg::Group *g = m_groups.value(name, NULL);

    if(g == NULL) {
        g = new osg::Group();
        g->setName(name);

       m_groups.insert(name, g);

       m_result.m_rootGroup->addChild(g);
    }

    return g;
}

DM_PainterToOsgElements::LocalVertexArray* DM_PainterToOsgElements::getOrCreateVertexArray_Local(size_t addToSize, size_t &firstIndex)
{
    if(m_localVertexArray == NULL)
        m_localVertexArray = new LocalVertexArray();

    firstIndex = m_localVertexArray->size();

    if(addToSize > 0)
        m_localVertexArray->resize(firstIndex+addToSize);

    return m_localVertexArray;
}

osg::PositionAttitudeTransform* DM_PainterToOsgElements::getOrCreateTransformForCoordinates_Local(const double &x, const double &y, const double &z)
{
    uint key = PS_COORDINATES_SYS_MANAGER->computeUniqueKeyForCoordinates(x, y ,z);

    if((m_localCoordinateSystemsLastUsedKey != NULL) && (m_localCoordinateSystemsLastUsedKey == key)) {
        return m_localCoordinateSystemsLastUsedValue;
    }

    osg::PositionAttitudeTransform *translation = m_localCoordinateSystems.value(key, NULL);

    if(translation == NULL) {

        translation = new osg::PositionAttitudeTransform();
        m_localCoordinateSystems.insert(key, translation);

        getOrCreateGroup(DM_PainterToOsgElements::NAME_LOCAL_GEOMETRIES_GROUP)->addChild(translation);

        CT_AbstractCoordinateSystem *cs = PS_COORDINATES_SYS_MANAGER->createCoordinateSystemForCoordinates(x, y, z);

        Eigen::Vector3d v;
        cs->offset(v);
        delete cs;

        translation->setPosition(osg::Vec3d(v[0], v[1], v[2]));
    }

    m_localCoordinateSystemsLastUsedKey = key;
    m_localCoordinateSystemsLastUsedValue = translation;

    return translation;
}

osg::PositionAttitudeTransform* DM_PainterToOsgElements::getOrCreateTransformForCoordinateSystem_Global(CT_AbstractCoordinateSystem *cs)
{
    uint key = cs->uniqueKey();

    if((m_globalCoordinateSystemsLastUsedKey != NULL) && (m_globalCoordinateSystemsLastUsedKey == key)) {
        return m_globalCoordinateSystemsLastUsedValue;
    }

    osg::PositionAttitudeTransform *translation = m_globalCoordinateSystems.value(key, NULL);

    if(translation == NULL) {

        translation = new osg::PositionAttitudeTransform();
        m_globalCoordinateSystems.insert(key, translation);

        getOrCreateGroup(DM_PainterToOsgElements::NAME_GLOBAL_GEOMETRIES_GROUP)->addChild(translation);

        Eigen::Vector3d v;
        cs->offset(v);

        translation->setPosition(osg::Vec3d(v[0], v[1], v[2]));
    }

    m_globalCoordinateSystemsLastUsedKey = key;
    m_globalCoordinateSystemsLastUsedValue = translation;

    return translation;
}

osg::Geometry *DM_PainterToOsgElements::getOrCreateGeometryForTypeAndTransform_Global(osg::PrimitiveSet::Mode type, osg::PositionAttitudeTransform *transform)
{
    return getOrCreateGeometryForTypeAndTransform_LocalAndGlobal(type, transform, &m_globalEffects);
}

osg::Geometry *DM_PainterToOsgElements::getOrCreateGeometryForTypeAndTransform_Local(osg::PrimitiveSet::Mode type, osg::PositionAttitudeTransform *transform)
{
    return getOrCreateGeometryForTypeAndTransform_LocalAndGlobal(type, transform, &m_localEffects);
}

osg::Geometry* DM_PainterToOsgElements::getOrCreateGeometryForTypeAndTransform_LocalAndGlobal(osg::PrimitiveSet::Mode type,
                                                                               osg::PositionAttitudeTransform *transform,
                                                                               EffectCollection *effectsCollectionToUse)
{
    osg::Geometry *geo = NULL;
    osgFX::Effect *effect = getOrCreateEffectForTypeAndTransform_LocalAndGlobal(type, transform, effectsCollectionToUse);


    if(effect->getChild(0)->asGeode()->getDrawableList().empty()) {
        if(effectsCollectionToUse == &m_localEffects)
            geo = createGeometry(new osg::DrawElementsUInt(type), type, effectsCollectionToUse);
        else {
            DM_DrawElementsUIntSynchronized *de = new DM_DrawElementsUIntSynchronized(type);

            CT_MPCIR mpcir = PS_REPOSITORY->registerPointCloudIndex(de);
            mpcir->setAutoDeleteCloudIndex(false);

            m_result.m_pointCloudIndexRegisteredCollection.append(mpcir);

            geo = createGeometry(de, type, effectsCollectionToUse);
        }

        effect->getChild(0)->asGeode()->addDrawable(geo);
    } else {
        osg::Geode *geode = effect->getChild(0)->asGeode();
        geo = (osg::Geometry*)geode->getDrawable(geode->getDrawableList().size()-1);
    }

    return geo;
}

osgFX::Effect *DM_PainterToOsgElements::getOrCreateEffectForTypeAndTransform_LocalAndGlobal(osg::PrimitiveSet::Mode type, osg::PositionAttitudeTransform *transform, DM_PainterToOsgElements::EffectCollection *effectsCollectionToUse)
{
    osgFX::Effect *effect = NULL;

    QHash<osg::PrimitiveSet::Mode, osgFX::Effect*> *hash = effectsCollectionToUse->value(transform, NULL);

    if(hash == NULL) {
        hash = new QHash<osg::PrimitiveSet::Mode, osgFX::Effect*>();
        effectsCollectionToUse->insert(transform, hash);
    }

    if((effect = hash->value(type, NULL)) == NULL) {
        if(effectsCollectionToUse == &m_localEffects)
            effect = m_geometriesConfiguration->createLocalEffect(type);
        else
            effect = m_geometriesConfiguration->createGlobalEffect(type);

        effect->addChild(new osg::Geode());
        transform->addChild(effect);

        hash->insert(type, effect);
    }

    return effect;
}

void DM_PainterToOsgElements::addVertexIndexToGeometry_Global(const size_t &globalIndex, osg::Geometry *geo)
{
    osg::DrawElementsUInt *primitiveSet = static_cast<osg::DrawElementsUInt*>(geo->getPrimitiveSet(0));
    primitiveSet->push_back(globalIndex);
    (*(ColorArrayType*)geo->getColorArray())[globalIndex] = m_currentConfig.m_color;
}

void DM_PainterToOsgElements::addVertexToGeometryAndVertexArrayWithTransform_Local(const double &x, const double &y, const double &z,
                                                                             osg::Geometry *geo,
                                                                             LocalVertexArray *vertexArray,
                                                                             const size_t &indexInVertexArray,
                                                                             osg::PositionAttitudeTransform *translation) const
{
    osg::DrawElementsUInt *primitiveSet = static_cast<osg::DrawElementsUInt*>(geo->getPrimitiveSet(0));

    primitiveSet->push_back(indexInVertexArray);
    (*vertexArray)[indexInVertexArray] = osg::Vec3f(x - translation->getPosition().x(),
                                                    y - translation->getPosition().y(),
                                                    z - translation->getPosition().z());
}

DM_PainterToOsgElements::LocalColorArrayType* DM_PainterToOsgElements::getOrCreateColorArray_Local()
{
    if(m_localGeometriesColorArray == NULL)
        m_localGeometriesColorArray = new LocalColorArrayType();

    return m_localGeometriesColorArray;
}

DM_PainterToOsgElements::LocalColorArrayType* DM_PainterToOsgElements::getOrCreateColorArray_Local(size_t addToSize, size_t &firstIndex)
{
    m_localGeometriesColorArray = getOrCreateColorArray_Local();

    firstIndex = m_localGeometriesColorArray->size();

    if(addToSize > 0) {
        m_localGeometriesColorArray->resize(firstIndex+addToSize);

        if(firstIndex>0)
            firstIndex -= 1;
    }

    return m_localGeometriesColorArray;
}

osg::Geometry* DM_PainterToOsgElements::createGeometry(osg::PrimitiveSet *primitiveSet, osg::PrimitiveSet::Mode primitiveSetMode, EffectCollection *effectsCollectionToUse)
{
    osg::Geometry* geo = new osg::Geometry;

    if(effectsCollectionToUse == &m_globalEffects)
        geo->setVertexArray(m_globalVertexArray.get());
    else {
        size_t fIndex;
        geo->setVertexArray(getOrCreateVertexArray_Local(0, fIndex));
    }

    if(primitiveSet != NULL)
        geo->addPrimitiveSet(primitiveSet);

    geo->setUseDisplayList(m_useDisplayList);

    if(effectsCollectionToUse == &m_localEffects) {
        geo->setColorArray(getOrCreateColorArray_Local());
        geo->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

        if(m_geometriesConfiguration != NULL) {
            if(m_geometriesConfiguration->localVertexAttribArray() != NULL) {
                geo->setVertexAttribArray(m_geometriesConfiguration->localVertexAttribArrayLocationIndex(), m_geometriesConfiguration->localVertexAttribArray(), osg::Array::BIND_PER_VERTEX);
                geo->setVertexAttribNormalize(m_geometriesConfiguration->localVertexAttribArrayLocationIndex(), false);
            }
        }

    } else {
        if(m_geometriesConfiguration != NULL){
            geo->setColorArray(m_geometriesConfiguration->globalColorArray());
            geo->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

            geo->setNormalArray(m_geometriesConfiguration->globalNormalArray());
            geo->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

            if(m_geometriesConfiguration->globalVertexAttribArray() != NULL) {
                geo->setVertexAttribArray(m_geometriesConfiguration->globalVertexAttribArrayLocationIndex(), m_geometriesConfiguration->globalVertexAttribArray(), osg::Array::BIND_PER_VERTEX);
                geo->setVertexAttribNormalize(m_geometriesConfiguration->globalVertexAttribArrayLocationIndex(), false);
            }
        }
    }

    return geo;
}

void DM_PainterToOsgElements::addNewPoint_Global(const size_t &globalIndex)
{
    CT_AbstractCoordinateSystem *cs = PS_COORDINATES_SYS_MANAGER->coordinateSystemForPointAt(globalIndex);

    osg::PositionAttitudeTransform *t = getOrCreateTransformForCoordinateSystem_Global(cs);
    osg::Geometry *geo = getOrCreateGeometryForTypeAndTransform_Global(osg::PrimitiveSet::POINTS, t);

    geo = checkIfMustCreateANewDrawableForGlobalPointsAndDotItIfYes(t, geo);

    addVertexIndexToGeometry_Global(globalIndex, geo);
}

void DM_PainterToOsgElements::addNewLine_Global(const size_t &globalIndex1, const size_t &globalIndex2)
{
    // TODO : split the line if it has gone through coordinate system
    CT_AbstractCoordinateSystem *cs = PS_COORDINATES_SYS_MANAGER->coordinateSystemForPointAt(globalIndex1);

    osg::PositionAttitudeTransform *t = getOrCreateTransformForCoordinateSystem_Global(cs);
    osg::Geometry *geo = getOrCreateGeometryForTypeAndTransform_Global(osg::PrimitiveSet::LINES, t);

    addVertexIndexToGeometry_Global(globalIndex1, geo);
    addVertexIndexToGeometry_Global(globalIndex2, geo);
}

void DM_PainterToOsgElements::addNewTriangle_Global(const size_t &globalIndex1, const size_t &globalIndex2, const size_t &globalIndex3)
{
    // TODO : split the triangle if it has gone through coordinate system
    CT_AbstractCoordinateSystem *cs = PS_COORDINATES_SYS_MANAGER->coordinateSystemForPointAt(globalIndex1);

    osg::PositionAttitudeTransform *t = getOrCreateTransformForCoordinateSystem_Global(cs);
    osg::Geometry *geo = getOrCreateGeometryForTypeAndTransform_Global(osg::PrimitiveSet::TRIANGLES, t);

    addVertexIndexToGeometry_Global(globalIndex1, geo);
    addVertexIndexToGeometry_Global(globalIndex2, geo);
    addVertexIndexToGeometry_Global(globalIndex3, geo);
}

void DM_PainterToOsgElements::addNewPoint_Local(const double &x, const double &y, const double &z)
{
    size_t i;
    LocalVertexArray *verts = getOrCreateVertexArray_Local(1, i);
    LocalColorArrayType* colors = getOrCreateColorArray_Local();
    colors->push_back(m_currentConfig.m_color);

    osg::PositionAttitudeTransform *t = getOrCreateTransformForCoordinates_Local(x, y, z);

    osg::Geometry *geo = getOrCreateGeometryForTypeAndTransform_Local(osg::PrimitiveSet::POINTS, t);

    addVertexToGeometryAndVertexArrayWithTransform_Local(x, y, z, geo, verts, i, t);
}

void DM_PainterToOsgElements::addNewLine_Local(const double &x1, const double &y1, const double &z1,
                                                 const double &x2, const double &y2, const double &z2)
{    
    addNewLine_Local(x1, y1, z1, m_currentConfig.m_color,
                     x2, y2, z2, m_currentConfig.m_color);
}

void DM_PainterToOsgElements::addNewLine_Local(const double &x1, const double &y1, const double &z1,
                                               const int &r1, const int &g1, const int &b1,
                                               const double &x2, const double &y2, const double &z2,
                                               const int &r2, const int &g2, const int &b2)
{
    addNewLine_Local(x1, y1, z1, staticIntColorToInternalColor(r1, g1, b1, 255),
                     x2, y2, z2, staticIntColorToInternalColor(r2, g2, b2, 255));
}

void DM_PainterToOsgElements::addNewLine_Local(const double &x1, const double &y1, const double &z1,
                                               const LocalColorArrayType::value_type &c1,
                                               const double &x2, const double &y2, const double &z2,
                                               const LocalColorArrayType::value_type &c2)
{
    // TODO : split line if it has gone through coordinate system
    if(fabs(x2-x1) < MIN_DOUBLE_VALUE_IN_METERS
            && fabs(y2-y1) < MIN_DOUBLE_VALUE_IN_METERS
            && fabs(z2-z1) < MIN_DOUBLE_VALUE_IN_METERS)
        return;

    size_t i;
    LocalVertexArray *verts = getOrCreateVertexArray_Local(2, i);
    LocalColorArrayType* colors = getOrCreateColorArray_Local();

    osg::PositionAttitudeTransform *t = getOrCreateTransformForCoordinates_Local(x1, y1, z1);

    osg::Geometry *geo = getOrCreateGeometryForTypeAndTransform_Local(osg::PrimitiveSet::LINES, t);

    addVertexToGeometryAndVertexArrayWithTransform_Local(x1, y1, z1, geo, verts, i, t); ++i;
    addVertexToGeometryAndVertexArrayWithTransform_Local(x2, y2, z2, geo, verts, i, t);

    colors->push_back(c1);
    colors->push_back(c2);
}

void DM_PainterToOsgElements::addNewTriangle_Local(const double &x1, const double &y1, const double &z1,
                                                   const double &x2, const double &y2, const double &z2,
                                                   const double &x3, const double &y3, const double &z3)
{
    addNewTriangle_Local(x1, y1, z1,
                         m_currentConfig.m_color,
                         x2, y2, z2,
                         m_currentConfig.m_color,
                         x3, y3, z3,
                         m_currentConfig.m_color);
}

void DM_PainterToOsgElements::addNewTriangle_Local(const double &x1, const double &y1, const double &z1,
                                                   const int &r1, const int &g1, const int &b1, const int &a1,
                                                   const double &x2, const double &y2, const double &z2,
                                                   const int &r2, const int &g2, const int &b2, const int &a2,
                                                   const double &x3, const double &y3, const double &z3,
                                                   const int &r3, const int &g3, const int &b3, const int &a3)
{
    addNewTriangle_Local(x1, y1, z1,
                         staticIntColorToInternalColor(r1, g1, b1, a1),
                         x2, y2, z2,
                         staticIntColorToInternalColor(r2, g2, b2, a2),
                         x3, y3, z3,
                         staticIntColorToInternalColor(r3, g3, b3, a3));
}

void DM_PainterToOsgElements::addNewTriangle_Local(const double &x1, const double &y1, const double &z1,
                                                   const LocalColorArrayType::value_type &c1,
                                                   const double &x2, const double &y2, const double &z2,
                                                   const LocalColorArrayType::value_type &c2,
                                                   const double &x3, const double &y3, const double &z3,
                                                   const LocalColorArrayType::value_type &c3)
{
    // TODO : split triangle if it has gone through coordinate system
    size_t i;
    LocalVertexArray *verts = getOrCreateVertexArray_Local(3, i);
    LocalColorArrayType* colors = getOrCreateColorArray_Local();
    colors->push_back(c1);
    colors->push_back(c2);
    colors->push_back(c3);

    osg::PositionAttitudeTransform *t = getOrCreateTransformForCoordinates_Local(x1, y1, z1);

    osg::Geometry *geo = getOrCreateGeometryForTypeAndTransform_Local(osg::PrimitiveSet::TRIANGLES, t);

    addVertexToGeometryAndVertexArrayWithTransform_Local(x1, y1, z1, geo, verts, i, t); ++i;
    addVertexToGeometryAndVertexArrayWithTransform_Local(x2, y2, z2, geo, verts, i, t); ++i;
    addVertexToGeometryAndVertexArrayWithTransform_Local(x3, y3, z3, geo, verts, i, t);
}

void DM_PainterToOsgElements::addNewArc_Local(const double &x, const double &y, const double &z,
                                              const double &radius,
                                              const double &startAngle, const double &arcAngle,
                                              const Eigen::Vector3d &direction)
{
    // TODO : split circle if it has gone through coordinate system
    int num_segments = 59; // must always be a odd value !

    size_t indexInVertexArray;
    LocalVertexArray* pts = getOrCreateVertexArray_Local(num_segments+1, indexInVertexArray);
    LocalColorArrayType* colors = getOrCreateColorArray_Local();

    osg::PositionAttitudeTransform *t = getOrCreateTransformForCoordinates_Local(x, y, z);
    osg::Geometry *geo = getOrCreateGeometryForTypeAndTransform_Local(osg::PrimitiveSet::LINES, t);

    osg::DrawElementsUInt *primitiveSet = static_cast<osg::DrawElementsUInt*>(geo->getPrimitiveSet(0));

    Eigen::Vector3d u(0,0,0);

    if(fabs(direction(0)) >= fabs(direction(1)))
    {
        double factor = 1.0/sqrt(direction(0)*direction(0)+direction(2)*direction(2));
        u(0) = (-direction(2)*factor);
        u(2) = (direction(0)*factor);
    }
    else
    {
        double factor = 1.0/sqrt(direction(1)*direction(1)+direction(2)*direction(2));
        u(1) = (direction(2)*factor);
        u(2) = (-direction(1)*factor);
    }

    Eigen::Vector3d v = direction.cross(u);
    v.normalize();

    float theta = arcAngle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
    float currentTheta = startAngle;

    //we now start at the start angle
    float nx = (radius * cosf(currentTheta))*u(0) + (radius * sinf(currentTheta))*v(0);
    float ny = (radius * cosf(currentTheta))*u(1) + (radius * sinf(currentTheta))*v(1);
    float nz = (radius * cosf(currentTheta))*u(2) + (radius * sinf(currentTheta))*v(2);

    float fx = x-t->getPosition().x();
    float fy = y-t->getPosition().y();
    float fz = z-t->getPosition().z();

    for(int ii = 0; ii < num_segments; ii++)
    {
        primitiveSet->push_back(indexInVertexArray);
        primitiveSet->push_back(indexInVertexArray+1);

        (*pts)[indexInVertexArray++] = osg::Vec3f(nx + fx, ny + fy, nz + fz);
        colors->push_back(m_currentConfig.m_color);

        currentTheta += theta;

        nx = (radius * cosf(currentTheta))*u(0) + (radius * sinf(currentTheta))*v(0);
        ny = (radius * cosf(currentTheta))*u(1) + (radius * sinf(currentTheta))*v(1);
        nz = (radius * cosf(currentTheta))*u(2) + (radius * sinf(currentTheta))*v(2);
    }

    (*pts)[indexInVertexArray++] = osg::Vec3f(nx + fx, ny + fy, nz + fz);
    colors->push_back(m_currentConfig.m_color);
}

void DM_PainterToOsgElements::addNewEllipse_Local(const double &x, const double &y, const double &z, const double &radius1, const double &radius2, const Eigen::Vector3d &direction)
{
    // TODO : split ellipse if it has gone through coordinate system
    int num_segments = 59; // must always be a odd value !

    size_t indexInVertexArray;
    LocalVertexArray* pts = getOrCreateVertexArray_Local(num_segments+1, indexInVertexArray);
    LocalColorArrayType* colors = getOrCreateColorArray_Local();

    osg::PositionAttitudeTransform *t = getOrCreateTransformForCoordinates_Local(x, y, z);
    osg::Geometry *geo = getOrCreateGeometryForTypeAndTransform_Local(osg::PrimitiveSet::LINES, t);

    osg::DrawElementsUInt *primitiveSet = static_cast<osg::DrawElementsUInt*>(geo->getPrimitiveSet(0));

    Eigen::Vector3d u(0,0,0);

    if(fabs(direction(0)) >= fabs(direction(1)))
    {
        double factor = 1.0/sqrt(direction(0)*direction(0)+direction(2)*direction(2));
        u(0) = (-direction(2)*factor);
        u(2) = (direction(0)*factor);
    }
    else
    {
        double factor = 1.0/sqrt(direction(1)*direction(1)+direction(2)*direction(2));
        u(1) = (direction(2)*factor);
        u(2) = (-direction(1)*factor);
    }

    Eigen::Vector3d v = direction.cross(u);
    v.normalize();

    float theta = (M_PI*2) / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
    float currentTheta = 0.0;

    //we now start at the start angle
    float nx = (radius1 * cosf(currentTheta))*u(0) + (radius2 * sinf(currentTheta))*v(0);
    float ny = (radius1 * cosf(currentTheta))*u(1) + (radius2 * sinf(currentTheta))*v(1);
    float nz = (radius1 * cosf(currentTheta))*u(2) + (radius2 * sinf(currentTheta))*v(2);

    float fx = x-t->getPosition().x();
    float fy = y-t->getPosition().y();
    float fz = z-t->getPosition().z();

    for(int ii = 0; ii < num_segments; ii++)
    {
        primitiveSet->push_back(indexInVertexArray);
        primitiveSet->push_back(indexInVertexArray+1);

        (*pts)[indexInVertexArray++] = osg::Vec3f(nx + fx, ny + fy, nz + fz);
        colors->push_back(m_currentConfig.m_color);

        currentTheta += theta;

        nx = (radius1 * cosf(currentTheta))*u(0) + (radius2 * sinf(currentTheta))*v(0);
        ny = (radius1 * cosf(currentTheta))*u(1) + (radius2 * sinf(currentTheta))*v(1);
        nz = (radius1 * cosf(currentTheta))*u(2) + (radius2 * sinf(currentTheta))*v(2);
    }

    (*pts)[indexInVertexArray++] = osg::Vec3f(nx + fx, ny + fy, nz + fz);
    colors->push_back(m_currentConfig.m_color);
}

void DM_PainterToOsgElements::addNewQuadFace_Local(const double &x1, const double &y1, const double &z1,
                                                     const double &x2, const double &y2, const double &z2,
                                                     const double &x3, const double &y3, const double &z3,
                                                     const double &x4, const double &y4, const double &z4,
                                                     bool filled)
{
    addNewQuadFace_Local(x1, y1, z1,
                         m_currentConfig.m_color,
                         x2, y2, z2,
                         m_currentConfig.m_color,
                         x3, y3, z3,
                         m_currentConfig.m_color,
                         x4, y4, z4,
                         m_currentConfig.m_color,
                         filled);
}

void DM_PainterToOsgElements::addNewQuadFace_Local(const double &x1, const double &y1, const double &z1,
                                                     const int &r1, const int &g1, const int &b1, const int &a1,
                                                     const double &x2, const double &y2, const double &z2,
                                                     const int &r2, const int &g2, const int &b2, const int &a2,
                                                     const double &x3, const double &y3, const double &z3,
                                                     const int &r3, const int &g3, const int &b3, const int &a3,
                                                     const double &x4, const double &y4, const double &z4,
                                                     const int &r4, const int &g4, const int &b4, const int &a4,
                                                     bool filled)
{
    LocalColorArrayType::value_type c1 = staticIntColorToInternalColor(r1, g1, b1, a1);
    LocalColorArrayType::value_type c2 = staticIntColorToInternalColor(r2, g2, b2, a2);
    LocalColorArrayType::value_type c3 = staticIntColorToInternalColor(r3, g3, b3, a3);
    LocalColorArrayType::value_type c4 = staticIntColorToInternalColor(r4, g4, b4, a4);

    addNewQuadFace_Local(x1, y1, z1,
                         c1,
                         x2, y2, z2,
                         c2,
                         x3, y3, z3,
                         c3,
                         x4, y4, z4,
                         c4,
                         filled);
}

void DM_PainterToOsgElements::addNewQuadFace_Local(const double &x1, const double &y1, const double &z1,
                                                   const LocalColorArrayType::value_type &c1,
                                                   const double &x2, const double &y2, const double &z2,
                                                   const LocalColorArrayType::value_type &c2,
                                                   const double &x3, const double &y3, const double &z3,
                                                   const LocalColorArrayType::value_type &c3,
                                                   const double &x4, const double &y4, const double &z4,
                                                   const LocalColorArrayType::value_type &c4,
                                                   bool filled)
{
    if(filled) {
        addNewTriangle_Local(x1, y1, z1, c1, x2, y2, z2, c2, x3, y3, z3, c3);
        addNewTriangle_Local(x3, y3, z3, c3, x4, y4, z4, c4, x1, y1, z1, c1);

    } else {
        addNewLine_Local(x1, y1, z1, c1, x2, y2, z2, c2);
        addNewLine_Local(x2, y2, z2, c2, x3, y3, z3, c3);
        addNewLine_Local(x3, y3, z3, c3, x4, y4, z4, c4);
        addNewLine_Local(x4, y4, z4, c4, x1, y1, z1, c1);
    }
}

void DM_PainterToOsgElements::getMinMax(double &xmin, double &xmax, double &ymin, double &ymax, double &zmin, double &zmax, double x1, double y1, double z1, double x2, double y2, double z2)
{
    xmin = std::numeric_limits<double>::max();
    ymin = xmin;
    zmin = xmin;
    xmax = -xmin;
    ymax = xmax;
    zmax = xmax;

    xmin = qMin(xmin, x1);
    ymin = qMin(ymin, y1);
    zmin = qMin(zmin, z1);
    xmin = qMin(xmin, x2);
    ymin = qMin(ymin, y2);
    zmin = qMin(zmin, z2);

    xmax = qMax(xmax, x1);
    ymax = qMax(ymax, y1);
    zmax = qMax(zmax, z1);
    xmax = qMax(xmax, x2);
    ymax = qMax(ymax, y2);
    zmax = qMax(zmax, z2);
}

void DM_PainterToOsgElements::adjustSizeOfLocalVertexAttribArray()
{
    if((m_localVertexArray != NULL)
            && (m_geometriesConfiguration != NULL)
            && (m_geometriesConfiguration->localVertexAttribArray() != NULL)) {
        m_geometriesConfiguration->localVertexAttribArray()->resizeArray(m_localVertexArray->size());
    }
}

osg::Geometry* DM_PainterToOsgElements::checkIfMustCreateANewDrawableForGlobalPointsAndDotItIfYes(osg::PositionAttitudeTransform *t, osg::Geometry *lastGeometry)
{
    osg::Geometry *geo = lastGeometry;

    if(((DM_DrawElementsUIntSynchronized*)geo->getPrimitiveSet(0))->size() > 10000) {
        //QHash<osg::PrimitiveSet::Mode, osgFX::Effect*> *hash = m_globalEffects.value(t, NULL);

        DM_DrawElementsUIntSynchronized *de = new DM_DrawElementsUIntSynchronized(osg::PrimitiveSet::POINTS);

        CT_MPCIR mpcir = PS_REPOSITORY->registerPointCloudIndex(de);
        mpcir->setAutoDeleteCloudIndex(false);

        m_result.m_pointCloudIndexRegisteredCollection.append(mpcir);

        geo = createGeometry(de, osg::PrimitiveSet::POINTS, &m_globalEffects);

        /*hash->insertMulti(osg::PrimitiveSet::POINTS, geo);

        t->getChild(0)->asGeode()->addDrawable(geo);*/

        osgFX::Effect *effect = getOrCreateEffectForTypeAndTransform_LocalAndGlobal(osg::PrimitiveSet::POINTS, t, &m_globalEffects);
        effect->getChild(0)->asGeode()->addDrawable(geo);
    }

    return geo;
}

bool DM_PainterToOsgElements::staticRecursiveChangeColorOfFirstColorArrayInGroup(osg::Group *node, const LocalColorArrayType::value_type &osgColor, bool colorArrayAlreadyModified)
{
    uint n = node->getNumChildren();

    for(uint i=0; i<n; ++i)
    {
        osg::Node *n = node->getChild(i);
        osg::Group *g = n->asGroup();
        osg::Geode *geode = n->asGeode();

        if(geode != NULL)
        {
            int nD = geode->getNumDrawables();

            if(nD > 0) {
                osg::Geometry *geo = geode->getDrawable(0)->asGeometry();

                if(geo != NULL) {
                    ColorArrayType *colorArray = (ColorArrayType*)geo->getColorArray();

                    if(colorArray != NULL) {

                        if(!colorArrayAlreadyModified) {
                            size_t size = colorArray->size();

                            if(osgColor.a() != KEEP_ALPHA_COLOR)  {
                                for(size_t j=0; j<size; ++j)
                                    (*colorArray)[j] = osgColor;
                            } else {
                                for(size_t j=0; j<size; ++j) {
                                    LocalColorArrayType::value_type &currentOsgColor = (*colorArray)[j];
                                    currentOsgColor.set(osgColor.r(), osgColor.g(), osgColor.b(), currentOsgColor.a());
                                }
                            }
                        }

                        for(int j=0; j<nD; ++j)
                            geode->getDrawable(j)->dirtyDisplayList();

                        colorArrayAlreadyModified = true;
                    }
                }
            }
        }
        else if((g != NULL) && staticRecursiveChangeColorOfFirstColorArrayInGroup(g, osgColor, colorArrayAlreadyModified))
        {
            colorArrayAlreadyModified = true;
        }
    }

    return colorArrayAlreadyModified;
}

bool DM_PainterToOsgElements::staticChangeColorOfCloudsOfFirstColorArrayInGroup(const QList<CT_AbstractCloudIndex *> &indexes, const LocalColorArrayType::value_type &osgColor, osg::Group *node, bool colorArrayAlreadyModified)
{
    uint n = node->getNumChildren();

    for(uint i=0; i<n; ++i)
    {
        osg::Node *n = node->getChild(i);
        osg::Group *g = n->asGroup();
        osg::Geode *geode = n->asGeode();

        if(geode != NULL)
        {
            int nD = geode->getNumDrawables();

            if(nD > 0) {
                osg::Geometry *geo = geode->getDrawable(0)->asGeometry();

                if(geo != NULL) {
                    ColorArrayType *colorArray = (ColorArrayType*)geo->getColorArray();

                    if(colorArray != NULL) {

                        if(!colorArrayAlreadyModified) {
                            QListIterator<CT_AbstractCloudIndex*> it(indexes);

                            if(osgColor.a() != KEEP_ALPHA_COLOR)  {
                                while(it.hasNext()) {
                                    CT_AbstractCloudIndex *index = it.next();
                                    size_t size = index->size();

                                    for(size_t i=0; i<size; ++i) {
                                        (*colorArray)[index->indexAt(i)] = osgColor;
                                    }
                                }
                            } else {
                                while(it.hasNext()) {
                                    CT_AbstractCloudIndex *index = it.next();
                                    size_t size = index->size();

                                    for(size_t i=0; i<size; ++i) {
                                        LocalColorArrayType::value_type &currentOsgColor = (*colorArray)[index->indexAt(i)];
                                        currentOsgColor.set(osgColor.r(), osgColor.g(), osgColor.b(), currentOsgColor.a());
                                    }
                                }
                            }
                        }

                        for(int j=0; j<nD; ++j)
                            geode->getDrawable(j)->dirtyDisplayList();

                        colorArrayAlreadyModified = true;
                    }
                }
            }
        }
        else if((g != NULL) && staticChangeColorOfCloudsOfFirstColorArrayInGroup(indexes, osgColor, g, colorArrayAlreadyModified))
        {
            colorArrayAlreadyModified = true;
        }
    }

    return colorArrayAlreadyModified;
}

void DM_PainterToOsgElements::staticRecursiveDirtyDisplayListOfDrawableInGroup(osg::Group *node)
{
    uint n = node->getNumChildren();

    for(uint i=0; i<n; ++i)
    {
        osg::Node *n = node->getChild(i);
        osg::Group *g = n->asGroup();
        osg::Geode *geode = n->asGeode();

        if(geode != NULL)
        {
            int nD = geode->getNumDrawables();

            for(int j=0; j<nD; ++j)
                geode->getDrawable(j)->dirtyDisplayList();
        }
        else if(g != NULL)
            staticRecursiveDirtyDisplayListOfDrawableInGroup(g);
    }
}

osg::Array* DM_PainterToOsgElements::staticRecursiveGetVertexAttribArray(osg::Group *node, const uint &index)
{
    uint n = node->getNumChildren();

    for(uint i=0; i<n; ++i)
    {
        osg::Node *n = node->getChild(i);

        osg::Geode *geode = n->asGeode();
        osg::Group *g = n->asGroup();

        if(geode != NULL)
        {
            uint nD = geode->getNumDrawables();

            for(uint j=0; j<nD; ++j) {

                osg::Geometry *geo = geode->getDrawable(j)->asGeometry();

                if(geo != NULL) {
                    osg::Array* array = geo->getVertexAttribArray(index);

                    if(array != NULL)
                        return array;
                }
            }
        } else if(g != NULL) {
            osg::Array *array = staticRecursiveGetVertexAttribArray(g, index);

            if(array != NULL)
                return array;
        }
    }

    return NULL;
}

osg::Vec4 DM_PainterToOsgElements::staticInternalColorToFloatColor(const DM_PainterToOsgElements::LocalColorArrayType::value_type &color)
{
    #ifdef COLOR_ARRAY_FLOAT
    return color;
    #else
    return osg::Vec4(color.r()/255.0, color.g()/255.0, color.b()/255.0, color.a()/255.0);
    #endif
}

osg::Vec4ub DM_PainterToOsgElements::staticInternalColorToIntColor(const DM_PainterToOsgElements::LocalColorArrayType::value_type &color)
{
    #ifdef COLOR_ARRAY_FLOAT
    return osg::Vec4ub(color.r()*255.0, color.g()*255.0, color.b()*255.0, color.a()*255.0);
    #else
    return color;
    #endif
}

DM_PainterToOsgElements::LocalColorArrayType::value_type DM_PainterToOsgElements::staticIntColorToInternalColor(const int &r, const int &g, const int &b, const int &a)
{
    #ifdef COLOR_ARRAY_FLOAT
    return LocalColorArrayType::value_type(((double)r)/255.0, ((double)g)/255.0, ((double)b)/255.0, ((double)a)/255.0);
    #else
    return LocalColorArrayType::value_type(r, g, b, a);
    #endif
}

DM_PainterToOsgElements::LocalColorArrayType::value_type DM_PainterToOsgElements::staticQColorToInternalColor(const QColor &color)
{
    return staticIntColorToInternalColor(color.red(), color.green(), color.blue(), color.alpha());
}

QColor DM_PainterToOsgElements::staticInternalColorToQColor(const DM_PainterToOsgElements::LocalColorArrayType::value_type &color)
{
    #ifdef COLOR_ARRAY_FLOAT
    return QColor(color.r()*255, color.g()*255, color.b()*255, color.a()*255);
    #else
    return QColor(color.r(), color.g(), color.b(), color.a());
    #endif
}

// DM_OSGCylinder

DM_PainterToOsgElements::DM_OSGCylinder::DM_OSGCylinder(const QVector<QPair<double, double> > &cosSinAlpha)
{
    int sides = cosSinAlpha.size();
    double minH = 0;
    double maxH = 1;

    Eigen::Vector4d v;
    v(0) = 0;
    v(1) = 1;
    v(2) = maxH;
    v(3) = 1;

    for(size_t i=0; i<sides; ++i)
    {
        const QPair<double, double> &fPair = cosSinAlpha[i];

        Eigen::Vector4d v0 = v;

        Eigen::Vector4d v1 = v0;
        v1(2) = minH;

        Eigen::Vector4d v2;
        v2(3) = 1;

        Eigen::Vector4d v3;
        v3(3) = 1;

        if(i<sides-1)
        {
            v2(0) = fPair.second;
            v2(1) = fPair.first;
            v2(2) = maxH;

            v3(0) = v2(0);
            v3(1) = v2(1);
            v3(2) = minH;

            v = v2;
        }
        else
        {
            v2(0) = 0;
            v2(1) = 1;
            v2(2) = maxH;

            v3(0) = v2(0);
            v3(1) = v2(1);
            v3(2) = minH;
        }

        m_v.push_back(v0);
        m_v.push_back(v1);
        m_v.push_back(v2);
        m_v.push_back(v3);
    }
}

osg::Geometry* DM_PainterToOsgElements::DM_OSGCylinder::draw(DM_PainterToOsgElements &painter) const
{
    int s = m_v.size();

    if(s > 0) {
        osg::Geometry *geometry = painter.createGeometry(NULL, osg::PrimitiveSet::TRIANGLES, &painter.m_localEffects);

        osg::Vec3Array *vertexArray = (osg::Vec3Array*)geometry->getVertexArray();
        LocalColorArrayType *colorArray = (LocalColorArrayType*)geometry->getColorArray();

        size_t nVertex = s*6;
        size_t currentIndex = vertexArray->size();

        geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, currentIndex, nVertex));

        int i = 0;

        DM_PainterToOsgElements::LocalColorArrayType::value_type color = DM_PainterToOsgElements::staticQColorToInternalColor(painter.getColor());

        vertexArray->resize(currentIndex+nVertex);
        colorArray->resize(currentIndex+nVertex);

        while(i<s) {

            const Eigen::Vector4d &v0 = m_v[i++];
            const Eigen::Vector4d &v1 = m_v[i++];
            const Eigen::Vector4d &v2 = m_v[i++];
            const Eigen::Vector4d &v3 = m_v[i++];

            (*vertexArray)[currentIndex] = osg::Vec3f(v0(0), v0(1), v0(2));
            (*colorArray)[currentIndex++] = color;
            (*vertexArray)[currentIndex] = osg::Vec3f(v1(0), v1(1), v1(2));
            (*colorArray)[currentIndex++] = color;
            (*vertexArray)[currentIndex] = osg::Vec3f(v2(0), v2(1), v2(2));
            (*colorArray)[currentIndex++] = color;

            (*vertexArray)[currentIndex] = osg::Vec3f(v1(0), v1(1), v1(2));
            (*colorArray)[currentIndex++] = color;
            (*vertexArray)[currentIndex] = osg::Vec3f(v3(0), v3(1), v3(2));
            (*colorArray)[currentIndex++] = color;
            (*vertexArray)[currentIndex] = osg::Vec3f(v2(0), v2(1), v2(2));
            (*colorArray)[currentIndex++] = color;

        }

        return geometry;
    }

    return NULL;
}

