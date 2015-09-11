#include "dm_multipleitemdrawabletoosgworker.h"

#include "view/DocumentView/GraphicsViews/3D/Picking/dm_osgpicker.h"

#include <osg/ShadeModel>
#include <osg/PolygonMode>

#include <QMutexLocker>

#define SHADER_INFO_LOCATION 6
//#define SHADER_COLOR_LOCATION 7

/*static const char *vertexShader = {
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


static const char *vertexShader = {
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
        m_geometriesConfiguration.setGlobalVertexAttribArray(SHADER_INFO_LOCATION, dynamic_cast<GDocumentViewForGraphics*>(m_view.document())->getOrCreateGlobalAttribArrayForPoints());
    }

    //m_geometriesConfiguration.setGlobalColorVertexAttribArrayLocationIndex(SHADER_COLOR_LOCATION);
    //m_geometriesConfiguration.setLocalColorVertexAttribArrayLocationIndex(SHADER_COLOR_LOCATION);

    createGlobalShader();
    createLocalShader();
}

DM_MultipleItemDrawableToOsgWorker::~DM_MultipleItemDrawableToOsgWorker()
{
    m_stop = true;

    m_mutex->lock();
    m_mutex->unlock();

    delete m_mutex;
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

void DM_MultipleItemDrawableToOsgWorker::createGlobalShader()
{
    osg::ref_ptr<osg::Program> program = new osg::Program;
    osg::ref_ptr<osg::Shader> shader = new osg::Shader( osg::Shader::VERTEX, vertexShader);

    if(program->addShader(shader)) {
        program->addBindAttribLocation("info", SHADER_INFO_LOCATION);
        //program->addBindAttribLocation("myColor", SHADER_COLOR_LOCATION);

        osg::ref_ptr<osg::StateSet> ss = new osg::StateSet;
        ss->setAttributeAndModes(program.get(), osg::StateAttribute::ON);

        QColor color = m_view.getOptions().getSelectedColor();

        ss->addUniform(new osg::Uniform("selectionColor", osg::Vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF())));
        ss->addUniform(new osg::Uniform("checkSelected", (GLbyte)1));
        ss->setMode(GL_DEPTH_TEST,osg::StateAttribute::ON);

        // Enable blending, select transparent bin.
        ss->setMode( GL_BLEND, osg::StateAttribute::ON );
        ss->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

        // Set blend function
        osg::BlendFunc *bf = new osg::BlendFunc;
        bf->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ss->setAttributeAndModes(bf);

        // Disable conflicting modes.
        ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

        osg::StateSet *ssQuads = new osg::StateSet(*ss.get(), osg::CopyOp::DEEP_COPY_ALL);
        ssQuads->setAttribute( new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL ));

        m_geometriesConfiguration.setGlobalGeometriesStateSet(ss);
        m_geometriesConfiguration.setGlobalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::QUADS, ssQuads);
    }
}

void DM_MultipleItemDrawableToOsgWorker::createLocalShader()
{
    if(m_geometriesConfiguration.globalStateSet(osg::PrimitiveSet::POINTS) != NULL) {

        osg::Program *program = (osg::Program*)m_geometriesConfiguration.globalStateSet(osg::PrimitiveSet::POINTS)->getAttribute(osg::StateAttribute::PROGRAM);

        if(program != NULL) {
            osg::StateSet *ss = new osg::StateSet;
            ss->setAttributeAndModes(program, osg::StateAttribute::ON);

            QColor color = m_view.getOptions().getSelectedColor();

            ss->addUniform(new osg::Uniform("selectionColor", osg::Vec4(color.redF(), color.greenF(), color.blueF(), color.alphaF())));
            ss->addUniform(new osg::Uniform("checkSelected", (GLbyte)1));
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

            osg::StateSet *ssQuads = new osg::StateSet(*ss, osg::CopyOp::DEEP_COPY_ALL);
            ssQuads->setAttribute( new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL ));

            m_geometriesConfiguration.setLocalGeometriesStateSet(ss);
            m_geometriesConfiguration.setLocalGeometriesStateSetByPrimitiveSetMode(osg::PrimitiveSet::QUADS, ssQuads);
        }
    }
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
        DM_GeometriesConfiguration newConfig = m_geometriesConfiguration;
        newConfig.setLocalVertexAttribArray(SHADER_INFO_LOCATION, new DM_OsgPicker::LocalVertexAttribArray(1));

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

        m_toConvertTemporary = m_toConvert.values();
        m_future = QtConcurrent::map(m_toConvertTemporary, staticComputeQtConcurrent);
        m_futureWatcher.setFuture(m_future);
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
