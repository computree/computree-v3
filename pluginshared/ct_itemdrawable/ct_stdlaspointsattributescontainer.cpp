#include "ct_stdlaspointsattributescontainer.h"

CT_StdLASPointsAttributesContainer::CT_StdLASPointsAttributesContainer() : CT_AbstractPointsAttributesContainer()
{
}

CT_StdLASPointsAttributesContainer::CT_StdLASPointsAttributesContainer(const CT_OutAbstractSingularItemModel *model,
                                                                       const CT_AbstractResult *result) : CT_AbstractPointsAttributesContainer(model,
                                                                                                                                               result)
{

}

CT_StdLASPointsAttributesContainer::CT_StdLASPointsAttributesContainer(const QString &modelName,
                                                                       const CT_AbstractResult *result) : CT_AbstractPointsAttributesContainer(modelName,
                                                                                                                                               result)
{

}

QString CT_StdLASPointsAttributesContainer::getType() const
{
    return staticGetType();
}

QString CT_StdLASPointsAttributesContainer::staticGetType()
{
    return CT_AbstractPointsAttributesContainer::staticGetType() + "/CT_StdLASPointsAttributesContainer";
}

void CT_StdLASPointsAttributesContainer::insertPointsAttributesAt(CT_LasDefine::LASPointAttributesType key, const CT_AbstractPointAttributesScalar *att)
{
    if(att != NULL)
        internalInsertAttributes(key, att);
}

QList<CT_LasDefine::LASPointAttributesType> CT_StdLASPointsAttributesContainer::lasPointAttributesTypes() const
{
    QList<CT_LasDefine::LASPointAttributesType> l;
    QList<int> la = internalAttributes().uniqueKeys();

    foreach (int i, la) {
        l.append((CT_LasDefine::LASPointAttributesType)i);
    }

    return l;
}

QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> CT_StdLASPointsAttributesContainer::lasPointsAttributes() const
{
    QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> h;
    QHash<int, CT_AbstractPointsAttributes *> ha = internalAttributes();

    QHashIterator<int, CT_AbstractPointsAttributes *> it(ha);

    while(it.hasNext()) {
        it.next();
        h.insert((CT_LasDefine::LASPointAttributesType)it.key(), (CT_AbstractPointAttributesScalar*)it.value());
    }

    return h;
}

CT_AbstractItemDrawable *CT_StdLASPointsAttributesContainer::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    return new CT_StdLASPointsAttributesContainer((CT_OutAbstractSingularItemModel*)model, result);
}
