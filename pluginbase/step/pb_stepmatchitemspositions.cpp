#include "pb_stepmatchitemspositions.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_referencepoint.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include <limits>
#include <QVector2D>

// Alias for indexing models
#define DEFin_Resrefpos "Resrefpos"
#define DEFin_grpref "grpref"
#define DEFin_refpos "refpos"
#define DEFin_refx "refx"
#define DEFin_refy "refy"
#define DEFin_refvalue "refvalue"

#define DEFin_Restranspos "Restranspos"
#define DEFin_grptrans "grptrans"
#define DEFin_transpos "transpos"
#define DEFin_transx "transx"
#define DEFin_transy "transy"
#define DEFin_transvalue "transvalue"

#define DEFout_trans "trans"
#define DEFout_grp "grp"
#define DEFout_transpos "transpos"


// Constructor : initialization of parameters
PB_StepMatchItemsPositions::PB_StepMatchItemsPositions(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _distThreshold = 0.2;
    _relativeSizeThreshold = 0.2;
    _minRelativeSize = 0;
}

// Step description (tooltip of contextual menu)
QString PB_StepMatchItemsPositions::getStepDescription() const
{
    return tr("Co-registration entre deux ensembles de positions 2D");
}

// Step detailled description
QString PB_StepMatchItemsPositions::getStepDetailledDescription() const
{
    return tr("No detailled description for this step");
}

