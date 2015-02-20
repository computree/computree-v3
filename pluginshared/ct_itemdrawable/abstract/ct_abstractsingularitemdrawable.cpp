#include "ct_abstractsingularitemdrawable.h"
#include <limits>
#include "math.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

// initialize default item attributes of this class for each unique number declared
CT_DEFAULT_IA_INIT(CT_AbstractSingularItemDrawable)

CT_AbstractSingularItemDrawable::CT_AbstractSingularItemDrawable() : CT_AbstractItemDrawable()
{
    _minCoordinates(0) = std::numeric_limits<double>::max();
    _minCoordinates(1) = std::numeric_limits<double>::max();
    _minCoordinates(2) = std::numeric_limits<double>::max();

    _maxCoordinates(0) = -std::numeric_limits<double>::max();
    _maxCoordinates(1) = -std::numeric_limits<double>::max();
    _maxCoordinates(2) = -std::numeric_limits<double>::max();
}

CT_AbstractSingularItemDrawable::CT_AbstractSingularItemDrawable(const CT_OutAbstractSingularItemModel *model,
                                                                 const CT_AbstractResult *result) : CT_AbstractItemDrawable(model, result)
{
    _minCoordinates(0) = std::numeric_limits<double>::max();
    _minCoordinates(1) = std::numeric_limits<double>::max();
    _minCoordinates(2) = std::numeric_limits<double>::max();

    _maxCoordinates(0) = -std::numeric_limits<double>::max();
    _maxCoordinates(1) = -std::numeric_limits<double>::max();
    _maxCoordinates(2) = -std::numeric_limits<double>::max();
}

CT_AbstractSingularItemDrawable::CT_AbstractSingularItemDrawable(const QString &modelName,
                                                                 const CT_AbstractResult *result) : CT_AbstractItemDrawable(modelName, result)
{
    _minCoordinates(0) = std::numeric_limits<double>::max();
    _minCoordinates(1) = std::numeric_limits<double>::max();
    _minCoordinates(2) = std::numeric_limits<double>::max();

    _maxCoordinates(0) = -std::numeric_limits<double>::max();
    _maxCoordinates(1) = -std::numeric_limits<double>::max();
    _maxCoordinates(2) = -std::numeric_limits<double>::max();
}

QString CT_AbstractSingularItemDrawable::staticGetType()
{
    return CT_AbstractItemDrawable::staticGetType() + "/CT_AbstractSingularItemDrawable";
}

void CT_AbstractSingularItemDrawable::changeResult(const CT_AbstractResult *newRes)
{
    QList<CT_AbstractItemAttribute*> l = m_itemAttributes.itemAttributesFromResult(result());
    QListIterator<CT_AbstractItemAttribute*> it(l);

    while(it.hasNext())
        it.next()->changeResult(newRes);

    CT_AbstractItemDrawable::changeResult(newRes);
}

bool CT_AbstractSingularItemDrawable::addItemAttribute(CT_AbstractItemAttribute *att)
{
    return m_itemAttributes.addItemAttribute(att);
}

CT_AbstractItemAttribute* CT_AbstractSingularItemDrawable::itemAttribute(const CT_OutAbstractItemAttributeModel *outModel) const
{
    if(outModel->originalModel() != outModel)
        return PS_DIAM->itemAttributeFromModel(outModel, getType());

    return m_itemAttributes.itemAttributeFromModel(outModel);
}

CT_AbstractItemAttribute* CT_AbstractSingularItemDrawable::itemAttributeByOUTModelName(const CT_VirtualAbstractStep *step, const QString &modelName) const
{
    CT_AbstractModel *model = PS_MODELS->searchModel(modelName, result(), step);

    Q_ASSERT_X(model != NULL, "CT_AbstractSingularItemDrawable itemAttributeByOUTModelName", "You search a item attribute with a modelName but the model was not found");

    CT_InAbstractItemAttributeModel *inModel = dynamic_cast<CT_InAbstractItemAttributeModel*>(model);
    CT_OutAbstractItemAttributeModel *outModel = NULL;

    if(inModel != NULL)
        return firstItemAttribute(inModel);
    else if((outModel = dynamic_cast<CT_OutAbstractItemAttributeModel*>(model)) != NULL)
        return itemAttribute(outModel);
    else
        qFatal("You search a item attribute with a model name but it was not a item attribute model");

    return NULL;
}

QList<CT_AbstractItemAttribute*> CT_AbstractSingularItemDrawable::itemAttributes(const CT_InAbstractItemAttributeModel *inModel) const
{
    QList<CT_AbstractItemAttribute*> l = m_itemAttributes.itemAttributesFromModel(inModel);

    if(l.isEmpty())
        return PS_DIAM->itemAttributesFromModel(inModel, getType());

    return l;
}

