#include "ct_stdlaspointsattributescontainershortcut.h"

#include <QDebug>
CT_StdLASPointsAttributesContainerShortcut::CT_StdLASPointsAttributesContainerShortcut() : CT_StdLASPointsAttributesContainer()
{
    _container = NULL;
}

CT_StdLASPointsAttributesContainerShortcut::CT_StdLASPointsAttributesContainerShortcut(const CT_OutAbstractSingularItemModel *model,
                                                                                       const CT_AbstractResult *result,
                                                                                       CT_StdLASPointsAttributesContainer* container) : CT_StdLASPointsAttributesContainer(model,
                                                                                                                                                                                result)
{
    _container = container;
}

CT_StdLASPointsAttributesContainerShortcut::CT_StdLASPointsAttributesContainerShortcut(const QString &modelName,
                                                                                       const CT_AbstractResult *result,
                                                                                       CT_StdLASPointsAttributesContainer* container) : CT_StdLASPointsAttributesContainer(modelName,
                                                                                                                                                                                result)
{
    _container = container;
}

void CT_StdLASPointsAttributesContainerShortcut::insertPointsAttributesAt(CT_LasDefine::LASPointAttributesType key, const CT_AbstractPointAttributesScalar *att)
{
    Q_UNUSED(key);
    Q_UNUSED(att);
    // do nothing
    qFatal("CT_StdLASPointsAttributesContainerShortcut class must not been used for inserting new attributes");
}

CT_AbstractPointsAttributes *CT_StdLASPointsAttributesContainerShortcut::pointsAttributesAt(const int &key) const
{
    if (_container != NULL)
    {
        return _container->pointsAttributesAt(key);
    }

    qFatal("CT_StdLASPointsAttributesContainerShortcut class not correctly initialized");
    return NULL;

}

QList<CT_LasDefine::LASPointAttributesType> CT_StdLASPointsAttributesContainerShortcut::lasPointAttributesTypes() const
{
    if (_container != NULL)
    {
        return _container->lasPointAttributesTypes();
    }

    qFatal("CT_StdLASPointsAttributesContainerShortcut class not correctly initialized");
    return QList<CT_LasDefine::LASPointAttributesType>();
}

void CT_StdLASPointsAttributesContainerShortcut::getLASDataAt(const size_t i, CT_LASData &data) const
{
    if (_container != NULL)
    {
        _container->getLASDataAt(i, data);
    } else {
        qFatal("CT_StdLASPointsAttributesContainerShortcut class not correctly initialized");
    }
}

QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> CT_StdLASPointsAttributesContainerShortcut::lasPointsAttributes() const
{
    if (_container != NULL)
    {
        return _container->lasPointsAttributes();
    }

    qFatal("CT_StdLASPointsAttributesContainerShortcut class not correctly initialized");
    return QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *>();
}

CT_AbstractItemDrawable *CT_StdLASPointsAttributesContainerShortcut::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    return new CT_StdLASPointsAttributesContainerShortcut((CT_OutAbstractSingularItemModel*)model, result, _container);
}
