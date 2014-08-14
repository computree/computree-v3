#include <QApplication>

#include "stdio.h"

#include "ct_instdgroupmodel.h"
#include "ct_inoneormoregroupmodel.h"
#include "ct_instdsingularitemmodel.h"

#include "ct_inresultmodelgroup.h"

#include "ct_outstdgroupmodel.h"
#include "ct_outstdsingularitemmodel.h"

#include "ct_outresultmodelgroup.h"

#include "ct_scene.h"
#include "ct_pointcluster.h"
#include "ct_circle.h"
#include "ct_standarditemgroup.h"
#include "ct_mesh/ct_mesh.h"
#include "ct_mesh/ct_face.h"
#include "ct_mesh/ct_edge.h"

#include "ct_mesh/tools/ct_meshallocatort.h"

/*
void showPossibilitiesSaved(const QList<CT_InAbstractModelPossibility*> &list)
{
    QListIterator<CT_InAbstractModelPossibility*> pIt(list);

    if(pIt.hasNext())
    {
        printf(" (saved - ");

        while(pIt.hasNext())
        {
            printf(QString(" 0x%1").arg((int)pIt.next()->outModel()).toLatin1());

            if(pIt.hasNext())
                printf(",");
            else
                printf(")");
        }
    }
}

void recursiveShowModel(DEF_CT_AbstractGroupModelIn *inModel, QString tab)
{
    QString tmp = tab;

    if(tmp.isEmpty())
        printf("----------------- IN MODEL -------------\r\n");

    printf(tab.toLatin1());

    if(inModel->uniqueName().isEmpty())
        printf("[+]");
    else
        printf(inModel->uniqueName().toLatin1());

    QList<CT_InAbstractModelPossibility*> list = inModel->getPossibilitiesSaved();
    showPossibilitiesSaved(list);

    printf(QString(" - %1 cycle(s)").arg(inModel->nSaveCycle()).toLatin1());

    tab += "\t";

    QListIterator< DEF_CT_AbstractGroupModelIn* > itG(inModel->groups());

    while(itG.hasNext())
    {
        printf("\r\n");
        recursiveShowModel(itG.next(), tab);
    }

    QListIterator<CT_InAbstractSingularItemModel*> itI(inModel->items());

    if(itI.hasNext())
    {
        printf("\r\n");

        while(itI.hasNext())
        {
            CT_InAbstractSingularItemModel *item = itI.next();

            printf(tab.toLatin1());
            printf(QString(item->uniqueName() +" (" + item->itemType() + ")").toLatin1());

            QList<CT_InAbstractModelPossibility*> list = item->getPossibilitiesSaved();
            showPossibilitiesSaved(list);

            printf(QString(" - %1 cycle(s)").arg(item->nSaveCycle()).toLatin1());

            printf("\r\n");

        }
    }

    if(tmp.isEmpty())
        printf("\r\n----------------------------------------\r\n");
}

void recursiveShowOutModel(DEF_CT_AbstractGroupModelOut *outModel, QString tab)
{
    QString tmp = tab;

    if(tmp.isEmpty())
        printf("----------------- OUT MODEL ------------\r\n");

    printf(tab.toLatin1());
    printf(QString(outModel->uniqueName() + "(0x%1)").arg((int)outModel).toLatin1());

    tab += "\t";

    QListIterator< DEF_CT_AbstractGroupModelOut* > itG(outModel->groups());

    while(itG.hasNext())
    {
        printf("\r\n");
        recursiveShowOutModel(itG.next(), tab);
    }

    QListIterator<CT_OutAbstractSingularItemModel*> itI(outModel->items());

    if(itI.hasNext())
    {
        printf("\r\n");

        while(itI.hasNext())
        {
            CT_OutAbstractSingularItemModel *item = itI.next();

            printf(tab.toLatin1());
            printf(QString(item->uniqueName() +" (" + item->item()->getType() + " / 0x%1)").arg((int)item).toLatin1());
            printf("\r\n");
        }
    }

    if(tmp.isEmpty())
        printf("\r\n----------------------------------------\r\n");
}

// test si un Item ayant le même nom est rejeté lorsqu'il est ajouté dans un group
void testAddItemWithSameNameModel(bool showInModel)
{
    CT_InStdSingularItemModel *item = new CT_InStdSingularItemModel("CT_AbstractItemDrawable/CT_Scene",
                                                                              CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                              "item");

    CT_InStandardGroupModel *group = new CT_InStandardGroupModel("group");
    group->addItem(item);

    CT_InStdSingularItemModel *rejectItem = new CT_InStdSingularItemModel("CT_AbstractItemDrawable/CT_PointCluster",
                                                                                    CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                                    "item");

    if(!group->addItem(rejectItem))
        printf("testAddItemWithSameNameModel :\t\t\t\tOK NOT ADDED\r\n");
    else
        printf("testAddItemWithSameNameModel :\t\t\t\tERROR ADDED\r\n");

    if(showInModel)
        recursiveShowModel(group, "");

    delete group;
}

// test si un Item ayant le même type est accepté lorsqu'il est ajouté dans un group
void testAddItemWithSameType(bool showInModel)
{
    CT_InStdSingularItemModel *item = new CT_InStdSingularItemModel("CT_AbstractItemDrawable/CT_Scene",
                                                                              CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                              "item");

    CT_InStandardGroupModel *group = new CT_InStandardGroupModel("group");
    group->addItem(item);

    CT_InStdSingularItemModel *rejectItem = new CT_InStdSingularItemModel("CT_AbstractItemDrawable/CT_Scene",
                                                                                    CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                                    "item2");

    if(group->addItem(rejectItem))
        printf("testAddItemWithSameType :\t\t\t\t\tOK ADDED\r\n");
    else
        printf("testAddItemWithSameType :\t\t\t\t\tERROR NOT ADDED\r\n");

    if(showInModel)
        recursiveShowModel(group, "");

    delete group;
}

// test si un Item ayant un nom et un type différent est accepté lorsqu'il est ajouté dans un group
void testAddItemWithDifferentNameModelAndType(bool showInModel)
{
    CT_InStdSingularItemModel *item = new CT_InStdSingularItemModel("CT_AbstractItemDrawable/CT_Scene",
                                                                              CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                              "modelName");

    CT_InStandardGroupModel *group = new CT_InStandardGroupModel("group");
    group->addItem(item);

    CT_InStdSingularItemModel *acceptItem = new CT_InStdSingularItemModel("CT_AbstractItemDrawable/CT_PointCluster",
                                                                                    CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                                    "modelName2");

    if(group->addItem(acceptItem))
        printf("testAddItemWithDifferentNameModelAndType :\t\t\tOK ADDED\r\n");
    else
        printf("testAddItemWithDifferentNameModelAndType :\t\t\tERROR NOT ADDED\r\n");

    if(showInModel)
        recursiveShowModel(group, "");

    delete group;
}

// test si un groupe ayant le même nom qu'un autre dans l'arborescence est rejeté lors de son ajout
void testAddGroupWithSameNameModel(bool showInModel)
{
    CT_InStandardGroupModel *parentGroup = new CT_InStandardGroupModel("group");
    CT_InStandardGroupModel *childGroup = new CT_InStandardGroupModel("groupT");
    CT_InStandardGroupModel *childGroup2 = new CT_InStandardGroupModel("group");

    if(!parentGroup->addGroup(childGroup))
    {
        printf("testAddGroupWithSameNameModel :\t\t\t\tPRE ERROR\r\n");
        delete childGroup2;
        delete parentGroup;

        return;
    }

    if(!childGroup->addGroup(childGroup2))
        printf("testAddGroupWithSameNameModel :\t\t\t\tOK NOT ADDED\r\n");
    else
        printf("testAddGroupWithSameNameModel :\t\t\t\tERROR ADDED\r\n");

    if(showInModel)
        recursiveShowModel(parentGroup, "");

    delete parentGroup;
}

// test si un groupe ayant un groupe qui a le même nom qu'un autre dans l'arborescence est rejeté lors de son ajout
void testAddGroupWithGroupWithSameNameModel(bool showInModel)
{
    CT_InStandardGroupModel *parentGroup = new CT_InStandardGroupModel("group");
    CT_InStandardGroupModel *childGroup = new CT_InStandardGroupModel("groupT");
    CT_InStandardGroupModel *childGroup2 = new CT_InStandardGroupModel("group");

    if(!childGroup->addGroup(childGroup2))
    {
        printf("testAddGroupWithGroupWithSameNameModel :\t\t\tPRE ERROR\r\n");
        delete childGroup2;
        delete parentGroup;

        return;
    }

    if(!parentGroup->addGroup(childGroup))
        printf("testAddGroupWithGroupWithSameNameModel :\t\t\tOK NOT ADDED\r\n");
    else
        printf("testAddGroupWithGroupWithSameNameModel :\t\t\tERROR ADDED\r\n");

    if(showInModel)
        recursiveShowModel(parentGroup, "");

    delete parentGroup;
}

// test si un groupe ayant un item du même nom que celui contenu dans un groupe de l'arborescence est rejeté lors de l'ajout du groupe dans l'arborescence
void testAddGroupWithSameItemNameModel(bool showInModel)
{
    CT_InStandardGroupModel *parentGroup = new CT_InStandardGroupModel("group");
    CT_InStandardGroupModel *childGroup = new CT_InStandardGroupModel("groupT");
    CT_InStandardGroupModel *childGroup2 = new CT_InStandardGroupModel("groupT2");

    CT_InStdSingularItemModel *item = new CT_InStdSingularItemModel("CT_AbstractItemDrawable/CT_Scene",
                                                                              CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                              "modelName");

    CT_InStdSingularItemModel *item2 = new CT_InStdSingularItemModel("CT_AbstractItemDrawable/CT_Scene",
                                                                                CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                                "modelName");

    if(!childGroup2->addItem(item2))
        printf("testAddGroupWithSameItemNameModel :\t\t\t\tPRE ERROR\r\n");

    if(!childGroup->addGroup(childGroup2))
        printf("testAddGroupWithSameItemNameModel :\t\t\t\tPRE ERROR 2\r\n");

    if(!parentGroup->addItem(item))
        printf("testAddGroupWithSameItemNameModel :\t\t\t\tPRE ERROR 3\r\n");

    if(!parentGroup->addGroup(childGroup))
        printf("testAddGroupWithSameItemNameModel :\t\t\t\tOK NOT ADDED\r\n");
    else
        printf("testAddGroupWithSameItemNameModel :\t\t\t\tERROR ADDED\r\n");

    if(showInModel)
        recursiveShowModel(parentGroup, "");

    delete parentGroup;
}

// test si un groupe ayant un item du même nom que celui contenu dans un groupe de l'arborescence est rejeté lors de l'ajout du groupe dans l'arborescence
void testAddGroupWithDifferentItemNameModel(bool showInModel)
{
    CT_InStandardGroupModel *parentGroup = new CT_InStandardGroupModel("group");
    CT_InStandardGroupModel *childGroup = new CT_InStandardGroupModel("groupT");
    CT_InStandardGroupModel *childGroup2 = new CT_InStandardGroupModel("groupT2");

    CT_InStdSingularItemModel *item = new CT_InStdSingularItemModel("CT_AbstractItemDrawable/CT_Scene",
                                                                              CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                              "modelName");

    CT_InStdSingularItemModel *item2 = new CT_InStdSingularItemModel("CT_AbstractItemDrawable/CT_PointCluster",
                                                                                CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                                "modelName2");

    if(!childGroup2->addItem(item2))
        printf("testAddGroupWithDifferentItemNameModel :\t\t\tPRE ERROR\r\n");

    if(!childGroup->addGroup(childGroup2))
        printf("testAddGroupWithDifferentItemNameModel :\t\t\tPRE ERROR 2\r\n");

    if(!parentGroup->addItem(item))
        printf("testAddGroupWithDifferentItemNameModel :\t\t\tPRE ERROR 3\r\n");

    if(parentGroup->addGroup(childGroup))
        printf("testAddGroupWithDifferentItemNameModel :\t\t\tOK ADDED\r\n");
    else
        printf("testAddGroupWithDifferentItemNameModel :\t\t\tERROR NOT ADDED\r\n");

    if(showInModel)
        recursiveShowModel(parentGroup, "");

    delete parentGroup;
}

// test si le modèle d'entrée composé uniquement de groupe trouve ce qu'il cherche dans le modèle de sortie identique
void testSiSearchIdenticalInputModelInOutputModel(bool showModel)
{
    CT_InStandardGroupModel *ig1 = new CT_InStandardGroupModel("ig1");
    CT_InStandardGroupModel *ig2 = new CT_InStandardGroupModel("ig2");
    CT_InStandardGroupModel *ig3 = new CT_InStandardGroupModel("ig3");

    CT_OutStandardGroupModel *og1 = new CT_OutStandardGroupModel("og1");
    CT_OutStandardGroupModel *og2 = new CT_OutStandardGroupModel("og2");
    CT_OutStandardGroupModel *og3 = new CT_OutStandardGroupModel("og3");

    // construit le modèle d'entrée
    if(!ig1->addGroup(ig2))
        printf("testSiSearchIdenticalInputModelInOutputModel :\t\t\tPRE ERROR 1\r\n");

    if(!ig2->addGroup(ig3))
        printf("testSiSearchIdenticalInputModelInOutputModel :\t\t\tPRE ERROR 2\r\n");

    // construit le modèle de sortie
    if(!og1->addGroup(og2))
        printf("testSiSearchIdenticalInputModelInOutputModel :\t\t\tPRE ERROR 3\r\n");

    if(!og2->addGroup(og3))
        printf("testSiSearchIdenticalInputModelInOutputModel :\t\t\tPRE ERROR 4\r\n");

    // test si le modèle de sortie est accepté par le modèle d'entrée
    if(ig1->recursiveFindOnePossibilityInModel(*og1, true))
        printf("testSiSearchIdenticalInputModelInOutputModel :\t\t\tOK FOUNDED\r\n");
    else
        printf("testSiSearchIdenticalInputModelInOutputModel :\t\t\tERROR NOT FOUNDED\r\n");

    if(showModel)
    {
        recursiveShowModel(ig1, "");
        recursiveShowOutModel(og1, "");
    }

    delete ig1;
    delete og1;
}

// test si le modèle d'entrée composé uniquement de groupe (3 groupes) ne trouve pas ce qu'il cherche dans le modèle de sortie composé uniquement de groupe (2 groupes) mais ayant un groupe manquant
void testSiSearchDifferentInputModelInOutputModel(bool showModel)
{
    CT_InStandardGroupModel *ig1 = new CT_InStandardGroupModel("ig1");
    CT_InStandardGroupModel *ig2 = new CT_InStandardGroupModel("ig2");
    CT_InStandardGroupModel *ig3 = new CT_InStandardGroupModel("ig3");

    CT_OutStandardGroupModel *og1 = new CT_OutStandardGroupModel("og1");
    CT_OutStandardGroupModel *og2 = new CT_OutStandardGroupModel("og2");

    // construit le modèle d'entrée
    if(!ig1->addGroup(ig2))
        printf("testSiSearchDifferentInputModelInOutputModel :\t\t\tPRE ERROR 1\r\n");

    if(!ig2->addGroup(ig3))
        printf("testSiSearchDifferentInputModelInOutputModel :\t\t\tPRE ERROR 2\r\n");

    // construit le modèle de sortie
    if(!og1->addGroup(og2))
        printf("testSiSearchDifferentInputModelInOutputModel :\t\t\tPRE ERROR 3\r\n");

    // test si le modèle de sortie est accepté par le modèle d'entrée
    if(!ig1->recursiveFindOnePossibilityInModel(*og1, true))
        printf("testSiSearchDifferentInputModelInOutputModel :\t\t\tOK NOT FOUNDED\r\n");
    else
        printf("testSiSearchDifferentInputModelInOutputModel :\t\t\tERROR FOUNDED\r\n");

    if(showModel)
    {
        recursiveShowModel(ig1, "");
        recursiveShowOutModel(og1, "");
    }

    delete ig1;
    delete og1;
}

// test si le modèle d'entrée composé de groupes et d'un item trouve ce qu'il cherche dans le modèle de sortie identique
void testSiSearchInputModelInOutputModelWithIdenticalItems(bool showModel)
{
    CT_InStandardGroupModel *ig1 = new CT_InStandardGroupModel("ig1");
    CT_InStandardGroupModel *ig2 = new CT_InStandardGroupModel("ig2");
    CT_InStandardGroupModel *ig3 = new CT_InStandardGroupModel("ig3");

    CT_InStdSingularItemModel *it1 = new CT_InStdSingularItemModel(CT_AbstractItemDrawableWithPointCloud::staticGetType(),
                                                                             CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                             "sceneIn");

    CT_OutStandardGroupModel *og1 = new CT_OutStandardGroupModel("og1");
    CT_OutStandardGroupModel *og2 = new CT_OutStandardGroupModel("og2");
    CT_OutStandardGroupModel *og3 = new CT_OutStandardGroupModel("og3");

    CT_OutStandardItemDrawableModel *ot1 = new CT_OutStandardItemDrawableModel(new CT_Scene(),
                                                                               "sceneOut");

    // construit le modèle d'entrée
    if(!ig1->addGroup(ig2))
        printf("testSiSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 1\r\n");

    if(!ig1->addItem(it1))
        printf("testSiSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 2\r\n");

    if(!ig2->addGroup(ig3))
        printf("testSiSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 3\r\n");

    // construit le modèle de sortie
    if(!og1->addGroup(og2))
        printf("testSiSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 4\r\n");

    if(!og1->addItem(ot1))
        printf("testSiSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 5\r\n");

    if(!og2->addGroup(og3))
        printf("testSiSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 6\r\n");

    // test si le modèle de sortie est accepté par le modèle d'entrée
    if(ig1->recursiveFindOnePossibilityInModel(*og1, true))
        printf("testSiSearchInputModelInOutputModelWithIdenticalItems :\t\tOK FOUNDED\r\n");
    else
        printf("testSiSearchInputModelInOutputModelWithIdenticalItems :\t\tERROR NOT FOUNDED\r\n");

    if(showModel)
    {
        recursiveShowModel(ig1, "");
        recursiveShowOutModel(og1, "");
    }

    delete ig1;
    delete og1;
}

// test si le modèle d'entrée composé de groupe et d'un item ne trouve pas ce qu'il cherche dans le modèle de sortie composé d'un item de type différent
void testSiSearchInputModelInOutputModelWithDifferentItemsType(bool showModel)
{
    CT_InStandardGroupModel *ig1 = new CT_InStandardGroupModel("ig1");
    CT_InStandardGroupModel *ig2 = new CT_InStandardGroupModel("ig2");
    CT_InStandardGroupModel *ig3 = new CT_InStandardGroupModel("ig3");

    CT_InStdSingularItemModel *it1 = new CT_InStdSingularItemModel(CT_Scene::staticGetType(),
                                                                             CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                             "sceneIn");

    CT_OutStandardGroupModel *og1 = new CT_OutStandardGroupModel("og1");
    CT_OutStandardGroupModel *og2 = new CT_OutStandardGroupModel("og2");
    CT_OutStandardGroupModel *og3 = new CT_OutStandardGroupModel("og3");

    CT_OutStandardItemDrawableModel *ot1 = new CT_OutStandardItemDrawableModel(new CT_PointCluster(),
                                                                               "pointClusterOut");

    // construit le modèle d'entrée
    if(!ig1->addGroup(ig2))
        printf("testSiSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 1\r\n");

    if(!ig1->addItem(it1))
        printf("testSiSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 2\r\n");

    if(!ig2->addGroup(ig3))
        printf("testSiSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 3\r\n");

    // construit le modèle de sortie
    if(!og1->addGroup(og2))
        printf("testSiSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 4\r\n");

    if(!og1->addItem(ot1))
        printf("testSiSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 5\r\n");

    if(!og2->addGroup(og3))
        printf("testSiSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 6\r\n");

    // test si le modèle de sortie est accepté par le modèle d'entrée
    if(!ig1->recursiveFindOnePossibilityInModel(*og1, true))
        printf("testSiSearchInputModelInOutputModelWithDifferentItemsType :\t\tOK NOT FOUNDED\r\n");
    else
        printf("testSiSearchInputModelInOutputModelWithDifferentItemsType :\t\tERROR FOUNDED\r\n");

    if(showModel)
    {
        recursiveShowModel(ig1, "");
        recursiveShowOutModel(og1, "");
    }

    delete ig1;
    delete og1;
}

// test si le modèle d'entrée composé uniquement de groupe trouve ce qu'il cherche dans le modèle de sortie identique
void testCoSearchIdenticalInputModelInOutputModel(bool showModel)
{
    CT_InStandardGroupModel *ig1 = new CT_InStandardGroupModel("ig1");
    CT_InOneOrMoreGroupModel *ig2 = new CT_InOneOrMoreGroupModel();
    CT_InStandardGroupModel *ig3 = new CT_InStandardGroupModel("ig3");

    CT_OutStandardGroupModel *og1 = new CT_OutStandardGroupModel("og1");
    CT_OutStandardGroupModel *og2 = new CT_OutStandardGroupModel("og2");
    CT_OutStandardGroupModel *og3 = new CT_OutStandardGroupModel("og3");

    // construit le modèle d'entrée
    if(!ig1->addGroup(ig2))
        printf("testCoSearchIdenticalInputModelInOutputModel :\t\t\tPRE ERROR 1\r\n");

    if(!ig2->addGroup(ig3))
        printf("testCoSearchIdenticalInputModelInOutputModel :\t\t\tPRE ERROR 2\r\n");

    // construit le modèle de sortie
    if(!og1->addGroup(og2))
        printf("testCoSearchIdenticalInputModelInOutputModel :\t\t\tPRE ERROR 3\r\n");

    if(!og2->addGroup(og3))
        printf("testCoSearchIdenticalInputModelInOutputModel :\t\t\tPRE ERROR 4\r\n");

    // test si le modèle de sortie est accepté par le modèle d'entrée
    if(ig1->recursiveFindOnePossibilityInModel(*og1, true))
        printf("testCoSearchIdenticalInputModelInOutputModel :\t\t\tOK FOUNDED\r\n");
    else
        printf("testCoSearchIdenticalInputModelInOutputModel :\t\t\tERROR NOT FOUNDED\r\n");

    if(showModel)
    {
        recursiveShowModel(ig1, "");
        recursiveShowOutModel(og1, "");
    }

    delete ig1;
    delete og1;
}

// test si le modèle d'entrée composé uniquement de groupe (3 groupes) ne trouve pas ce qu'il cherche dans le modèle de sortie composé uniquement de groupe (2 groupes) mais ayant un groupe manquant
void testCoSearchDifferentInputModelInOutputModel(bool showModel)
{
    CT_InStandardGroupModel *ig1 = new CT_InStandardGroupModel("ig1");
    CT_InOneOrMoreGroupModel *ig2 = new CT_InOneOrMoreGroupModel();
    CT_InStandardGroupModel *ig3 = new CT_InStandardGroupModel("ig3");

    CT_OutStandardGroupModel *og1 = new CT_OutStandardGroupModel("og1");
    CT_OutStandardGroupModel *og2 = new CT_OutStandardGroupModel("og2");

    // construit le modèle d'entrée
    if(!ig1->addGroup(ig2))
        printf("testCoSearchDifferentInputModelInOutputModel :\t\t\tPRE ERROR 1\r\n");

    if(!ig2->addGroup(ig3))
        printf("testCoSearchDifferentInputModelInOutputModel :\t\t\tPRE ERROR 2\r\n");

    // construit le modèle de sortie
    if(!og1->addGroup(og2))
        printf("testCoSearchDifferentInputModelInOutputModel :\t\t\tPRE ERROR 3\r\n");

    // test si le modèle de sortie est accepté par le modèle d'entrée
    if(!ig1->recursiveFindOnePossibilityInModel(*og1, true))
        printf("testCoSearchDifferentInputModelInOutputModel :\t\t\tOK NOT FOUNDED\r\n");
    else
        printf("testCoSearchDifferentInputModelInOutputModel :\t\t\tERROR FOUNDED\r\n");

    if(showModel)
    {
        recursiveShowModel(ig1, "");
        recursiveShowOutModel(og1, "");
    }

    delete ig1;
    delete og1;
}

// test si le modèle d'entrée composé de groupes et d'un item trouve ce qu'il cherche dans le modèle de sortie
void testCoSearchInputModelInOutputModelWithIdenticalItems(bool showModel)
{
    CT_InOneOrMoreGroupModel *ig1 = new CT_InOneOrMoreGroupModel();
    CT_InStandardGroupModel *ig2 = new CT_InStandardGroupModel("ig2");

    CT_InStdSingularItemModel *it1 = new CT_InStdSingularItemModel(CT_AbstractItemDrawableWithPointCloud::staticGetType(),
                                                                             CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                             "sceneIn");

    CT_OutStandardGroupModel *og1 = new CT_OutStandardGroupModel("og1");
    CT_OutStandardGroupModel *og2 = new CT_OutStandardGroupModel("og2");
    CT_OutStandardGroupModel *og3 = new CT_OutStandardGroupModel("og3");

    CT_OutStandardItemDrawableModel *ot1 = new CT_OutStandardItemDrawableModel(new CT_Scene(),
                                                                               "sceneOut");

    // construit le modèle d'entrée
    if(!ig1->addGroup(ig2))
        printf("testCoSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 1\r\n");

    if(!ig1->addItem(it1))
        printf("testCoSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 2\r\n");

    // construit le modèle de sortie
    if(!og1->addGroup(og2))
        printf("testCoSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 3\r\n");

    if(!og2->addItem(ot1))
        printf("testCoSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 4\r\n");

    if(!og2->addGroup(og3))
        printf("testCoSearchInputModelInOutputModelWithIdenticalItems :\t\tPRE ERROR 5\r\n");

    // test si le modèle de sortie est accepté par le modèle d'entrée
    if(ig1->recursiveFindOnePossibilityInModel(*og1, true))
        printf("testCoSearchInputModelInOutputModelWithIdenticalItems :\t\tOK FOUNDED\r\n");
    else
        printf("testCoSearchInputModelInOutputModelWithIdenticalItems :\t\tERROR NOT FOUNDED\r\n");

    if(showModel)
    {
        recursiveShowModel(ig1, "");
        recursiveShowOutModel(og1, "");
    }

    delete ig1;
    delete og1;
}

// test si le modèle d'entrée composé de groupe et d'un item ne trouve pas ce qu'il cherche dans le modèle de sortie composé d'un item de type différent
void testCoSearchInputModelInOutputModelWithDifferentItemsType(bool showModel)
{
    CT_InOneOrMoreGroupModel *ig1 = new CT_InOneOrMoreGroupModel();
    CT_InStandardGroupModel *ig2 = new CT_InStandardGroupModel("ig2");

    CT_InStdSingularItemModel *it1 = new CT_InStdSingularItemModel(CT_Scene::staticGetType(),
                                                                             CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                             "sceneIn");

    CT_OutStandardGroupModel *og1 = new CT_OutStandardGroupModel("og1");
    CT_OutStandardGroupModel *og2 = new CT_OutStandardGroupModel("og2");
    CT_OutStandardGroupModel *og3 = new CT_OutStandardGroupModel("og3");

    CT_OutStandardItemDrawableModel *ot1 = new CT_OutStandardItemDrawableModel(new CT_PointCluster(),
                                                                               "pointClusterOut");

    // construit le modèle d'entrée
    if(!ig1->addGroup(ig2))
        printf("testCoSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 1\r\n");

    if(!ig1->addItem(it1))
        printf("testCoSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 2\r\n");

    // construit le modèle de sortie
    if(!og1->addGroup(og2))
        printf("testCoSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 3\r\n");

    if(!og2->addItem(ot1))
        printf("testCoSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 4\r\n");

    if(!og2->addGroup(og3))
        printf("testCoSearchInputModelInOutputModelWithDifferentItemsType :\t\tPRE ERROR 5\r\n");

    // test si le modèle de sortie est accepté par le modèle d'entrée
    if(!ig1->recursiveFindOnePossibilityInModel(*og1, true))
        printf("testCoSearchInputModelInOutputModelWithDifferentItemsType :\t\tOK NOT FOUNDED\r\n");
    else
        printf("testCoSearchInputModelInOutputModelWithDifferentItemsType :\t\tERROR FOUNDED\r\n");

    if(showModel)
    {
        recursiveShowModel(ig1, "");
        recursiveShowOutModel(og1, "");
    }

    delete ig1;
    delete og1;
}

// test si le modèle d'entrée trouve plusieurs choix dans le modèle de sortie
void testCoSearchInputModelInOutputModelWithDifferentStructure(bool showModel)
{
    CT_InStandardGroupModel *ig1 = new CT_InStandardGroupModel("ig1");
    CT_InOneOrMoreGroupModel *ig2 = new CT_InOneOrMoreGroupModel();

    CT_InStdSingularItemModel *it1 = new CT_InStdSingularItemModel(CT_PointCluster::staticGetType(),
                                                                             CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                             "pointClusterIn");

    CT_OutStandardGroupModel *og1 = new CT_OutStandardGroupModel("og1");
    CT_OutStandardGroupModel *og2 = new CT_OutStandardGroupModel("og2");
    CT_OutStandardGroupModel *og3 = new CT_OutStandardGroupModel("og3");
    CT_OutStandardGroupModel *og4 = new CT_OutStandardGroupModel("og4");
    CT_OutStandardGroupModel *og5 = new CT_OutStandardGroupModel("og5");

    CT_OutStandardItemDrawableModel *ot1 = new CT_OutStandardItemDrawableModel(new CT_PointCluster(),
                                                                               "pointClusterOut");

    CT_OutStandardItemDrawableModel *ot2 = new CT_OutStandardItemDrawableModel(new CT_Scene(),
                                                                               "sceneOut");

    CT_OutStandardItemDrawableModel *ot3 = new CT_OutStandardItemDrawableModel(new CT_PointCluster(),
                                                                               "pointClusterOut2");

    CT_OutStandardItemDrawableModel *ot4 = new CT_OutStandardItemDrawableModel(new CT_Scene(),
                                                                               "sceneOut2");

    // construit le modèle d'entrée
    if(!ig1->addGroup(ig2))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tPRE ERROR 0\r\n");

    if(!ig2->addItem(it1))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tPRE ERROR 1\r\n");

    // construit le modèle de sortie
    if(!og1->addGroup(og2))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tPRE ERROR 3\r\n");

    if(!og1->addGroup(og3))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tPRE ERROR 4\r\n");

    if(!og3->addItem(ot1))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tPRE ERROR 5\r\n");

    if(!og2->addGroup(og4))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tPRE ERROR 6\r\n");

    if(!og4->addItem(ot2))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tPRE ERROR 7\r\n");

    if(!og4->addGroup(og5))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tPRE ERROR 8\r\n");

    if(!og5->addItem(ot3))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tPRE ERROR 9\r\n");

    if(!og5->addItem(ot4))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tPRE ERROR 10\r\n");

    // test si le modèle de sortie est accepté par le modèle d'entrée
    if(ig1->recursiveFindAllPossibilitiesInModel(*og1, true))
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tOK FOUNDED\r\n");
    else
        printf("testCoSearchInputModelInOutputModelWithDifferentStructure :\t\tERROR NOT FOUNDED\r\n");

    if(showModel)
    {
        recursiveShowModel(ig1, "");
        recursiveShowOutModel(og1, "");
    }

    delete ig1;
    delete og1;
}

void testCoSearchInResultModelInOutputResultModel()
{
    CT_InOneOrMoreGroupModel *inRoot = new CT_InOneOrMoreGroupModel();
    CT_InStdSingularItemModel *inCircle = new CT_InStdSingularItemModel(CT_Circle::staticGetType(),
                                                                               CT_InStdSingularItemModel::C_ChooseMultipleIfMultiple,
                                                                               "circle");

    // construit le modèle d'entrée
    if(!inRoot->addItem(inCircle))
        printf("testCoSearchInResultModelInOutputResultModel :\t\t\tPRE ERROR 0\r\n");

    CT_InResultModelGroup *inResModel = new CT_InResultModelGroup(inRoot, "r", false);

    QList<CT_OutResultModelGroup*> outResultModels;

    // construit plusieurs modèle de sortie
    CT_OutStandardGroupModel *og1 = new CT_OutStandardGroupModel("og1");
    CT_OutStandardItemDrawableModel *ot1 = new CT_OutStandardItemDrawableModel(new CT_Circle(), "outCircle");

    CT_OutStandardGroupModel *og2 = new CT_OutStandardGroupModel("og2");
    CT_OutStandardItemDrawableModel *ot2 = new CT_OutStandardItemDrawableModel(new CT_Circle(), "outCircle2");

    if(!og1->addItem(ot1))
        printf("testCoSearchInResultModelInOutputResultModel :\t\t\tPRE ERROR 1\r\n");

    if(!og2->addItem(ot2))
        printf("testCoSearchInResultModelInOutputResultModel :\t\t\tPRE ERROR 2\r\n");

    CT_OutResultModelGroup *outResModel = new CT_OutResultModelGroup(og1, "r1");
    outResultModels.append(outResModel);
    outResModel = new CT_OutResultModelGroup(og2, "r2");
    outResultModels.append(outResModel);

    QListIterator<CT_OutResultModelGroup*> it(outResultModels);

    while(it.hasNext())
    {
        if(!inResModel->recursiveFindOnePossibilityInModel(*it.next(), true))
        {
            qDeleteAll(outResultModels.begin(), outResultModels.end());
            delete inResModel;

            printf("testCoSearchInResultModelInOutputResultModel :\t\t\tERROR NOT FOUND\r\n");

            return;
        }
    }

    if(inResModel->nPossibilitiesSaved() == outResultModels.size())
        printf("testCoSearchInResultModelInOutputResultModel :\t\t\tOK FOUND\r\n");
    else
        printf("testCoSearchInResultModelInOutputResultModel :\t\t\tERROR NOT FOUND\r\n");

    qDeleteAll(outResultModels.begin(), outResultModels.end());
    delete inResModel;
}

void testCreateStandardItemGroupAndAddItemDrawableNotInModel()
{
    CT_OutStandardGroupModel *layer = new CT_OutStandardGroupModel("layer");
    CT_OutStandardGroupModel *cluster = new CT_OutStandardGroupModel("cluster");
    CT_OutStandardItemDrawableModel *pCircle = new CT_OutStandardItemDrawableModel(new CT_Circle(),
                                                                                    "circle");

    CT_OutStandardItemDrawableModel *pCircle2 = new CT_OutStandardItemDrawableModel(new CT_Circle(),
                                                                                     "circle2");

    // construit le modèle de sortie
    if(!layer->addGroup(cluster))
        printf("testCreateStandardItemGroupAndAddItemDrawableNotInModel :\t\tPRE ERROR 1\r\n");

    if(!cluster->addItem(pCircle))
        printf("testCreateStandardItemGroupAndAddItemDrawableNotInModel :\t\tPRE ERROR 2\r\n");

    CT_StandardItemGroup *rLayerG = new CT_StandardItemGroup(layer, 0, NULL);
    CT_StandardItemGroup *rClusterG1 = new CT_StandardItemGroup(cluster, 0, NULL);
    CT_StandardItemGroup *rClusterG2 = new CT_StandardItemGroup(cluster, 1, NULL);
    CT_Circle *rPcircle = new CT_Circle(pCircle2, 0, NULL, NULL);

    if(!rLayerG->addGroup(rClusterG1))
    {
        delete rClusterG1;
        printf("testCreateStandardItemGroupAndAddItemDrawableNotInModel :\t\tPRE ERROR 3\r\n");
    }

    if(!rLayerG->addGroup(rClusterG2))
    {
        delete rClusterG2;
        printf("testCreateStandardItemGroupAndAddItemDrawableNotInModel :\t\tPRE ERROR 4\r\n");
    }

    if(!rClusterG1->addItemDrawable(rPcircle))
    {
        delete rPcircle;
        printf("testCreateStandardItemGroupAndAddItemDrawableNotInModel :\t\tOK NOT ADDED\r\n");
    }
    else
    {
        printf("testCreateStandardItemGroupAndAddItemDrawableNotInModel :\t\tERROR ADDED (ok for performance)\r\n");
    }

    delete rLayerG;
    delete layer;
    delete pCircle2;
}

void testCreateStandardItemGroupAndAddItemDrawable()
{
    CT_OutStandardGroupModel *layer = new CT_OutStandardGroupModel("layer");
    CT_OutStandardGroupModel *cluster = new CT_OutStandardGroupModel("cluster");
    CT_OutStandardItemDrawableModel *pCircle = new CT_OutStandardItemDrawableModel(new CT_Circle(),
                                                                                    "circle");

    // construit le modèle de sortie
    if(!layer->addGroup(cluster))
        printf("testCreateStandardItemGroupAndAddItemDrawable :\t\t\tPRE ERROR 1\r\n");

    if(!cluster->addItem(pCircle))
        printf("testCreateStandardItemGroupAndAddItemDrawable :\t\t\tPRE ERROR 2\r\n");

    CT_StandardItemGroup *rLayerG = new CT_StandardItemGroup(layer, 0, NULL);
    CT_StandardItemGroup *rClusterG1 = new CT_StandardItemGroup(cluster, 0, NULL);
    CT_StandardItemGroup *rClusterG2 = new CT_StandardItemGroup(cluster, 1, NULL);
    CT_Circle *rPcircle = new CT_Circle(pCircle, 0, NULL, NULL);

    if(!rLayerG->addGroup(rClusterG1))
    {
        delete rClusterG1;
        printf("testCreateStandardItemGroupAndAddItemDrawable :\t\t\tPRE ERROR 3\r\n");
    }

    if(!rLayerG->addGroup(rClusterG2))
    {
        delete rClusterG2;
        printf("testCreateStandardItemGroupAndAddItemDrawable :\t\t\tPRE ERROR 4\r\n");
    }

    if(rClusterG1->addItemDrawable(rPcircle))
        printf("testCreateStandardItemGroupAndAddItemDrawable :\t\t\tOK ADDED\r\n");
    else
    {
        delete rPcircle;
        printf("testCreateStandardItemGroupAndAddItemDrawable :\t\t\tERROR NOT ADDED\r\n");
    }

    delete rLayerG;
    delete layer;
}*/