QList<CT_AbstractItemAttribute *> CT_AbstractSingularItemDrawable::itemAttributes() const
{
    QList<CT_AbstractItemAttribute *> l;

    l.append(PS_DIAM->itemAttributes(getType()));
    l.append(m_itemAttributes.itemAttributes());

    return l;
}

CT_AbstractItemAttribute* CT_AbstractSingularItemDrawable::firstItemAttribute(const CT_InAbstractItemAttributeModel *inModel) const
{
    CT_AbstractItemAttribute *l = m_itemAttributes.firstItemAttributeFromModel(inModel);

    if(l == NULL)
        return PS_DIAM->firstItemAttributeFromModel(inModel, getType());

    return l;
}

CT_AbstractItemAttribute* CT_AbstractSingularItemDrawable::firstItemAttributeByINModelName(const CT_AbstractResult *yourInResult, const CT_VirtualAbstractStep *step, const QString &modelName) const
{
    CT_AbstractModel *model = PS_MODELS->searchModel(modelName, yourInResult, step);

    Q_ASSERT_X(model != NULL, "CT_AbstractSingularItemDrawable firstItemAttributeByINModelName", "You search a item attribute with a modelName but the model was not found");

    CT_InAbstractItemAttributeModel *inModel = dynamic_cast<CT_InAbstractItemAttributeModel*>(model);
    CT_OutAbstractItemAttributeModel *outModel = NULL;

    if(inModel != NULL)
        return firstItemAttribute(inModel);
    else if((outModel = dynamic_cast<CT_OutAbstractItemAttributeModel*>(model)) != NULL)
        return itemAttribute(outModel);
    else
        qFatal("You search a item attribute with a model name but it was not a item attribute model");

    return NULL;
}

QList<CT_AbstractItemAttribute *> CT_AbstractSingularItemDrawable::defaultItemAttributes() const
{
    return PS_DIAM->itemAttributes(getType());
}

QList<CT_AbstractItemAttribute *> CT_AbstractSingularItemDrawable::notDefaultItemAttributes() const
{
    return m_itemAttributes.itemAttributes();
}

void CT_AbstractSingularItemDrawable::setBoundingBox(double minx, double miny, double minz, double maxx, double maxy, double maxz)
{
    _minCoordinates(0) = minx;
    _minCoordinates(1) = miny;
    _minCoordinates(2) = minz;

    _maxCoordinates(0) = maxx;
    _maxCoordinates(1) = maxy;
    _maxCoordinates(2) = maxz;

    updateCenterFromBoundingBox();
}

void CT_AbstractSingularItemDrawable::updateCenterFromBoundingBox()
{
    setCenterCoordinate((_maxCoordinates + _minCoordinates) / 2.0);
}

QString CT_AbstractSingularItemDrawable::internalVerifyModel(const CT_OutAbstractModel *model) const
{
    if(dynamic_cast<const CT_OutAbstractSingularItemModel*>(model) == NULL)
        return tr("Model passed in parameter is not a CT_OutAbstractGroupModel");

    return QString();
}

void CT_AbstractSingularItemDrawable::internalSetWillBeRemovedFromResult(const CT_AbstractResult *res)
{
    m_itemAttributes.removeItemAttributeFromResult(res);
}

void CT_AbstractSingularItemDrawable::getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    min = _minCoordinates;
    max = _maxCoordinates;
}


void CT_AbstractSingularItemDrawable::getScaledBoundingBox(double xCoeff, double yCoeff, double zCoeff, Eigen::Vector3d &min, Eigen::Vector3d &max)
{
    getBoundingBox(min, max);

    min(0) = getCenterX() - (fabs(min.x()-getCenterX()) * xCoeff);
    min(1) = getCenterY() - (fabs(min.y()-getCenterY()) * yCoeff);
    min(2) = getCenterZ() - (fabs(min.z()-getCenterZ()) * zCoeff);

    max(0) = getCenterX() + (fabs(max.x()-getCenterX()) * xCoeff);
    max(1) = getCenterY() + (fabs(max.y()-getCenterY()) * yCoeff);
    max(2) = getCenterZ() + (fabs(max.z()-getCenterZ()) * zCoeff);
}

void CT_AbstractSingularItemDrawable::getBufferedBoundingBox(double xMeters, double yMeters, double zMeters, Eigen::Vector3d &min, Eigen::Vector3d &max)
{
    getBoundingBox(min, max);

    min(0) -= xMeters;
    min(1) -= yMeters;
    min(2) -= zMeters;

    max(0) += xMeters;
    max(1) += yMeters;
    max(2) += zMeters;
}
