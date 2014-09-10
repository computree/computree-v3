#include "ct_abstractsingularitemdrawable.h"
#include <limits>
#include "math.h"

// initialize default item attributes of this class for each unique number declared
CT_DEFAULT_IA_INIT(CT_AbstractSingularItemDrawable)

CT_AbstractSingularItemDrawable::CT_AbstractSingularItemDrawable() : CT_AbstractItemDrawable()
{
    _minCoordinates.setX(std::numeric_limits<float>::max());
    _minCoordinates.setY(std::numeric_limits<float>::max());
    _minCoordinates.setZ(std::numeric_limits<float>::max());

    _maxCoordinates.setX(-std::numeric_limits<float>::max());
    _maxCoordinates.setY(-std::numeric_limits<float>::max());
    _maxCoordinates.setZ(-std::numeric_limits<float>::max());
}

CT_AbstractSingularItemDrawable::CT_AbstractSingularItemDrawable(const CT_OutAbstractSingularItemModel *model,
                                                                 const CT_AbstractResult *result) : CT_AbstractItemDrawable(model, result)
{
    _minCoordinates.setX(std::numeric_limits<float>::max());
    _minCoordinates.setY(std::numeric_limits<float>::max());
    _minCoordinates.setZ(std::numeric_limits<float>::max());

    _maxCoordinates.setX(-std::numeric_limits<float>::max());
    _maxCoordinates.setY(-std::numeric_limits<float>::max());
    _maxCoordinates.setZ(-std::numeric_limits<float>::max());
}

CT_AbstractSingularItemDrawable::CT_AbstractSingularItemDrawable(const QString &modelName,
                                                                 const CT_AbstractResult *result) : CT_AbstractItemDrawable(modelName, result)
{
    _minCoordinates.setX(std::numeric_limits<float>::max());
    _minCoordinates.setY(std::numeric_limits<float>::max());
    _minCoordinates.setZ(std::numeric_limits<float>::max());

    _maxCoordinates.setX(-std::numeric_limits<float>::max());
    _maxCoordinates.setY(-std::numeric_limits<float>::max());
    _maxCoordinates.setZ(-std::numeric_limits<float>::max());
}

QString CT_AbstractSingularItemDrawable::staticGetType()
{
    return CT_AbstractItemDrawable::staticGetType() + "/CT_AbstractSingularItemDrawable";
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

QList<CT_AbstractItemAttribute *> CT_AbstractSingularItemDrawable::defaultItemAttributes() const
{
    return PS_DIAM->itemAttributes(getType());
}

void CT_AbstractSingularItemDrawable::setBoundingBox(float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
    _minCoordinates.setX(minx);
    _minCoordinates.setY(miny);
    _minCoordinates.setZ(minz);

    _maxCoordinates.setX(maxx);
    _maxCoordinates.setY(maxy);
    _maxCoordinates.setZ(maxz);

    updateCenterFromBoundingBox();
}

void CT_AbstractSingularItemDrawable::updateCenterFromBoundingBox()
{
    setCenterX((_maxCoordinates.x() + _minCoordinates.x())/2);
    setCenterY((_maxCoordinates.y() + _minCoordinates.y())/2);
    setCenterZ((_maxCoordinates.z() + _minCoordinates.z())/2);
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

void CT_AbstractSingularItemDrawable::getBoundingBox(QVector3D &min, QVector3D &max) const
{
    min.setX(_minCoordinates.x());
    min.setY(_minCoordinates.y());
    min.setZ(_minCoordinates.z());

    max.setX(_maxCoordinates.x());
    max.setY(_maxCoordinates.y());
    max.setZ(_maxCoordinates.z());
}


void CT_AbstractSingularItemDrawable::getScaledBoundingBox(double xCoeff, double yCoeff, double zCoeff, QVector3D &min, QVector3D &max)
{
    max.setX(getCenterX() + (fabs(_maxCoordinates.x()-getCenterX()) * xCoeff));
    max.setY(getCenterY() + (fabs(_maxCoordinates.y()-getCenterY()) * yCoeff));
    max.setZ(getCenterZ() + (fabs(_maxCoordinates.z()-getCenterZ()) * zCoeff));

    min.setX(getCenterX() - (fabs(_minCoordinates.x()-getCenterX()) * xCoeff));
    min.setY(getCenterY() - (fabs(_minCoordinates.y()-getCenterY()) * yCoeff));
    min.setZ(getCenterZ() - (fabs(_minCoordinates.z()-getCenterZ()) * zCoeff));

}

void CT_AbstractSingularItemDrawable::getBufferedBoundingBox(double xMeters, double yMeters, double zMeters, QVector3D &min, QVector3D &max)
{
    min.setX(_minCoordinates.x() - xMeters);
    min.setY(_minCoordinates.y() - yMeters);
    min.setZ(_minCoordinates.z() - zMeters);

    max.setX(_maxCoordinates.x() + xMeters);
    max.setY(_maxCoordinates.y() + yMeters);
    max.setZ(_maxCoordinates.z() + zMeters);
}
