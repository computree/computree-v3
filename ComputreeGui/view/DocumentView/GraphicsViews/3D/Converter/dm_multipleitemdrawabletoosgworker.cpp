#include "dm_multipleitemdrawabletoosgworker.h"

#include "view/DocumentView/GraphicsViews/3D/Picking/dm_osgpicker.h"
#include "dm_osguniformwithslot.h"

#include <osg/ShadeModel>
#include <osg/PolygonMode>

#include <QMutexLocker>

#define SHADER_INFO_LOCATION 6
//#define SHADER_COLOR_LOCATION 7

/*static const char *vertexSource = {
    "#version 120\n"
    "// selection color of points\n"
    "uniform vec4 selectionColor;\n"
    "// to check if it is selected\n"
    "uniform int checkSelected;\n"
    "// info of the point (is selected ?)\n"
    "attribute float info;\n"
    "// vertex color\n"
    "attribute vec4 myColor;\n"
    "void main(void)\n"
    "{\n"
    "    int infoInt = int(info);\n"
    "    if(infoInt == checkSelected)\n"
    "    {\n"
    "         gl_FrontColor = selectionColor;\n"
    "    }\n"
    "    else\n"
    "    {\n"
    "         gl_FrontColor = myColor;\n"
    "    }\n"
    "    gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "}\n"
};*/


static const char *vertexSource = {
    "#version 120\n"
    "// selection color of points\n"
    "uniform vec4 selectionColor;\n"
    "// to check if it is selected\n"
    "uniform int checkSelected;\n"
    "// info of the point (is selected ?)\n"
    "attribute float info;\n"
    "void main(void)\n"
    "{\n"
    "    int infoInt = int(info);\n"
    "    if(infoInt == checkSelected)\n"
    "    {\n"
    "         gl_FrontColor = selectionColor;\n"
    "    }\n"
    "    else\n"
    "    {\n"
    "         gl_FrontColor = gl_Color;\n"
    "    }\n"
    "    gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "}\n"
};

static const char *vertexSourceSecondPass = {
    "#version 120\n"
    "#extension GL_EXT_geometry_shader4 : enable\n"
    "// selection color of points\n"
    "uniform vec4 selectionColor;\n"
    "// to check if it is selected\n"
    "uniform int checkSelected;\n"
    "// info of the point (is selected ?)\n"
    "attribute float info;\n"
    "// length of normals\n"
    "uniform float normalLength;\n"
    "// color of normals\n"
    "uniform vec4 normalColor;\n"
    "// pass the normal converted to geometry shader\n"
    "varying vec4 geoNormal;\n"
    "// pass the normal color to geometry shader\n"
    "varying vec4 geoNormalColor;\n"
    "void main(void)\n"
    "{\n"
    "    int infoInt = int(info);\n"
    "    if(infoInt == checkSelected)\n"
    "    {\n"
    "         gl_FrontColor = selectionColor;\n"
    "    }\n"
    "    else\n"
    "    {\n"
    "         gl_FrontColor = gl_Color;\n"
    "    }\n"
    "    geoNormal = gl_ModelViewProjectionMatrix * (gl_Vertex + (vec4(gl_Normal, 0.0) * normalLength));\n"
    "    geoNormalColor = normalColor;\n"
    "    gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "}\n"
};

static const char* geomSource = {
    "#version 120\n"
    "#extension GL_EXT_geometry_shader4 : enable\n"
    "varying in vec4 geoNormal[];\n"
    "varying in vec4 geoNormalColor[];\n"
    "void main(void)\n"
    "{\n"
    "    gl_FrontColor = geoNormalColor[0];\n"
    "    gl_Position = gl_PositionIn[0];\n"
    "    EmitVertex();\n"
    "    gl_FrontColor = geoNormalColor[0];\n"
    "    gl_Position = geoNormal[0];\n"
    "    EmitVertex();\n"
    "    EndPrimitive();\n"
    "}\n"
};

