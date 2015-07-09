#include "ct_sphere.h"

const CT_StandardSphereDrawManager CT_Sphere::SPHERE_DRAW_MANAGER;

CT_DEFAULT_IA_INIT(CT_Sphere)

CT_Sphere::CT_Sphere() : CT_AbstractShape()
{
    setBaseDrawManager(&SPHERE_DRAW_MANAGER);
}

CT_Sphere::CT_Sphere(const CT_OutAbstractSingularItemModel *model,
                     const CT_AbstractResult *result,
                     CT_SphereData *data) : CT_AbstractShape(model, result, data)
{
    setBaseDrawManager(&SPHERE_DRAW_MANAGER);
}

CT_Sphere::CT_Sphere(const QString &modelName,
                     const CT_AbstractResult *result,
                     CT_SphereData *data) : CT_AbstractShape(modelName, result, data)
{
    setBaseDrawManager(&SPHERE_DRAW_MANAGER);
}

QString CT_Sphere::getType() const
{
    return CT_Sphere::staticGetType();
}

QString CT_Sphere::staticGetType()
{
    return CT_AbstractShape::staticGetType() + "/CT_Sphere";
}

double CT_Sphere::getRadius() const
{
    return ((const CT_SphereData&)getData()).getRadius();
}

CT_AbstractItemDrawable* CT_Sphere::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Sphere *sph = new CT_Sphere((const CT_OutAbstractSingularItemModel *)model, result, (getPointerData() != NULL) ? ((const CT_SphereData&)getData()).clone() : NULL);
    sph->setId(id());
    sph->setAlternativeDrawManager(getAlternativeDrawManager());

    return sph;
}

CT_AbstractItemDrawable* CT_Sphere::copy(const QString &modelName, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Sphere *sph = new CT_Sphere(modelName, result, (getPointerData() != NULL) ? ((const CT_SphereData&)getData()).clone() : NULL);
    sph->setId(id());
    sph->setAlternativeDrawManager(getAlternativeDrawManager());

    return sph;
}