void printPoint(const CT_Point &p, const int &j, const size_t &index)
{
    printf("p%d (%d) : %f/%f/%f\r\n", j, index, p.data[0], p.data[1], p.data[2]);
}

void printPoints(CT_ABSTRACT_NMPCIR pcir)
{
    printf("-------\r\n");

    if(pcir.data() != NULL)
    {
        CT_Repository::ConstVertexIndexIterator begin = pcir->constBegin();
        CT_Repository::ConstVertexIndexIterator end = pcir->constEnd();

        int j = 0;
        while(begin != end)
        {
            printPoint(begin.cT(), j, begin.cIndex());
            ++j;
            ++begin;
        }
    }
    else
    {
        printf("data == NULL\r\n");
    }
}

void testLoadUnloadSizedPointsCloud()
{
    CT_ABSTRACT_NMPCIR pcir1 = PS_REPOSITORY->createNewPointCloud(5);
    CT_ABSTRACT_NMPCIR pcir2 = PS_REPOSITORY->createNewPointCloud(5);
    CT_ABSTRACT_NMPCIR pcir3 = PS_REPOSITORY->createNewPointCloud(5);

    CT_Repository::ConstVertexIndexIterator begin = pcir1->constBegin();
    CT_Repository::ConstVertexIndexIterator end = pcir1->constEnd();

    int i = 0;
    while(begin != end)
    {
        begin.cT() = CT_Point(i, i, i);
        ++i;
        ++begin;
    }

    begin = pcir2->constBegin();
    end = pcir2->constEnd();

    while(begin != end)
    {
        begin.cT() = CT_Point(i, i, i);
        ++i;
        ++begin;
    }

    begin = pcir3->constBegin();
    end = pcir3->constEnd();

    float val = i;

    while(begin != end)
    {
        begin.cT() = CT_Point(i, i, i);
        ++i;
        ++begin;
    }

    printPoints(pcir1);
    printPoints(pcir2);
    printPoints(pcir3);

    printf("--------\r\ndelete points cloud n°2\r\n");
    pcir2.clear();

    printPoints(pcir1);
    printPoints(pcir2);
    printPoints(pcir3);

    if(pcir3->constBegin().cT().data[0] != val)
    {
        printf("--------\r\nerror points cloud !!\r\n");
    }
    else
    {
        printf("--------\r\nOK points cloud\r\n");
    }
}