DM_MultipleItemDrawableToOsgWorker::DM_MultipleItemDrawableToOsgWorker(GOsgGraphicsView &view) : m_view(view)
{
    m_timerComputeQueue.setInterval(100);
    m_timerComputeQueue.setSingleShot(true);

    connect(&m_timerComputeQueue, SIGNAL(timeout()), this, SLOT(computeCollection()), Qt::QueuedConnection);
    connect(this, SIGNAL(mustStartTimer()), &m_timerComputeQueue, SLOT(start()), Qt::QueuedConnection);

    connect(&m_futureWatcher, SIGNAL(finished()), this, SLOT(computeCollectionFinished()), Qt::QueuedConnection);

    m_mutex = new QMutex(QMutex::Recursive);
    m_stop = false;

    if(m_view.getDocumentView() != NULL) {
        m_geometriesConfiguration.setGlobalColorArray(dynamic_cast<GDocumentViewForGraphics*>(m_view.document())->getOrCreateGlobalColorArrayForPoints());
        m_geometriesConfiguration.setGlobalNormalArray(dynamic_cast<GDocumentViewForGraphics*>(m_view.document())->getOrCreateGlobalNormalArrayForPoints());
        m_geometriesConfiguration.setGlobalVertexAttribArray(SHADER_INFO_LOCATION, dynamic_cast<GDocumentViewForGraphics*>(m_view.document())->getOrCreateGlobalAttribArrayForPoints());
    }

    //m_geometriesConfiguration.setGlobalColorVertexAttribArrayLocationIndex(SHADER_COLOR_LOCATION);
    //m_geometriesConfiguration.setLocalColorVertexAttribArrayLocationIndex(SHADER_COLOR_LOCATION);



    initGeometriesConfigurationForGlobalElements();
    initGeometriesConfigurationForLocalElements();
}

DM_MultipleItemDrawableToOsgWorker::~DM_MultipleItemDrawableToOsgWorker()
{
    m_stop = true;

    m_mutex->lock();
    m_mutex->unlock();

    delete m_mutex;
}

bool DM_MultipleItemDrawableToOsgWorker::isConversionInProgress() const
{
    return !m_toConvert.isEmpty();
}

QHash<CT_AbstractItemDrawable *, DM_PainterToOsgElementsResult> DM_MultipleItemDrawableToOsgWorker::takeResults()
{
    QMutexLocker locker(m_mutex);

    QHash< CT_AbstractItemDrawable*, DM_PainterToOsgElementsResult > l = m_results;
    m_results.clear();

    return l;
}

uint DM_MultipleItemDrawableToOsgWorker::staticGlobalVertexAttribArrayLocationIndex()
{
    return SHADER_INFO_LOCATION;
}

uint DM_MultipleItemDrawableToOsgWorker::staticLocalVertexAttribArrayLocationIndex()
{
    return SHADER_INFO_LOCATION;
}

void initStateSet(osg::ref_ptr<osg::StateSet> ss, osg::ref_ptr<osg::Program> program, GOsgGraphicsView &view)
{
    // enable depth test
    ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::ON);

    // Enable blending, select transparent bin.
    ss->setMode( GL_BLEND, osg::StateAttribute::ON );
    ss->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

    // Set blend function
    osg::BlendFunc *bf = new osg::BlendFunc;
    bf->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ss->setAttributeAndModes(bf);

    // Conversely, disable writing to depth buffer so that
    // a transparent polygon will allow polygons behind it to shine thru.
    // OSG renders transparent polygons after opaque ones.
    /*osg::Depth* depth = new osg::Depth;
    depth->setWriteMask( false );
    geo->getOrCreateStateSet()->setAttributeAndModes( depth, osg::StateAttribute::ON );*/

    // Disable conflicting modes.
    ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    // set parameter for program on this state set
    QColor color = view.getOptions().getSelectedColor();

    DM_OsgUniformWithSlot *sColorUniform = new DM_OsgUniformWithSlot("selectionColor", osg::Vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF()));
    QObject::connect((DM_GraphicsViewOptions*)&view.constGetOptionsInternal(), SIGNAL(selectionColorChanged(QColor)), sColorUniform, SLOT(setBySlot(QColor)));

    ss->addUniform(sColorUniform);
    ss->addUniform(new osg::Uniform("checkSelected", (GLbyte)1));

    // set program to this state set
    ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
}