// Step URL
QString PB_StepMatchItemsPositions::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepMatchItemsPositions::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepMatchItemsPositions(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepMatchItemsPositions::createInResultModelListProtected()
{
    CT_InResultModelGroup *resIn_refpos = createNewInResultModel(DEFin_Resrefpos, tr("Positions de référence"));
    resIn_refpos->setRootGroup(DEFin_grpref, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_refpos->addItemModel(DEFin_grpref, DEFin_refpos, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item de référence"));
    resIn_refpos->addItemAttributeModel(DEFin_refpos, DEFin_refx, QList<QString>() << CT_AbstractCategory::DATA_X, CT_AbstractCategory::FLOAT, tr("Coordonnée X"));
    resIn_refpos->addItemAttributeModel(DEFin_refpos, DEFin_refy, QList<QString>() << CT_AbstractCategory::DATA_Y, CT_AbstractCategory::FLOAT, tr("Coordonnée Y"));
    resIn_refpos->addItemAttributeModel(DEFin_refpos, DEFin_refvalue, QList<QString>() << CT_AbstractCategory::DATA_NUMBER, CT_AbstractCategory::NUMBER, tr("Valeur"));

    CT_InResultModelGroup *resIn_transpos = createNewInResultModel(DEFin_Restranspos, tr("Positions à transformer"));
    resIn_transpos->setRootGroup(DEFin_grptrans, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_transpos->addItemModel(DEFin_grptrans, DEFin_transpos, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item à transformer"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transx, QList<QString>() << CT_AbstractCategory::DATA_X, CT_AbstractCategory::FLOAT, tr("Coordonnée X"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transy, QList<QString>() << CT_AbstractCategory::DATA_Y, CT_AbstractCategory::FLOAT, tr("Coordonnée Y"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transvalue, QList<QString>() << CT_AbstractCategory::DATA_NUMBER, CT_AbstractCategory::NUMBER, tr("Valeur"));

}

// Creation and affiliation of OUT models
void PB_StepMatchItemsPositions::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_trans = createNewOutResultModel(DEFout_trans, tr("Positions transformées"));
    res_trans->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
    res_trans->addItemModel(DEFout_grp, DEFout_transpos, new CT_ReferencePoint(), tr("Positions transformées"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepMatchItemsPositions::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Distance maximale entre points appariés :", "m", 0, 100, 3, _distThreshold, 1);
    configDialog->addDouble("Taille relative minimum entre items appariés :", "", 0, 1, 2, _relativeSizeThreshold, 1);
    configDialog->addDouble("Taille relative minimale :", "", 0, 1, 2, _minRelativeSize, 1);
}

void PB_StepMatchItemsPositions::compute()
{
    // DONT'T FORGET TO ADD THIS STEP TO THE PLUGINMANAGER !!!!!

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_refpos = inResultList.at(0);
    CT_ResultGroup* resIn_transpos = inResultList.at(1);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_trans = outResultList.at(0);

    QList<QPair<QVector2D, double> > refPositions;
    QList<QPair<QVector2D, double> > transPositions;

    double minRefValue = std::numeric_limits<double>::max();
    double maxRefValue = -std::numeric_limits<double>::max();
    double minTransValue = std::numeric_limits<double>::max();
    double maxTransValue = -std::numeric_limits<double>::max();

    // create refPositions List
    CT_ResultGroupIterator itIn_grpref(resIn_refpos, this, DEFin_grpref);
    while (itIn_grpref.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn_grpref = (CT_AbstractItemGroup*) itIn_grpref.next();
        
        const CT_AbstractSingularItemDrawable* itemIn_refpos = (CT_AbstractSingularItemDrawable*)grpIn_grpref->firstItemByINModelName(this, DEFin_refpos);
        if (itemIn_refpos != NULL)
        {            
            float xRef = itemIn_refpos->firstItemAttributeByINModelName(this, DEFin_refx)->toFloat(itemIn_refpos, NULL);
            float yRef = itemIn_refpos->firstItemAttributeByINModelName(this, DEFin_refy)->toFloat(itemIn_refpos, NULL);
            float valRef = itemIn_refpos->firstItemAttributeByINModelName(this, DEFin_refvalue)->toDouble(itemIn_refpos, NULL);

            if (valRef < minRefValue) {minRefValue = valRef;}
            if (valRef > maxRefValue) {maxRefValue = valRef;}

            refPositions.append(QPair<QVector2D, double>(QVector2D(xRef, yRef), valRef));
        }
    }
    
    // create transPositions List
    CT_ResultGroupIterator itIn_grptrans(resIn_transpos, this, DEFin_grptrans);
    while (itIn_grptrans.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn_grptrans = (CT_AbstractItemGroup*) itIn_grptrans.next();
        
        const CT_AbstractSingularItemDrawable* itemIn_transpos = (CT_AbstractSingularItemDrawable*)grpIn_grptrans->firstItemByINModelName(this, DEFin_transpos);
        if (itemIn_transpos != NULL)
        {
            float xTrans = itemIn_transpos->firstItemAttributeByINModelName(this, DEFin_transx)->toFloat(itemIn_transpos, NULL);
            float yTrans = itemIn_transpos->firstItemAttributeByINModelName(this, DEFin_transy)->toFloat(itemIn_transpos, NULL);
            float valTrans = itemIn_transpos->firstItemAttributeByINModelName(this, DEFin_transvalue)->toDouble(itemIn_transpos, NULL);

            if (valTrans < minTransValue) {minTransValue = valTrans;}
            if (valTrans > maxTransValue) {maxTransValue = valTrans;}

            transPositions.append(QPair<QVector2D, double>(QVector2D(xTrans, yTrans), valTrans));
        }
    }
    
    // Convert values to relatives values
    double deltaRef = maxRefValue - minRefValue;
    for (int i = 0 ; i < refPositions.size() ; i++)
    {
        refPositions[i].second = (refPositions.at(i).second - minRefValue) / deltaRef;
    }

    double deltaTrans = maxTransValue - minTransValue;
    for (int i = 0 ; i < transPositions.size() ; i++)
    {
        transPositions[i].second = (transPositions.at(i).second - minTransValue) / deltaTrans;
    }


    // Algo de Points Matching en lui même
    for (int refCounter = 0 ; refCounter < refPositions.size() ; refCounter++)
    {
        const QVector2D &refPos = refPositions.at(refCounter).first;
        double refVal = refPositions.at(refCounter).second;

        for (int transCounter = 0 ; transCounter < transPositions.size() ; transCounter++)
        {
            const QVector2D &transPos = transPositions.at(transCounter).first;
            double transVal = transPositions.at(transCounter).second;

            if (transVal >= _relativeSizeThreshold)
            {
                // Do translation of trans data
                QVector2D delta = transPos - refPos;
                for (int i = 0 ; i < transPositions.size() ; i++)
                {
                    transPositions[i].first -= delta;
                }

                // Search for second trans point
                for (int i = 0 ; i < transPositions.size() ; i++)
                {
                    if (i != transCounter)
                    {
                        const QVector2D &transPos_i = transPositions.at(i).first;
                        double transVal_i = transPositions.at(i).second;

                        // Search for second ref point
                        for (int j = 0 ; j < refPositions.size() ; j++)
                        {
                            if (j != refCounter)
                            {
                                const QVector2D &refPos_j = refPositions.at(i).first;
                                double refVal_j = refPositions.at(i).second;

                                QVector2D vectRef = refPos_j - refPos;
                                QVector2D vectTrans = transPos_i - transPos;

                                // If distances have the same value +- _distThreshold
                                if (fabs(vectRef.length() - vectTrans.length()) < _distThreshold)
                                {
                                    float dotProd = QVector2D::dotProduct(vectRef, vectTrans);


                                }

                            }
                        }
                    }
                }
            }
        }
    }




    // OUT results creation (move it to the appropried place in the code)
    CT_StandardItemGroup* grp_grp= new CT_StandardItemGroup(DEFout_grp, res_trans);
    res_trans->addGroup(grp_grp);
    
    //CT_ReferencePoint* item_transpos = new CT_ReferencePoint(DEFout_transpos, res_trans);
    //grp_grp->addItemDrawable(item_transpos);



}
