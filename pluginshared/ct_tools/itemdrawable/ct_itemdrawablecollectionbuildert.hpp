#ifndef CT_ITEMDRAWABLECOLLECTIONBUILDERT_HPP
#define CT_ITEMDRAWABLECOLLECTIONBUILDERT_HPP

#include "ct_tools/itemdrawable/ct_itemdrawablecollectionbuildert.h"

#include "ct_result/tools/iterator/ct_resultgroupiterator.h"
#include "ct_result/tools/iterator/ct_resultitemiterator.h"
#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"
#include "ct_itemdrawable/model/outModel/abstract/ct_outabstractgroupmodel.h"
#include "ct_result/ct_resultgroup.h"

template<typename Type, typename Type2, typename Type3, typename Type4>
CT_ItemDrawableCollectionBuilderT<Type, Type2, Type3, Type4>::CT_ItemDrawableCollectionBuilderT()
{
}

template<typename Type, typename Type2, typename Type3, typename Type4>
bool CT_ItemDrawableCollectionBuilderT<Type, Type2, Type3, Type4>::buildFrom(const CT_VirtualAbstractStep *step, bool forceSearchModels)
{
    m_collection.clear();
    m_onlyFindModels = forceSearchModels;

    if(step == NULL)
        return false;

    recursiveBuildFromStep(step, forceSearchModels);

    return true;
}

template<typename Type, typename Type2, typename Type3, typename Type4>
template<typename Cast1, typename Cast2, typename Cast3, typename Cast4>
void CT_ItemDrawableCollectionBuilderT<Type, Type2, Type3, Type4>::onlyKeepsTheseTypesInCollection4()
{
    QMutableListIterator<CT_ItemDrawableCollectionHierarchyStep> it(m_collection);

    while(it.hasNext())
    {
        CT_ItemDrawableCollectionHierarchyStep &pas = it.next();
        QMutableListIterator<CT_ItemDrawableCollectionHierarchyResult> itR(pas.results);

        while(itR.hasNext())
        {
            CT_ItemDrawableCollectionHierarchyResult &par = itR.next();

            if(m_onlyFindModels)
            {
                QMutableListIterator<CT_OutAbstractItemModel*> itA(par.modelsCollection);

                while(itA.hasNext())
                {
                    CT_OutAbstractItemModel *model = itA.next();

                    if((dynamic_cast<Cast1*>(model->itemDrawable()) == NULL)
                            && (dynamic_cast<Cast2*>(model->itemDrawable()) == NULL)
                            && (dynamic_cast<Cast3*>(model->itemDrawable()) == NULL)
                            && (dynamic_cast<Cast4*>(model->itemDrawable()) == NULL))
                    {
                        itA.remove();
                    }
                }

                QList<CT_OutAbstractItemModel*> lTmp;
                itA = lTmp;

                if(par.modelsCollection.isEmpty())
                    itR.remove();
            }
            else
            {
                QMutableListIterator<CT_AbstractSingularItemDrawable*> itA(par.collection);

                while(itA.hasNext())
                {
                    CT_AbstractSingularItemDrawable *item = itA.next();

                    if((dynamic_cast<Cast1*>(item) == NULL)
                            && (dynamic_cast<Cast2*>(item) == NULL)
                            && (dynamic_cast<Cast3*>(item) == NULL)
                            && (dynamic_cast<Cast4*>(item) == NULL))
                    {
                        itA.remove();
                    }
                }

                QList<CT_AbstractSingularItemDrawable*> lTmp;
                itA = lTmp;

                if(par.collection.isEmpty())
                    itR.remove();
            }
        }

        QList< CT_ItemDrawableCollectionHierarchyResult > lTmp;
        itR = lTmp;

        if(pas.results.isEmpty())
            it.remove();
    }
}

template<typename Type, typename Type2, typename Type3, typename Type4>
const QList<CT_ItemDrawableCollectionHierarchyStep>& CT_ItemDrawableCollectionBuilderT<Type, Type2, Type3, Type4>::collection() const
{
    return m_collection;
}