void printEdge(const CT_Edge *e, const int &j, const size_t &index)
{
    if(e != NULL)
    {
        printf("he%d (%d) :\r\n", j, index);
        printPoint(*e->pointAt(0), 0, e->iPointAt(0));
        printPoint(*e->pointAt(1), 1, e->iPointAt(1));
        printf("-- he --\r\n");
    }
    else
    {
        printf("he%d (NULL)\r\n", j);
    }
}

void printMesh(CT_Mesh *m)
{
    printf("------- NEW MESH -------\r\n");

    if(m != NULL)
    {
        if(m->pVert() != NULL)
        {
            CT_Mesh::VertexIndexIterator begin = m->vert().begin();
            CT_Mesh::VertexIndexIterator end = m->vert().end();

            int j = 0;
            while(begin != end)
            {
                printPoint(begin.cT(), j, begin.cIndex());
                ++j;
                ++begin;
            }
        }
        else
        {
            printf("NO VERTEX\r\n");
        }

        if(m->pHedge() != NULL)
        {
            CT_Mesh::HEdgeIndexIterator begin = m->hedge().begin();
            CT_Mesh::HEdgeIndexIterator end = m->hedge().end();

            int j = 0;
            while(begin != end)
            {
                printEdge(&begin.cT(), j, begin.cIndex());
                printPoint(*begin.cT().P(0), 0, begin.cT().iPointAt(0));
                printPoint(*begin.cT().P(1), 1, begin.cT().iPointAt(1));
                ++j;
                ++begin;
            }
        }
        else
        {
            printf("NO HALF EDGE\r\n");
        }

        if(m->pFace() != NULL)
        {
            CT_Mesh::FaceIndexIterator begin = m->face().begin();
            CT_Mesh::FaceIndexIterator end = m->face().end();

            int j = 0;
            while(begin != end)
            {
                printf("f%d (%d) :\r\n", j, begin.cIndex());
                printEdge(begin.cT().edge(),0, begin.cT().iEdge());
                //printEdge(begin.cT().edge()->next(),1);
                //printEdge(begin.cT().edge()->previous(),2);
                printEdge(begin.cT().edge()->twin(),3, begin.cT().edge()->iTwin());
                ++j;
                ++begin;
            }
        }
        else
        {
            printf("NO FACE\r\n");
        }
    }
    else
    {
        printf("Mesh == NULL\r\n");
    }

    printf("------- NEW MESH END -------\r\n");
}

