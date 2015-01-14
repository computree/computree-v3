#include "ct_abstractexporter.h"

#include "private/ct_abstractexporter_p.h"
#include "ct_global/ct_context.h"
#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

CT_AbstractExporter::CT_AbstractExporter() :
    d_ptr(new CT_AbstractExporterPrivate)
{
}

CT_AbstractExporter::~CT_AbstractExporter()
{
}


QString CT_AbstractExporter::getExporterName() const
{
    return metaObject()->className();
}

QString CT_AbstractExporter::getExporterCustomName() const
{
    return getExporterName();
}

const QList<FileFormat>& CT_AbstractExporter::exportFormats() const
{
    Q_D(const CT_AbstractExporter);

    return d->_formats;
}

bool CT_AbstractExporter::setItemDrawableToExport(const QList<CT_AbstractItemDrawable *> &list)
{
    Q_D(CT_AbstractExporter);

    if(canExportItems())
        d->_items = list;

    return true;
}

bool CT_AbstractExporter::setPointsToExport(const QList<CT_AbstractCloudIndex *> &list)
{
    Q_D(CT_AbstractExporter);

    if(canExportPoints())
        d->_points = list;

    return true;
}

bool CT_AbstractExporter::setFacesToExport(const QList<CT_AbstractCloudIndex *> &list)
{
    Q_D(CT_AbstractExporter);

    if(canExportFaces())
        d->_faces = list;

    return true;
}

bool CT_AbstractExporter::setEdgesToExport(const QList<CT_AbstractCloudIndex *> &list)
{
    Q_D(CT_AbstractExporter);

    if(canExportEdges())
        d->_edges = list;

    return true;
}

bool CT_AbstractExporter::exportOnlyGroup() const
{
    Q_D(const CT_AbstractExporter);

    return d->_eOnlyGroup;
}

bool CT_AbstractExporter::canExportPoints() const
{
    Q_D(const CT_AbstractExporter);

    return d->_ePoints;
}

bool CT_AbstractExporter::canExportEdges() const
{
    Q_D(const CT_AbstractExporter);

    return d->_eEdges;
}

bool CT_AbstractExporter::canExportFaces() const
{
    Q_D(const CT_AbstractExporter);

    return d->_eFaces;
}

bool CT_AbstractExporter::canExportItems() const
{
    Q_D(const CT_AbstractExporter);

    return d->_eItems;
}

bool CT_AbstractExporter::hasSomethingToExport() const
{
    Q_D(const CT_AbstractExporter);

    return !d->_items.isEmpty() || !d->_points.isEmpty() || !d->_faces.isEmpty() || !d->_edges.isEmpty();
}

void CT_AbstractExporter::setDocumentManager(const DocumentManagerInterface *dm)
{
    Q_D(CT_AbstractExporter);

    d->m_dm = (DocumentManagerInterface*)dm;
}

void CT_AbstractExporter::setMyDocument(const DocumentInterface *di)
{
    Q_D(CT_AbstractExporter);

    d->m_myd = (DocumentInterface*)di;
}

void CT_AbstractExporter::setMyStep(const CT_VirtualAbstractStep *step)
{
    Q_D(CT_AbstractExporter);

    d->m_myStep = (CT_VirtualAbstractStep*)step;
}

bool CT_AbstractExporter::setExportFilePath(const QString &filepath)
{
    Q_D(CT_AbstractExporter);

    d->_filepath = filepath;

    return true;
}

QString CT_AbstractExporter::exportFilePath() const
{
    Q_D(const CT_AbstractExporter);

    return d->_filepath;
}

SettingsNodeGroup* CT_AbstractExporter::saveExportConfiguration() const
{
    Q_D(const CT_AbstractExporter);

    SettingsNodeGroup *root = new SettingsNodeGroup("ROOT");
    SettingsNodeGroup *group = new SettingsNodeGroup("CT_AbstractExporter");
    group->addValue(new SettingsNodeValue("Version", "1"));
    group->addValue(new SettingsNodeValue("FilePath", d->_filepath));

    root->addGroup(group);

    return root;
}