osg::ref_ptr<osg::Program> DM_MultipleItemDrawableToOsgWorker::createFirstPassForGlobalPoints() const
{
    osg::ref_ptr<osg::Program> program = new osg::Program;

    if(program->addShader(new osg::Shader( osg::Shader::VERTEX, vertexSource))) {
        program->addBindAttribLocation("info", SHADER_INFO_LOCATION);
    }

    return program;
}

osg::ref_ptr<osg::Program> DM_MultipleItemDrawableToOsgWorker::createSecondPassForGlobalPoints() const
{
    osg::ref_ptr<osg::Program> program = new osg::Program;

    if(program->addShader(new osg::Shader( osg::Shader::VERTEX, vertexSourceSecondPass))
            && program->addShader(new osg::Shader( osg::Shader::GEOMETRY, geomSource))) {
        program->addBindAttribLocation("info", SHADER_INFO_LOCATION);

        program->setParameter( GL_GEOMETRY_VERTICES_OUT_EXT, 2 );
        program->setParameter( GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS );
        program->setParameter( GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_LINE_STRIP );
    }

    return program;
}

DM_GeometriesConfiguration DM_MultipleItemDrawableToOsgWorker::getGeometriesConfiguration() const
{
    DM_GeometriesConfiguration newConfig = m_geometriesConfiguration;
    newConfig.setLocalVertexAttribArray(SHADER_INFO_LOCATION, new DM_OsgPicker::LocalVertexAttribArray(1));

    return newConfig;
}

void DM_MultipleItemDrawableToOsgWorker::initGeometriesConfigurationForGlobalElements()
{
    // create the shader for first pass on elements that use global points
    osg::ref_ptr<osg::Program > program = createFirstPassForGlobalPoints();

    // create a state set for generic elements
    osg::ref_ptr<osg::StateSet> ss = new osg::StateSet;

    // init the state set and add the program to it
    initStateSet(ss, program, m_view);

    // add the state set to configuration so it will be used for all elements
    m_geometriesConfiguration.setGlobalGeometriesStateSet(QList< osg::ref_ptr<osg::StateSet> >() << ss);

    // ------------

    // create a state set for osg::PrimitiveSet::QUADS elements that was a copy of the previous state set
    osg::StateSet *ssQuads = new osg::StateSet(*ss.get(), osg::CopyOp::DEEP_COPY_ALL);
    // add a specific attribute
    ssQuads->setAttribute( new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL ));

    // add it to configuration so it will be used for all elements of type osg::PrimitiveSet::QUADS
    m_geometriesConfiguration.setGlobalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::QUADS, QList< osg::ref_ptr<osg::StateSet> >() << ssQuads);

    // -----------

    osg::ref_ptr<osg::Program > programSecondPass = createSecondPassForGlobalPoints();

    // create a state set for generic elements
    osg::ref_ptr<osg::StateSet> ssSecondPass = new osg::StateSet;

    // init the state set and add the program to it
    initStateSet(ssSecondPass, programSecondPass, m_view);

    QColor color = m_view.constGetOptionsInternal().normalColor();

    DM_OsgUniformWithSlot *nColorUniform = new DM_OsgUniformWithSlot("normalColor", osg::Vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF()));
    connect((DM_GraphicsViewOptions*)&m_view.constGetOptionsInternal(), SIGNAL(normalColorChanged(QColor)), nColorUniform, SLOT(setBySlot(QColor)));

    ssSecondPass->addUniform(nColorUniform);

    DM_OsgUniformWithSlot *nLengthUniform = new DM_OsgUniformWithSlot("normalLength", m_view.constGetOptionsInternal().normalLength());
    connect((DM_GraphicsViewOptions*)&m_view.constGetOptionsInternal(), SIGNAL(normalLengthChanged(float)), nLengthUniform, SLOT(setBySlot(float)));

    ssSecondPass->addUniform(nLengthUniform);

    // add the state set to configuration so it will be used for all elements
    m_geometriesConfiguration.setGlobalGeometriesStateSet(QList< osg::ref_ptr<osg::StateSet> >() << ss << ssSecondPass);
}