void allocateMesh(CT_Mesh *m, float valP)
{
    CT_MeshAllocatorT<CT_Mesh>::AddVertices(*m, 4);
    CT_MeshAllocatorT<CT_Mesh>::AddHEdges(*m,10);
    CT_MeshAllocatorT<CT_Mesh>::AddFaces(*m,2);

    CT_Mesh::VertexPointer ivp[4];

    CT_Mesh::VertexIndexIterator vi = m->vert().begin();
    ivp[0]=&*vi;(*vi).P()=CT_Point ( 0, 0, 0); ++vi;
    ivp[1]=&*vi;(*vi).P()=CT_Point ( valP, 0, 0); ++vi;
    ivp[2]=&*vi;(*vi).P()=CT_Point ( valP, valP, 0); ++vi;
    ivp[3]=&*vi;(*vi).P()=CT_Point ( 0, valP, 0);

    CT_Mesh::HEdgePointer iep[10];

    CT_Mesh::EdgeIndexIterator hei = m->hedge().begin();
    iep[0] = &*hei; (*hei).setPoint0(ivp[0]);  (*hei).setPoint1(ivp[1]); ++hei; //hedge 0
    iep[1] = &*hei; (*hei).setPoint0(ivp[1]);  (*hei).setPoint1(ivp[2]); ++hei; //hedge 1
    iep[2] = &*hei; (*hei).setPoint0(ivp[2]);  (*hei).setPoint1(ivp[3]); ++hei; //hedge 2
    iep[3] = &*hei; (*hei).setPoint0(ivp[3]);  (*hei).setPoint1(ivp[1]); ++hei; //hedge 3
    iep[4] = &*hei; (*hei).setPoint0(ivp[0]);  (*hei).setPoint1(ivp[2]); ++hei; //hedge 4

    iep[5] = &*hei; (*hei).setTwin(iep[0]); iep[0]->setTwin(&(*hei)); (*hei).setPoint0(ivp[1]);  (*hei).setPoint1(ivp[0]); ++hei; // twin hedge 0
    iep[6] = &*hei; (*hei).setTwin(iep[1]); iep[1]->setTwin(&(*hei)); (*hei).setPoint0(ivp[2]);  (*hei).setPoint1(ivp[1]); ++hei; // twin hedge 1
    iep[7] = &*hei; (*hei).setTwin(iep[2]); iep[2]->setTwin(&(*hei)); (*hei).setPoint0(ivp[3]);  (*hei).setPoint1(ivp[2]); ++hei; // twin hedge 2
    iep[8] = &*hei; (*hei).setTwin(iep[3]); iep[3]->setTwin(&(*hei)); (*hei).setPoint0(ivp[1]);  (*hei).setPoint1(ivp[3]); ++hei; // twin hedge 3
    iep[9] = &*hei; (*hei).setTwin(iep[4]); iep[4]->setTwin(&(*hei)); (*hei).setPoint0(ivp[2]);  (*hei).setPoint1(ivp[0]); // twin hedge 3

    CT_Mesh::FacePointer ifp[2];

    CT_Mesh::FaceIndexIterator fi = m->face().begin();
    ifp[0] = &*fi; (*fi).setEdge(iep[0]);  ++fi;
    ifp[1] = &*fi; (*fi).setEdge(iep[3]);

    ifp[0] = NULL;
}