bool CT_AbstractExporter::loadExportConfiguration(const SettingsNodeGroup *root)
{
    if(root == NULL)
        return false;

    QList<SettingsNodeGroup*> groups = root->groupsByTagName("CT_AbstractExporter");

    if(groups.isEmpty())
        return false;

    QList<SettingsNodeValue*> values = groups.first()->valuesByTagName("FilePath");

    if(values.isEmpty())
        return false;

    return setExportFilePath(values.first()->value().toString());
}

bool CT_AbstractExporter::exportToFile()
{
    clearErrorMessage();

    Q_D(CT_AbstractExporter);

    d->_progress = 0;

    PS_COORDINATES_SYS_MANAGER->initUsedOfAllCoordinateSystem();

    if(!protectedExportToFile())
        return false;

    if(!PS_COORDINATES_SYS_MANAGER->wasAtLeastOneUsed())
        PS_LOG->addErrorMessage(LogInterface::exporter, tr("Exporter error ! The exporter has not used the coordinate system !"));

    d->_progress = 100;

    return true;
}

QString CT_AbstractExporter::errorMessage() const
{
    Q_D(const CT_AbstractExporter);

    return d->_errMsg;
}

void CT_AbstractExporter::clearErrorMessage()
{
    Q_D(CT_AbstractExporter);

    d->_errMsg.clear();
}

const QList<CT_AbstractItemDrawable*>& CT_AbstractExporter::itemDrawableToExport() const
{
    Q_D(const CT_AbstractExporter);

    return d->_items;
}

const QList<CT_AbstractCloudIndex *> &CT_AbstractExporter::pointsToExport() const
{
    Q_D(const CT_AbstractExporter);

    return d->_points;
}

const QList<CT_AbstractCloudIndex *> &CT_AbstractExporter::facesToExport() const
{
    Q_D(const CT_AbstractExporter);

    return d->_faces;
}

const QList<CT_AbstractCloudIndex *> &CT_AbstractExporter::edgesToExport() const
{
    Q_D(const CT_AbstractExporter);

    return d->_edges;
}

DocumentManagerInterface* CT_AbstractExporter::documentManager() const
{
    Q_D(const CT_AbstractExporter);

    return d->m_dm;
}

DocumentInterface* CT_AbstractExporter::myDocument() const
{
    Q_D(const CT_AbstractExporter);

    return d->m_myd;
}

CT_VirtualAbstractStep* CT_AbstractExporter::myStep() const
{
    Q_D(const CT_AbstractExporter);

    return d->m_myStep;
}

void CT_AbstractExporter::addNewExportFormat(const FileFormat &format)
{
    Q_D(CT_AbstractExporter);

    d->_formats.append(format);
}

void CT_AbstractExporter::setNotNeedToUseCoordinateSystem()
{
    PS_COORDINATES_SYS->informThatUsed();
}

void CT_AbstractExporter::setExportProgress(int progress)
{
    Q_D(CT_AbstractExporter);

    if(d->_progress != progress)
    {
        d->_progress = progress;

        emit exportInProgress(progress);
    }
}

void CT_AbstractExporter::setErrorMessage(const QString &err)
{
    Q_D(CT_AbstractExporter);

    d->_errMsg = err;
}

void CT_AbstractExporter::setCanExportPoints(bool e)
{
    Q_D(CT_AbstractExporter);

    d->_ePoints = e;
}

void CT_AbstractExporter::setCanExportEdges(bool e)
{
    Q_D(CT_AbstractExporter);

    d->_eEdges = e;
}

void CT_AbstractExporter::setCanExportFaces(bool e)
{
    Q_D(CT_AbstractExporter);

    d->_eFaces = e;
}

void CT_AbstractExporter::setExportOnlyGroup(bool e)
{
    Q_D(CT_AbstractExporter);

    d->_eOnlyGroup = e;
}

void CT_AbstractExporter::setCanExportItems(bool e)
{
    Q_D(CT_AbstractExporter);

    d->_eItems = e;
}