template<typename Type, typename Type2, typename Type3, typename Type4>
void CT_ItemDrawableCollectionBuilderT<Type, Type2, Type3, Type4>::recursiveBuildFromStep(const CT_VirtualAbstractStep *step, bool findOnlyModels)
{
    CT_ItemDrawableCollectionHierarchyStep hi;
    hi.step = (CT_VirtualAbstractStep*)step;

    if(findOnlyModels) {
        QList< QList<CT_OutAbstractResultModelGroup*> > results = step->getAllOutResultModels();
        QListIterator< QList<CT_OutAbstractResultModelGroup*> > it(results);

        while(it.hasNext())
        {
            QList<CT_OutAbstractResultModelGroup*> subList = it.next();

            foreach (CT_OutAbstractResultModelGroup *r, subList) {
                CT_ItemDrawableCollectionHierarchyResult hir;
                hir.modelResult = (CT_OutAbstractResultModel*)r;

                recursiveBuildFromModels(QList<CT_OutAbstractModel *>() << (CT_OutAbstractModel*)r, hir, findOnlyModels);

                if(!hir.modelsCollection.isEmpty())
                    hi.results.append(hir);
            }
        }
    } else {
        QList<CT_ResultGroup*> results = step->getResults();
        QListIterator<CT_ResultGroup*> it(results);

        while(it.hasNext())
        {
            CT_ItemDrawableCollectionHierarchyResult hir;
            hir.result = it.next();

            recursiveBuildFromModels(QList<CT_OutAbstractModel *>() << ((CT_OutAbstractModel*)hir.result->model()->rootModel()), hir, findOnlyModels);

            if(!hir.collection.isEmpty())
                hi.results.append(hir);
        }
    }

    if(!hi.results.empty())
        m_collection.append(hi);

    QList<CT_VirtualAbstractStep*> steps = step->getStepChildList();

    while(!steps.isEmpty())
        recursiveBuildFromStep(dynamic_cast<CT_VirtualAbstractStep*>(steps.takeFirst()), findOnlyModels);
}

template<typename Type, typename Type2, typename Type3, typename Type4>
void CT_ItemDrawableCollectionBuilderT<Type, Type2, Type3, Type4>::recursiveBuildFromModels(QList<CT_OutAbstractModel *> models,
                                                                                            CT_ItemDrawableCollectionHierarchyResult &hir,
                                                                                            bool findOnlyModels)
{
    QListIterator<CT_OutAbstractModel*> itM(models);

    while(itM.hasNext())
    {
        CT_OutAbstractModel *model = itM.next();

        if((model->result() != NULL) || findOnlyModels)
        {
            CT_AbstractItemDrawable *item = NULL;

            const CT_OutAbstractSingularItemModel *itemModel = dynamic_cast<const CT_OutAbstractSingularItemModel*>(model);
            const CT_OutAbstractGroupModel *grModel = dynamic_cast<const CT_OutAbstractGroupModel*>(model);

            if(itemModel != NULL)
                item = itemModel->itemDrawable();
            else if(grModel != NULL)
                item = grModel->group();

            if(item != NULL)
            {
                if((dynamic_cast<Type*>(item) != NULL)
                        || (dynamic_cast<Type2*>(item) != NULL)
                        || (dynamic_cast<Type3*>(item) != NULL)
                        || (dynamic_cast<Type4*>(item) != NULL))
                {
                    if(grModel != NULL)
                    {
                        if(findOnlyModels) {
                            hir.modelsCollection.append((CT_OutAbstractGroupModel*)grModel);
                        } else {
                            CT_ResultGroupIterator it((CT_ResultGroup*)model->result(), grModel);

                            while(it.hasNext())
                                hir.collection.append((CT_AbstractSingularItemDrawable*)it.next());
                        }
                    }
                    else
                    {
                        if(findOnlyModels) {
                            hir.modelsCollection.append((CT_OutAbstractSingularItemModel*)itemModel);
                        } else {
                            CT_ResultItemIterator it((CT_ResultGroup*)model->result(), itemModel);

                            while(it.hasNext())
                                hir.collection.append((CT_AbstractSingularItemDrawable*)it.next());
                        }
                    }
                }
            }

            // TODO if it's an attribute !!

            recursiveBuildFromModels(model->childrensStaticCast<CT_OutAbstractModel>(), hir, findOnlyModels);
        }
    }
}

#endif // CT_ITEMDRAWABLECOLLECTIONBUILDERT_HPP