void testLoadUnloadMesh()
{
    CT_Mesh *m1 = new CT_Mesh();
    CT_Mesh *m2 = new CT_Mesh();
    CT_Mesh *m3 = new CT_Mesh();

    float valP = 5;

    allocateMesh(m1, 10);
    allocateMesh(m2, 15);
    allocateMesh(m3, valP);

    printMesh(m1);
    printMesh(m2);
    printMesh(m3);

    delete m2;
    m2 = NULL;

    printMesh(m1);
    printMesh(m2);
    printMesh(m3);

    if((m3->vert().begin()+1).cT().data[0] != valP)
    {
        printf("--------\r\nerror points cloud in mesh!!\r\n");
    }
    else
    {
        printf("--------\r\nOK points cloud in mesh\r\n");
    }

    delete m1;
    delete m3;
}

int main(int argc, char *argv[])
{
    /*bool showInModel[] = { false, false, false, false, false, false, false };
    bool showInOutModel[] = { false, false, false, false, false, false, false, false, false };

    // TEST des nouvelles fonctionnalités (voir sortie de l'application, tout doit être OK)
    printf("\r\n");

    // TEST AJOUT (groupe et item)
    testAddItemWithSameNameModel(showInModel[0]);                                     // item ayant le même nom est rejeté lorsq'il est ajouté au même groupe
    testAddItemWithSameType(showInModel[1]);                                          // item ayant le même type est rejeté  lorsq'il est ajouté au même groupe
    testAddItemWithDifferentNameModelAndType(showInModel[2]);                         // item ayant le nom ET le type différent est accepté lorsq'il est ajouté au même groupe
    testAddGroupWithSameNameModel(showInModel[3]);                                    // groupe ayant le même nom est rejeté lorsqu'il est ajouté dans l'arborescence
    testAddGroupWithGroupWithSameNameModel(showInModel[4]);                           // groupe (1) ayant un groupe (2) ayant le même nom est rejeté lorsque le groupe (1) est ajouté dans l'arborescence
    testAddGroupWithSameItemNameModel(showInModel[5]);                                // groupe (1) ayant un groupe (2) ayant un item (I1) ayant le même nom que l'item (I2) est rejeté lorsque le groupe (1) est ajouté dans l'arborescence qui contient déjà un item (I2) ayant le même nom que l'item I1
    testAddGroupWithDifferentItemNameModel(showInModel[6]);                           // groupe (1) ayant un groupe (2) ayant un item (I1) ayant un nom différent que l'item (I2), déjà présent dans l'arborescence, est accepté lorsque le groupe (1) est ajouté dans l'arborescence

    printf("\r\n-------------\r\n");

    // TEST RECHERCHE SIMPLE (cherche un modèle d'entrée dans un modèle de sortie avec des groupes standard)
    testSiSearchIdenticalInputModelInOutputModel(showInOutModel[0]);                     // un modèle d'entrée composé uniquement de groupes recherche sa structure dans un modèle de sortie identique
    testSiSearchDifferentInputModelInOutputModel(showInOutModel[1]);                     // un modèle d'entrée composé uniquement de groupes ne trouve pas sa structure dans un modèle de sortie ayant un groupe en moins
    testSiSearchInputModelInOutputModelWithIdenticalItems(showInOutModel[2]);            // un modèle d'entrée composé de groupes et d'un item recherche sa structure dans un modèle de sortie identique
    testSiSearchInputModelInOutputModelWithDifferentItemsType(showInOutModel[3]);        // un modèle d'entrée composé de groupes et d'un item ne trouve pas sa structure dans un modèle de sortie ayant un item de type différent

    printf("\r\n-------------\r\n");

    // TEST RECHERCHE COMPLEXE (cherche un modèle d'entrée dans un modèle de sortie avec des groupes modèles d'entrées du type OneOrMore)
    testCoSearchIdenticalInputModelInOutputModel(showInOutModel[4]);
    testCoSearchDifferentInputModelInOutputModel(showInOutModel[5]);
    testCoSearchInputModelInOutputModelWithIdenticalItems(showInOutModel[6]);
    testCoSearchInputModelInOutputModelWithDifferentItemsType(showInOutModel[7]);
    testCoSearchInputModelInOutputModelWithDifferentStructure(showInOutModel[8]);

    printf("\r\n-------------\r\n");

    // TEST RECHERCHE DE POSSIBILITE DANS DES RESULTATS MODELES
    testCoSearchInResultModelInOutputResultModel();

    printf("\r\n-------------\r\n");

    // TEST CREATION D'ITEMGROUP ET AJOUT D'ITEMGROUP ET ITEMDRAWABLE
    testCreateStandardItemGroupAndAddItemDrawableNotInModel();
    testCreateStandardItemGroupAndAddItemDrawable();*/

    // TEST NUAGES de Points
    //testLoadUnloadSizedPointsCloud();

    // TEST Mesh
    testLoadUnloadMesh();

    return 0;
}
