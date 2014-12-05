#include "pb_stepmatchitemspositions.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_point2d.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include <limits>
#include <eigen/Eigen/Core>
#include <eigen/Eigen/Dense>
#include <eigen/Eigen/Geometry>

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
    _coef_nbRwc = 1;
    _coef_nbTwc = 1;
    _coef_nbSim = 1;

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
    res_trans->addItemModel(DEFout_grp, DEFout_transpos, new CT_Point2D(), tr("Positions transformées"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepMatchItemsPositions::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble("Distance maximale entre points appariés :", "m", 0, 100, 3, _distThreshold, 1);
    configDialog->addDouble("Taille relative minimum entre items appariés :", "", 0, 1, 2, _relativeSizeThreshold, 1);
    configDialog->addDouble("Taille relative minimale :", "", 0, 1, 2, _minRelativeSize, 1);
    configDialog->addDouble("Poid du critère Nb. pos. de référence ayant une pos. transformée proche :", "", 0, 1000, 2, _coef_nbRwc);
    configDialog->addDouble("Poid du critère Nb. pos. transformées ayant une pos. de référence proche :", "", 0, 1000, 2, _coef_nbTwc);
    configDialog->addDouble("Poid du critère Nb. pos. transformées ayant une pos. de référence proche avec une taille similaire :", "", 0, 1000, 2, _coef_nbSim);

}

void PB_StepMatchItemsPositions::compute()
{

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_refpos = inResultList.at(0);
    CT_ResultGroup* resIn_transpos = inResultList.at(1);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_trans = outResultList.at(0);

    QList<QPair<Eigen::Vector2f, double> > refPositions;
    QList<QPair<Eigen::Vector2f, double> > transPositions;

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

            refPositions.append(QPair<Eigen::Vector2f, double>(Eigen::Vector2f(xRef, yRef), valRef));
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

            transPositions.append(QPair<Eigen::Vector2f, double>(Eigen::Vector2f(xTrans, yTrans), valTrans));
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


    float bestScore = 0;

    Eigen::Vector2f rotationCenter;
    Eigen::Vector2f translationVector;
    Eigen::Matrix2f rotationMatrix;


    // Points Matching algorithm
    for (int refCounter = 0 ; refCounter < refPositions.size() ; refCounter++)
    {
        const Eigen::Vector2f &refPos = refPositions.at(refCounter).first;

        for (int transCounter = 0 ; transCounter < transPositions.size() ; transCounter++)
        {
            const Eigen::Vector2f &transPos = transPositions.at(transCounter).first;
            double transVal = transPositions.at(transCounter).second;

            if (transVal >= _relativeSizeThreshold)
            {
                // Do translation of trans data
                Eigen::Vector2f delta = transPos - refPos;
                for (int i = 0 ; i < transPositions.size() ; i++)
                {
                    transPositions[i].first -= delta;
                }

                // Search for second trans point
                for (int i = 0 ; i < transPositions.size() ; i++)
                {
                    if (i != transCounter)
                    {
                        const Eigen::Vector2f &transPos_i = transPositions.at(i).first;

                        // Search for second ref point
                        for (int j = 0 ; j < refPositions.size() ; j++)
                        {
                            if (j != refCounter)
                            {
                                const Eigen::Vector2f &refPos_j = refPositions.at(j).first;

                                Eigen::Vector2f vectRef = refPos_j - refPos;
                                Eigen::Vector2f vectTrans = transPos_i - transPos;

                                // If distances have the same value +- _distThreshold
                                if (fabs(vectRef.norm() - vectTrans.norm()) < _distThreshold)
                                {
                                    float cosTheta = vectTrans.dot(vectRef)/(vectTrans.norm()*vectRef.norm());
                                    float theta = acos(cosTheta);

                                    float orientation = vectRef.x()*vectTrans.y() - vectRef.y()*vectTrans.x();

                                    if (orientation < 0) {theta = 3.0*M_PI/2.0 + (M_PI/2.0 - theta);}

                                    Eigen::Matrix2f rotation;
                                    rotation << cos(theta), -sin(theta),
                                                sin(theta),  cos(theta);

                                    // Apply rotation to all trans points
                                    for (int k = 0 ; k < transPositions.size() ; k++)
                                    {
                                        Eigen::Vector2f &transPos_k = transPositions[i].first;
                                        transPos_k = rotation*(transPos_k - refPos) + refPos;
                                    }

                                    // Compute match scores

                                    // Number of ref points with a trans point within _distThreshold
                                    int Nbref_with_Corresp = 0;
                                    // Number of trans points with a ref point within _distThreshold
                                    int Nbtrans_with_Corresp = 0;
                                    // Number of trans points with a ref point within _distThreshold AND relatives sizes are similar (difference < _relativeSizeThreshold)
                                    int NbtransRef_Similarity = 0;

                                    for (int rf = 0 ; rf < refPositions.size() ; rf++)
                                    {
                                        const Eigen::Vector2f &rfPos = refPositions.at(rf).first;
                                        bool trFound = false;

                                        for (int tr = 0 ; tr < transPositions.size() && !trFound; tr++)
                                        {
                                            const Eigen::Vector2f &trPos = transPositions.at(tr).first;
                                            float dist = (trPos - rfPos).norm();

                                            if (dist < _distThreshold)
                                            {
                                                Nbref_with_Corresp++;
                                                trFound = true;
                                            }
                                        }
                                    }

                                    for (int tr = 0 ; tr < transPositions.size(); tr++)
                                    {
                                        const Eigen::Vector2f &trPos = transPositions.at(tr).first;
                                        double trVal = transPositions.at(tr).second;
                                        bool rfSimilar = false;

                                        for (int rf = 0 ; rf < refPositions.size() && !rfSimilar; rf++)
                                        {
                                            const Eigen::Vector2f &rfPos = refPositions.at(rf).first;
                                            double rfVal = refPositions.at(rf).second;
                                            float dist = (trPos - rfPos).norm();

                                            if (dist < _distThreshold)
                                            {
                                                Nbtrans_with_Corresp++;
                                                float sizeDiff = fabs(rfVal - trVal);

                                                if (sizeDiff < _relativeSizeThreshold)
                                                {
                                                    NbtransRef_Similarity++;
                                                    rfSimilar = true;
                                                }
                                            }
                                        }
                                    }

                                    // Compute global score
                                    float globalScore = _coef_nbRwc * ((double)Nbref_with_Corresp    / (double)refPositions.size())   +
                                                        _coef_nbTwc * ((double)Nbtrans_with_Corresp  / (double)transPositions.size()) +
                                                        _coef_nbSim * ((double)NbtransRef_Similarity / (double)transPositions.size()) ;

                                    if (globalScore > bestScore)
                                    {
                                        bestScore = globalScore;
                                        rotationCenter = refPos;
                                        rotationMatrix = rotation;
                                        translationVector = delta;
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Compute transformation matrix
//    Eigen::Matrix<float, 3, 3> transformationMatrix;
//    transformationMatrix << cos(thetaRotation), -sin(thetaRotation), (rotationCenter[0]*(1 - cos(thetaRotation)) + rotationCenter[1]*sin(thetaRotation) + translationVector[0]),
//                            sin(thetaRotation),  cos(thetaRotation), (rotationCenter[1]*(1 - cos(thetaRotation)) - rotationCenter[0]*sin(thetaRotation) + translationVector[1]),
//                            0                 , 0                  , 1                                                                                                         ;

    qDebug() << "cos(theta) =" << rotationMatrix(0,0);
    qDebug() << "-sin(theta)=" << rotationMatrix(0,1);
    qDebug() << "sin(theta) =" << rotationMatrix(1,0);
    qDebug() << "cos(theta) =" << rotationMatrix(1,1);

    // Apply selected transformation to trans data
    for (int i = 0 ; i < transPositions.size() ; i++)
    {
//        Eigen::Vector3f tmp;
//        tmp[0] = transPositions[i].first[0];
//        tmp[1] = transPositions[i].first[1];
//        tmp[2] = 1;
//        tmp = transformationMatrix*tmp;

//        transPositions[i].first[0] = tmp[0];
//        transPositions[i].first[1] = tmp[1];


        transPositions[i].first -= translationVector;
        transPositions[i].first = rotationMatrix*(transPositions[i].first - rotationCenter) + rotationCenter;


        // OUT results creation (move it to the appropried place in the code)
        CT_StandardItemGroup* grp_grp= new CT_StandardItemGroup(DEFout_grp, res_trans);
        res_trans->addGroup(grp_grp);

//        CT_Point2D* item_transpos = new CT_Point2D(DEFout_transpos, res_trans, new CT_Point2DData(tmp[0], tmp[1]));
        CT_Point2D* item_transpos = new CT_Point2D(DEFout_transpos, res_trans, new CT_Point2DData(transPositions[i].first[0], transPositions[i].first[1]));
        grp_grp->addItemDrawable(item_transpos);
    }

}