void DM_MultipleItemDrawableToOsgWorker::initGeometriesConfigurationForLocalElements()
{
    // get the shader for on elements that use local points (it's the same that the shader for first pass on global points)
    osg::ref_ptr<osg::Program > program = createFirstPassForGlobalPoints();

    // create a state set for generic elements
    osg::ref_ptr<osg::StateSet> ss = new osg::StateSet;

    // init the state set and add the program to it
    initStateSet(ss, program, m_view);

    // add the state set to configuration so it will be used for all elements
    m_geometriesConfiguration.setLocalGeometriesStateSet(QList< osg::ref_ptr<osg::StateSet> >() << ss);

    // ------------

    // create a state set for osg::PrimitiveSet::QUADS elements that was a copy of the previous state set
    osg::StateSet *ssQuads = new osg::StateSet(*ss.get(), osg::CopyOp::DEEP_COPY_ALL);

    // add a specific attribute
    ssQuads->setAttribute( new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL ));

    m_geometriesConfiguration.setLocalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::QUADS, QList< osg::ref_ptr<osg::StateSet> >() << ssQuads);
}

void DM_MultipleItemDrawableToOsgWorker::staticComputeQtConcurrent(DM_SingleItemDrawableToOsgWorker *worker)
{
    worker->compute();
}

void DM_MultipleItemDrawableToOsgWorker::addItemDrawable(CT_AbstractItemDrawable &item, const QColor &defaultColor)
{
    QMutexLocker locker(m_mutex);

    DM_SingleItemDrawableToOsgWorker *worker = m_toConvert.value(&item, NULL);

    if(worker == NULL) {
        DM_GeometriesConfiguration newConfig = getGeometriesConfiguration();

        worker = new DM_SingleItemDrawableToOsgWorker(m_view);
        worker->setColor(defaultColor);
        worker->setItemDrawable(&item);
        worker->setGeometriesConfiguration(newConfig);

        m_toConvert.insert(&item, worker);
    } else {
        worker->setColor(defaultColor);
    }

    locker.unlock();

    emit mustStartTimer();
}

void DM_MultipleItemDrawableToOsgWorker::removeItemDrawable(CT_AbstractItemDrawable &item)
{
    removeItemDrawable(&item);
}

void DM_MultipleItemDrawableToOsgWorker::removeItemDrawable(CT_AbstractItemDrawable *item)
{
    QMutexLocker locker(m_mutex);

    if(!m_toConvert.isEmpty())
        delete m_toConvert.take(item);

    disconnect(item, NULL, this, NULL);
}

void DM_MultipleItemDrawableToOsgWorker::itemDrawableDestroyed(QObject *o)
{
    removeItemDrawable((CT_AbstractItemDrawable*)o);
}

void DM_MultipleItemDrawableToOsgWorker::computeCollection()
{
    m_mutex->lock();

    m_aop = GUI_MANAGER->requestExclusiveAsyncOperation();

    if(m_aop != NULL)
    {
        GUI_MANAGER->initProgressDialog(m_aop->progressDialog(), tr("Veuillez patienter pendant la conversion des items."));

        connect(&m_futureWatcher, SIGNAL(progressRangeChanged(int,int)), m_aop, SLOT(setProgressRange(int,int)), Qt::QueuedConnection);
        connect(&m_futureWatcher, SIGNAL(progressValueChanged(int)), m_aop, SLOT(setProgress(int)), Qt::QueuedConnection);

        if(!m_toConvert.isEmpty()) {
            m_toConvertTemporary = m_toConvert.values();
            m_future = QtConcurrent::map(m_toConvertTemporary, staticComputeQtConcurrent);
            m_futureWatcher.setFuture(m_future);
        } else {
            computeCollectionFinished();
        }
    }
}

void DM_MultipleItemDrawableToOsgWorker::computeCollectionFinished()
{
    QHashIterator<CT_AbstractItemDrawable*, DM_SingleItemDrawableToOsgWorker*> it(m_toConvert);

    while(it.hasNext()) {
        it.next();

        m_results.insert(it.value()->itemDrawable(), it.value()->result());

        delete it.value();
    }

    m_toConvert.clear();

    m_mutex->unlock();

    delete m_aop;
    m_aop = NULL;

    emit newResultAvailable();
}
