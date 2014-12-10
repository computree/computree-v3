#include "pb_stepmatchitemspositions.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_referencepoint.h"
#include "ct_itemdrawable/ct_line.h"
#include "ct_itemdrawable/ct_attributeslist.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include <limits>
#include <eigen/Eigen/Core>
#include <eigen/Eigen/Dense>
#include <eigen/Eigen/Geometry>
#include <QFile>
#include <QTextStream>

// Alias for indexing models
#define DEFin_Resrefpos "Resrefpos"
#define DEFin_grpref "grpref"
#define DEFin_refpos "refpos"
#define DEFin_refx "refx"
#define DEFin_refy "refy"
#define DEFin_refvalue "refvalue"
#define DEFin_refid "refid"

#define DEFin_Restranspos "Restranspos"
#define DEFin_grptrans "grptrans"
#define DEFin_transpos "transpos"
#define DEFin_transx "transx"
#define DEFin_transy "transy"
#define DEFin_transvalue "transvalue"
#define DEFin_transid "transid"

#define DEFout_rootGrp "rootGrp"
#define DEFout_attributes "attributes"
#define DEFout_attRmseDist "attRmseDist"
#define DEFout_attRmseVal "attRmseVal"
#define DEFout_attMaxDist "attMaxDist"
#define DEFout_attMaxDistDiff "attMaxDistDiff"
#define DEFout_trans "trans"
#define DEFout_grp "grp"
#define DEFout_transpos "transpos"

#define DEFout_trans2 "trans2"
#define DEFout_grp2 "grp2"
#define DEFout_transpos2 "transpos2"
#define DEFout_transposCorresp "transposCorresp"
#define DEFout_correspLine "correspLine"
#define DEFout_transId "transId"
#define DEFout_refCorrespId "refCorespId"
#define DEFout_deltaValue "deltaValue"
#define DEFout_distance "distance"

// Constructor : initialization of parameters
PB_StepMatchItemsPositions::PB_StepMatchItemsPositions(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _distThreshold = 3;
    _relativeSizeThreshold = 0.2;
    _minRelativeSize = 0;
    _coef_nbRwc = 1;
    _coef_nbTwc = 1;
    _coef_nbSim = 1;
    _drawMode = 0;
    _relativeMode = 0;
    _minval = 0;
    _maxval = 5;
    _exportReport = true;
    _reportFileName.append("matchingReport.txt");
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
    resIn_refpos->addItemAttributeModel(DEFin_refpos, DEFin_refid, QList<QString>() << CT_AbstractCategory::DATA_ID, CT_AbstractCategory::ANY, tr("ID"));

    CT_InResultModelGroup *resIn_transpos = createNewInResultModel(DEFin_Restranspos, tr("Positions à transformer"));
    resIn_transpos->setRootGroup(DEFin_grptrans, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_transpos->addItemModel(DEFin_grptrans, DEFin_transpos, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item à transformer"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transx, QList<QString>() << CT_AbstractCategory::DATA_X, CT_AbstractCategory::FLOAT, tr("Coordonnée X"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transy, QList<QString>() << CT_AbstractCategory::DATA_Y, CT_AbstractCategory::FLOAT, tr("Coordonnée Y"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transvalue, QList<QString>() << CT_AbstractCategory::DATA_NUMBER, CT_AbstractCategory::NUMBER, tr("Valeur"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transid, QList<QString>() << CT_AbstractCategory::DATA_ID, CT_AbstractCategory::ANY, tr("ID"));

}

// Creation and affiliation of OUT models
void PB_StepMatchItemsPositions::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_trans2 = createNewOutResultModel(DEFout_trans2, tr("Positions transformées"));
    res_trans2->setRootGroup(DEFout_rootGrp, new CT_StandardItemGroup(), tr("Groupe racine"));
    res_trans2->addItemModel(DEFout_rootGrp, DEFout_attributes, new CT_AttributesList(), tr("Qualité de Matching"));
    res_trans2->addItemAttributeModel(DEFout_attributes, DEFout_attRmseDist,
                                      new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_NUMBER),
                                      tr("RMSE Dist"));
    res_trans2->addItemAttributeModel(DEFout_attributes, DEFout_attRmseVal,
                                      new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_NUMBER),
                                      tr("RMSE Val"));
    res_trans2->addItemAttributeModel(DEFout_attributes, DEFout_attMaxDist,
                                      new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_NUMBER),
                                      tr("Max Dist"));
    res_trans2->addItemAttributeModel(DEFout_attributes, DEFout_attMaxDistDiff,
                                      new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_NUMBER),
                                      tr("Max Val diff"));

    res_trans2->addGroupModel(DEFout_rootGrp, DEFout_grp2, new CT_StandardItemGroup(), tr("Groupe"));
    res_trans2->addItemModel(DEFout_grp2, DEFout_transpos2, new CT_ReferencePoint(), tr("Position transformée"));
    res_trans2->addItemAttributeModel(DEFout_transpos2, DEFout_transId,
                                      new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_ID),
                                      tr("ID position transformée"));
    res_trans2->addItemAttributeModel(DEFout_transpos2, DEFout_refCorrespId,
                                      new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_ID),
                                      tr("ID position de référence"));
    res_trans2->addItemAttributeModel(DEFout_transpos2, DEFout_deltaValue,
                                      new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_NUMBER),
                                      tr("Ecart ValTrans - ValRef"));
    res_trans2->addItemAttributeModel(DEFout_transpos2, DEFout_distance,
                                      new CT_StdItemAttributeT<float>(CT_AbstractCategory::DATA_NUMBER),
                                      tr("Distance 2D Trans - Ref"));


    res_trans2->addItemModel(DEFout_grp2, DEFout_transposCorresp, new CT_ReferencePoint(), tr("Position de référence correspondante"));
    res_trans2->addItemModel(DEFout_grp2, DEFout_correspLine, new CT_Line(), tr("Ligne de correspondance"));

    CT_OutResultModelGroup *res_trans = createNewOutResultModel(DEFout_trans, tr("Positions intermédiaires"));
    res_trans->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
    res_trans->addItemModel(DEFout_grp, DEFout_transpos, new CT_ReferencePoint(), tr("Positions intermédiaires"));

}

// Semi-automatic creation of step parameters DialogBox
void PB_StepMatchItemsPositions::createPostConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

    configDialog->addDouble(tr("Distance maximale entre points appariés :"), "m", 0, 100, 3, _distThreshold, 1);
    configDialog->addDouble(tr("Seuil de taille relative minimum entre items appariés :"), "", 0, 1, 2, _relativeSizeThreshold, 1);
    configDialog->addDouble(tr("Taille relative minimale :"), "", 0, 1, 2, _minRelativeSize, 1);
    configDialog->addDouble(tr("Poid du critère Nb. pos. de référence ayant une pos. transformée proche :"), "", 0, 1000, 2, _coef_nbRwc);
    configDialog->addDouble(tr("Poid du critère Nb. pos. transformées ayant une pos. de référence proche :"), "", 0, 1000, 2, _coef_nbTwc);
    configDialog->addDouble(tr("Poid du critère Nb. pos. transformées ayant une pos. de référence proche avec une taille similaire :"), "", 0, 1000, 2, _coef_nbSim);

    configDialog->addText(tr("Mode de représentation :"), "", "");

    CT_ButtonGroup &bg_drawMode = configDialog->addButtonGroup(_drawMode);

    configDialog->addExcludeValue("", "", tr("Valeur Z"), bg_drawMode, 0);
    configDialog->addExcludeValue("", "", tr("Cercle"), bg_drawMode, 1);
    configDialog->addText(tr("Comment représenter en Z la variable de taille ?"), "", "");

    CT_ButtonGroup &bg_relativeMode = configDialog->addButtonGroup(_relativeMode);

    configDialog->addExcludeValue("", "", tr("Valeur absolue"), bg_relativeMode, 0);
    configDialog->addExcludeValue("", "", tr("Valeur relative"), bg_relativeMode, 1);
    configDialog->addText(tr("En cas de valeur relative :"), "", "");
    configDialog->addDouble(tr("Valeur de Z/Rayon minimum"), "m", -1e+09, 1e+09, 2, _minval, 1);
    configDialog->addDouble(tr("Valeur de Z/Rayon maximum"), "m", -1e+09, 1e+09, 2, _maxval, 1);

    configDialog->addEmpty();
    configDialog->addBool(tr("Exporter un rapport de Recalage"), "", "", _exportReport);
    configDialog->addFileChoice(tr("Fichier d'export du rapport de Recalage"), CT_FileChoiceButton::OneNewFile, tr("Fichier texte (*.txt)"), _reportFileName);

}

void PB_StepMatchItemsPositions::compute()
{

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_refpos = inResultList.at(0);
    CT_ResultGroup* resIn_transpos = inResultList.at(1);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_trans = outResultList.at(1);
    CT_ResultGroup* res_trans2 = outResultList.at(0);
    CT_StandardItemGroup *rootGroup = new CT_StandardItemGroup(DEFout_rootGrp,res_trans2);
    res_trans2->addGroup(rootGroup);

    QList<QPair<Eigen::Vector2f, float> > refPositions;
    QList<QPair<Eigen::Vector2f, float> > transPositions;
    QList<QPair<Eigen::Vector2f, float> > transPositionsTmp;

    QMap<int, QString> refIds;
    QMap<int, QString> transIds;
    QMap<int, float> deltaDistMap;
    QMap<int, float> deltaValMap;

    float minRefValue = std::numeric_limits<float>::max();
    float maxRefValue = -std::numeric_limits<float>::max();
    float minTransValue = std::numeric_limits<float>::max();
    float maxTransValue = -std::numeric_limits<float>::max();

    // create refPositions List
    int cptRef = 0;
    CT_ResultGroupIterator itIn_grpref(resIn_refpos, this, DEFin_grpref);
    while (itIn_grpref.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn_grpref = (CT_AbstractItemGroup*) itIn_grpref.next();
        
        const CT_AbstractSingularItemDrawable* itemIn_refpos = (CT_AbstractSingularItemDrawable*)grpIn_grpref->firstItemByINModelName(this, DEFin_refpos);
        if (itemIn_refpos != NULL)
        {            
            float xRef = itemIn_refpos->firstItemAttributeByINModelName(this, DEFin_refx)->toFloat(itemIn_refpos, NULL);
            float yRef = itemIn_refpos->firstItemAttributeByINModelName(this, DEFin_refy)->toFloat(itemIn_refpos, NULL);
            float valRef = itemIn_refpos->firstItemAttributeByINModelName(this, DEFin_refvalue)->toFloat(itemIn_refpos, NULL);
            QString idRef = itemIn_refpos->firstItemAttributeByINModelName(this, DEFin_refid)->toString(itemIn_refpos, NULL);

            if (valRef < minRefValue) {minRefValue = valRef;}
            if (valRef > maxRefValue) {maxRefValue = valRef;}

            refPositions.append(QPair<Eigen::Vector2f, float>(Eigen::Vector2f(xRef, yRef), valRef));
            refIds.insert(cptRef, idRef);
        }
        cptRef++;
    }
    
    // create transPositions List
    CT_ResultGroupIterator itIn_grptrans(resIn_transpos, this, DEFin_grptrans);
    int cptTrans = 0;
    while (itIn_grptrans.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn_grptrans = (CT_AbstractItemGroup*) itIn_grptrans.next();
        
        const CT_AbstractSingularItemDrawable* itemIn_transpos = (CT_AbstractSingularItemDrawable*)grpIn_grptrans->firstItemByINModelName(this, DEFin_transpos);
        if (itemIn_transpos != NULL)
        {
            float xTrans = itemIn_transpos->firstItemAttributeByINModelName(this, DEFin_transx)->toFloat(itemIn_transpos, NULL);
            float yTrans = itemIn_transpos->firstItemAttributeByINModelName(this, DEFin_transy)->toFloat(itemIn_transpos, NULL);
            float valTrans = itemIn_transpos->firstItemAttributeByINModelName(this, DEFin_transvalue)->toFloat(itemIn_transpos, NULL);
            QString idTrans = itemIn_transpos->firstItemAttributeByINModelName(this, DEFin_transid)->toString(itemIn_transpos, NULL);

            if (valTrans < minTransValue) {minTransValue = valTrans;}
            if (valTrans > maxTransValue) {maxTransValue = valTrans;}

            transPositions.append(QPair<Eigen::Vector2f, float>(Eigen::Vector2f(xTrans, yTrans), valTrans));
            transPositionsTmp.append(QPair<Eigen::Vector2f, float>(Eigen::Vector2f(xTrans, yTrans), valTrans));
            transIds.insert(cptTrans, idTrans);
        }
        cptTrans++;
    }
    
    // Convert values to relatives values
    float deltaRef = maxRefValue - minRefValue;
    for (int i = 0 ; i < refPositions.size() ; i++)
    {
        refPositions[i].second = (refPositions.at(i).second - minRefValue) / deltaRef;
    }

    float deltaTrans = maxTransValue - minTransValue;
    for (int i = 0 ; i < transPositions.size() ; i++)
    {
        transPositions[i].second = (transPositions.at(i).second - minTransValue) / deltaTrans;
        transPositionsTmp[i].second = (transPositionsTmp.at(i).second - minTransValue) / deltaTrans;
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
            float transVal = transPositions.at(transCounter).second;

            if (transVal >= _relativeSizeThreshold)
            {
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

                                    // Compute translation for trans points
                                    Eigen::Vector2f delta = refPos - transPos;

                                    // Compute rotation for trans points
                                    double cosTheta = vectTrans.dot(vectRef)/(vectTrans.norm()*vectRef.norm());
                                    double theta = acos(cosTheta);

                                    double orientation = vectRef.x()*vectTrans.y() - vectRef.y()*vectTrans.x();

                                    if (orientation < 0) {theta = 3.0*M_PI/2.0 + (M_PI/2.0 - theta);}

                                    Eigen::Matrix2f rotation;
                                    rotation << cos(theta), -sin(theta),
                                                sin(theta),  cos(theta);

                                    // Apply translation and rotation to all trans points
                                    for (int k = 0 ; k < transPositionsTmp.size() ; k++)
                                    {
                                        transPositionsTmp[k].first = transPositions[k].first + delta;
                                        transPositionsTmp[k].first = rotation*(transPositionsTmp[k].first - refPos) + refPos;
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

                                        for (int tr = 0 ; tr < transPositionsTmp.size() && !trFound; tr++)
                                        {
                                            const Eigen::Vector2f &trPos = transPositionsTmp.at(tr).first;
                                            float dist = (trPos - rfPos).norm();

                                            if (dist < _distThreshold)
                                            {
                                                Nbref_with_Corresp++;
                                                trFound = true;
                                            }
                                        }
                                    }

                                    for (int tr = 0 ; tr < transPositionsTmp.size(); tr++)
                                    {
                                        const Eigen::Vector2f &trPos = transPositionsTmp.at(tr).first;
                                        float trVal = transPositionsTmp.at(tr).second;
                                        bool rfSimilar = false;

                                        for (int rf = 0 ; rf < refPositions.size() && !rfSimilar; rf++)
                                        {
                                            const Eigen::Vector2f &rfPos = refPositions.at(rf).first;
                                            float rfVal = refPositions.at(rf).second;
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
                                    float globalScore = _coef_nbRwc * ((float)Nbref_with_Corresp    / (float)refPositions.size())   +
                                                        _coef_nbTwc * ((float)Nbtrans_with_Corresp  / (float)transPositionsTmp.size()) +
                                                        _coef_nbSim * ((float)NbtransRef_Similarity / (float)transPositionsTmp.size()) ;

                                    if (globalScore > bestScore)
                                    {
                                        bestScore = globalScore;
                                        translationVector = delta;
                                        rotationMatrix = rotation;
                                        rotationCenter = refPos;
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
    Eigen::Matrix3f transformationMatrix = computeTransfMatrix2D(rotationCenter, translationVector, rotationMatrix);

    // Apply selected transformation to trans data
    for (int i = 0 ; i < transPositions.size() ; i++)
    {
                Eigen::Vector3f tmp;
                tmp[0] = transPositions[i].first[0];
                tmp[1] = transPositions[i].first[1];
                tmp[2] = 1;
                tmp = transformationMatrix*tmp;

                transPositions[i].first[0] = tmp[0];
                transPositions[i].first[1] = tmp[1];
//        transPositions[i].first = transPositions[i].first + translationVector;
//        transPositions[i].first = rotationMatrix*(transPositions[i].first - rotationCenter) + rotationCenter;
    }

    // Export transmorfed coordinates
    for (int i = 0 ; i < transPositions.size() ; i++)
    {
        // OUT results creation (move it to the appropried place in the code)
        CT_StandardItemGroup* grp_grp= new CT_StandardItemGroup(DEFout_grp, res_trans);
        res_trans->addGroup(grp_grp);

        CT_ReferencePoint* item_transpos = new CT_ReferencePoint(DEFout_transpos, res_trans, transPositions[i].first[0], transPositions[i].first[1], 0, 0);
        grp_grp->addItemDrawable(item_transpos);
    }

    // Find matching points pairs
    // first: compute all ref/trans distances
    QMultiMap<float, QPair<int, int> > distances;
    for (int refCounter = 0 ; refCounter < refPositions.size() ; refCounter++)
    {
        const Eigen::Vector2f &refPos = refPositions.at(refCounter).first;

        for (int transCounter = 0 ; transCounter < transPositions.size() ; transCounter++)
        {
            const Eigen::Vector2f &transPos = transPositions.at(transCounter).first;

             Eigen::Vector2f delta = (transPos - refPos);

             if (delta.norm() < _distThreshold)
             {
                 distances.insert(delta.norm(), QPair<int, int>(refCounter, transCounter));
             }
        }
    }

    // second: keep only one correspondance for each ref/trans
    QList<QPair<int, int> > candidates = distances.values();
    QMap<int, int> correspondances;
    while (!candidates.isEmpty())
    {
        QPair<int, int> pair = candidates.takeFirst();       
        correspondances.insert(pair.first, pair.second);

        for (int i = candidates.size() - 1 ; i >= 0 ; i--)
        {
            const QPair<int, int> &pair2 = candidates.at(i);
            if (pair.first == pair2.first || pair.second == pair2.second)
            {
                candidates.removeAt(i);
            }
        }
    }

    int nbMatches = correspondances.size();

    Eigen::MatrixXf refMat(nbMatches, 3);
    Eigen::MatrixXf transMat(nbMatches, 3);

    // put coordinates in matrices
    QMapIterator<int, int> it(correspondances);
    int cpt = 0;
    while (it.hasNext())
    {
        it.next();

        const Eigen::Vector2f &refPos = refPositions.at(it.key()).first;
        const Eigen::Vector2f &transPos = transPositions.at(it.value()).first;

        refMat(cpt, 0) = refPos(0);
        refMat(cpt, 1) = refPos(1);
        refMat(cpt, 2) = 0;

        transMat(cpt, 0) = transPos(0);
        transMat(cpt, 1) = transPos(1);
        transMat(cpt, 2) = 0;
        ++cpt;
    }

    Eigen::Vector3f center3D;
    Eigen::Vector3f translation3D;

    Eigen::Matrix3f rotationMatrix3D = Kabsch(refMat, transMat, nbMatches, center3D, translation3D);
    Eigen::Matrix2f rotationMatrix2D = rotationMatrix3D.block<2,2>(0,0);

    Eigen::Vector2f rotationCenter2D;
    Eigen::Vector2f translationVector2D;
    rotationCenter2D[0] = center3D[0];
    rotationCenter2D[1] = center3D[1];
    translationVector2D[0] = translation3D[0];
    translationVector2D[1] = translation3D[1];

    Eigen::Matrix3f transformationMatrix2 = computeTransfMatrix2D(rotationCenter2D, translationVector2D, rotationMatrix2D);

    // Apply selected transformation to trans data
    for (int i = 0 ; i < transPositions.size() ; i++)
    {
                Eigen::Vector3f tmp;
                tmp[0] = transPositions[i].first[0];
                tmp[1] = transPositions[i].first[1];
                tmp[2] = 1;
                tmp = transformationMatrix2*tmp;

                transPositions[i].first[0] = tmp[0];
                transPositions[i].first[1] = tmp[1];
    }

    // Quality criteria
    float rmseDist = 0;
    float rmseVal = 0;
    float maxDist = 0;
    float maxVal = 0;

    // Export transformed coordinates
    for (int i = 0 ; i < transPositions.size() ; i++)
    {
        const Eigen::Vector2f &transPos = transPositions.at(i).first;
        float transVal = transPositions.at(i).second;
        QString transId = transIds.value(i, "");

        // OUT results creation (move it to the appropried place in the code)
        CT_StandardItemGroup* grp_grp2= new CT_StandardItemGroup(DEFout_grp2, res_trans2);
        rootGroup->addGroup(grp_grp2);

        CT_ReferencePoint* item_transpos2;
        float zValTrans = transVal;
        float transValAbs = transVal*deltaTrans + minTransValue;
        float delta = _maxval - _minval;

        if (_relativeMode == 0) // absolute value
        {
            zValTrans = transValAbs;
        } else {
            zValTrans = zValTrans*delta + _minval;
        }

        if (_drawMode == 0)
        {
            item_transpos2 = new CT_ReferencePoint(DEFout_transpos2, res_trans2, transPos[0], transPos[1], zValTrans, 0);
        } else {
            item_transpos2 = new CT_ReferencePoint(DEFout_transpos2, res_trans2, transPos[0], transPos[1], 0, zValTrans);
        }
        grp_grp2->addItemDrawable(item_transpos2);

        item_transpos2->addItemAttribute(new CT_StdItemAttributeT<QString>(DEFout_transId,CT_AbstractCategory::DATA_ID,res_trans2, transId));


        int refIndice = correspondances.key(i, -1);
        if (refIndice >= 0)
        {
            const Eigen::Vector2f &refPos = refPositions.at(refIndice).first;
            float refVal = refPositions.at(refIndice).second;
            QString refId = refIds.value(refIndice);
            float distance = (refPos - transPos).norm();

            CT_ReferencePoint* item_refposCorresp;
            float zValRef = refVal;
            float refValAbs = refVal*deltaRef + minRefValue;

            float deltaVal = transValAbs - refValAbs;

            if (_relativeMode == 0) // absolute value
            {
                zValRef = refValAbs;
            } else {
                zValRef = zValRef*delta + _minval;
            }

            if (_drawMode == 0)
            {
                item_refposCorresp = new CT_ReferencePoint(DEFout_transposCorresp, res_trans2, refPos[0], refPos[1], zValRef, 0);
            } else {
                item_refposCorresp = new CT_ReferencePoint(DEFout_transposCorresp, res_trans2, refPos[0], refPos[1], 0, zValRef);
            }
            grp_grp2->addItemDrawable(item_refposCorresp);

            item_transpos2->addItemAttribute(new CT_StdItemAttributeT<QString>(DEFout_refCorrespId,CT_AbstractCategory::DATA_ID,res_trans2, refId));
            item_transpos2->addItemAttribute(new CT_StdItemAttributeT<float>(DEFout_deltaValue,CT_AbstractCategory::DATA_NUMBER,res_trans2, deltaVal));
            item_transpos2->addItemAttribute(new CT_StdItemAttributeT<float>(DEFout_distance,CT_AbstractCategory::DATA_NUMBER,res_trans2, distance));

            deltaDistMap.insert(refIndice, distance);
            deltaValMap.insert(refIndice, deltaVal);

            rmseDist += distance*distance;
            rmseVal += deltaVal*deltaVal;

            if (distance > maxDist) {maxDist = distance;}
            if (fabs(deltaVal) > maxVal) {maxVal = fabs(deltaVal);}

            CT_Line *line;
            if (_drawMode == 0)
            {
                line = new CT_Line(DEFout_correspLine, res_trans2, new CT_LineData(QVector3D(transPos[0], transPos[1], zValTrans), QVector3D(refPos[0], refPos[1], zValRef)));
            } else {
                line = new CT_Line(DEFout_correspLine, res_trans2, new CT_LineData(QVector3D(transPos[0], transPos[1], 0), QVector3D(refPos[0], refPos[1], 0)));
            }
            grp_grp2->addItemDrawable(line);

        }
    }

    rmseDist = sqrt(rmseDist/(float)nbMatches);
    rmseVal  = sqrt(rmseVal/(float)nbMatches);

    CT_AttributesList *attributes = new CT_AttributesList(DEFout_attributes, res_trans2);
    rootGroup->addItemDrawable(attributes);

    attributes->addItemAttribute(new CT_StdItemAttributeT<float>(DEFout_attRmseDist,CT_AbstractCategory::DATA_NUMBER,res_trans2, rmseDist));
    attributes->addItemAttribute(new CT_StdItemAttributeT<float>(DEFout_attRmseVal,CT_AbstractCategory::DATA_NUMBER,res_trans2, rmseVal));
    attributes->addItemAttribute(new CT_StdItemAttributeT<float>(DEFout_attMaxDist,CT_AbstractCategory::DATA_NUMBER,res_trans2, maxDist));
    attributes->addItemAttribute(new CT_StdItemAttributeT<float>(DEFout_attMaxDistDiff,CT_AbstractCategory::DATA_NUMBER,res_trans2, maxVal));

    Eigen::Matrix3f resultingMatrix = transformationMatrix*transformationMatrix2;

    QFile f(_reportFileName.first());
    if (f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&f);

        stream << "# Computree Matching report (generated by PluginBase/PB_StepMatchItemsPositions)\n";
        stream << "\n";
        stream << "# Transformation Matrix:\n";
        stream << QString::number(resultingMatrix(0,0), 'f', 10) << "\t" << QString::number(resultingMatrix(0,1), 'f', 10) << "\t" << QString::number(resultingMatrix(0,2), 'f', 10) << "\n";
        stream << QString::number(resultingMatrix(1,0), 'f', 10) << "\t" << QString::number(resultingMatrix(1,1), 'f', 10) << "\t" << QString::number(resultingMatrix(1,2), 'f', 10) << "\n";
        stream << QString::number(resultingMatrix(2,0), 'f', 10) << "\t" << QString::number(resultingMatrix(2,1), 'f', 10) << "\t" << QString::number(resultingMatrix(2,2), 'f', 10) << "\n";
        stream << "\n";
        stream << "# Matching quality criteria:\n";
        stream << "Number of reference positions                   :" << "\t" << refPositions.size() << "\n";
        stream << "Number of transformed positions                 :" << "\t" << transPositions.size() << "\n";
        stream << "Number of matching positions                    :" << "\t" << nbMatches << "\n";
        stream << "RMSE of matching distances (m)                  :" << "\t" << rmseDist << "\n";
        stream << "RMSE of differences between matching size values:" << "\t" << rmseVal << "\n";
        stream << "Maximum matching distance (m)                   :" << "\t" << maxDist << "\n";
        stream << "Maximum difference between matching size values :" << "\t" << maxVal << "\n";
        stream << "\n";
        stream << "# Matching algorithm parameters:\n";
        stream << "Maximum distance between matching positions (m)                           :" << "\t" << _distThreshold << "\n";
        stream << "Maximum allowed relative difference between size values to accept matching:" << "\t" << _relativeSizeThreshold << "\n";
        stream << "Minimum size value to consider transformed position in matching           :" << "\t" << _minRelativeSize << "\n";
        stream << "Score weight for reference positions positive matching                    :" << "\t" << _coef_nbRwc << "\n";
        stream << "Score weight for transformed positions positive matching                  :" << "\t" << _coef_nbTwc << "\n";
        stream << "Score weight for size similarity                                          :" << "\t" << _coef_nbSim << "\n";
        stream << "\n";
        stream << "# Positions data:\n";
        stream << "IDref\tXref\tYref\tValref\tIDtrans\tXtrans\tYtrans\tValtrans\tDeltaDist\tDeltaVal\n";



        for (int refCounter = 0 ; refCounter < refPositions.size() ; refCounter++)
        {
            const Eigen::Vector2f &refPos = refPositions.at(refCounter).first;
            float refVal = refPositions.at(refCounter).second;
            float refValAbs = refVal*deltaRef + minRefValue;

            double xr, yr, zr;
            PS_COORDINATES_SYS->convertExport(refPos[0], refPos[1], 0, xr, yr, zr);

            stream << refIds.value(refCounter, "");
            stream << "\t" << QString::number(xr, 'f', 4);
            stream << "\t" << QString::number(yr, 'f', 4);
            stream << "\t" << refValAbs;

            int transIndice = correspondances.value(refCounter, -1);
            if (transIndice >= 0)
            {
                const Eigen::Vector2f &transPos = transPositions.at(transIndice).first;
                float transVal = transPositions.at(transIndice).second;
                float transValAbs = transVal*deltaTrans + minTransValue;

                double xt, yt, zt;
                PS_COORDINATES_SYS->convertExport(transPos[0], transPos[1], 0, xt, yt, zt);

                stream << "\t" << transIds.value(transIndice, "");
                stream << "\t" << QString::number(xt, 'f', 4);
                stream << "\t" << QString::number(yt, 'f', 4);
                stream << "\t" << transValAbs;
                stream << "\t" << deltaDistMap.value(refCounter);
                stream << "\t" << deltaValMap.value(refCounter);

            } else {
                stream << "\t\t\t\t\t\t";
            }

            stream << "\n";
        }

        for (int transCounter = 0 ; transCounter < transPositions.size() ; transCounter++)
        {
            const Eigen::Vector2f &transPos = transPositions.at(transCounter).first;
            float transVal = transPositions.at(transCounter).second;
            float transValAbs = transVal*deltaTrans + minTransValue;

            double xt, yt, zt;
            PS_COORDINATES_SYS->convertExport(transPos[0], transPos[1], 0, xt, yt, zt);

            if (correspondances.key(transCounter, -1) < 0)
            {
                stream << "\t\t\t";
                stream << "\t" << transIds.value(transCounter, "");
                stream << "\t" << QString::number(xt, 'f', 4);
                stream << "\t" << QString::number(yt, 'f', 4);
                stream << "\t" << transValAbs;
                stream << "\t\t\n";
            }
        }

        stream << "\n";
        f.close();
    }

}

// Algorithm from https://github.com/MichalNowicki/Scientific/tree/master/TrackSLAM/Matching
Eigen::Matrix3f PB_StepMatchItemsPositions::Kabsch(Eigen::MatrixXf &refPositions, Eigen::MatrixXf &transPositions, int pointsNumber, Eigen::Vector3f &center, Eigen::Vector3f &translation)
{        
    float means[6];
    // Shift to center
    for(int j=0;j<6;j++) means[j] = 0.0f;

    for(int j=0;j<pointsNumber;j++)
    {
        for(int h=0;h<3;h++) means[h] += transPositions(j, h);
        for(int h=0;h<3;h++) means[h+3] += refPositions(j, h);
    }
    for(int j=0;j<6;j++) means[j] /= pointsNumber;

    // After counting, we must subtract the arithmetic
    for(int j=0;j<pointsNumber;j++)
    {
        for(int h=0;h<3;h++) transPositions(j, h) -= means[h];
        for(int h=0;h<3;h++) refPositions(j, h) -= means[h+3];
    }

    // counting transpose P * Q
    Eigen::MatrixXf A = transPositions.transpose() * refPositions;

    // SVD
    Eigen::JacobiSVD<Eigen::MatrixXf> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::MatrixXf V = svd.matrixU();
    Eigen::MatrixXf W = svd.matrixV();

    // Do a right-handed
    Eigen::Matrix3f U = Eigen::Matrix3f::Identity(3,3);
    U(2,2) = sgn(A.determinant());

    // Rotation matrix is:
    U = W * U * V.transpose();

    for(int h=0;h<3;h++) center[h] = means[h+3];
    for(int h=0;h<3;h++) translation[h] = means[h+3] - means[h];

    return U;
}

Eigen::Matrix3f PB_StepMatchItemsPositions::computeTransfMatrix2D(const Eigen::Vector2f &center, const Eigen::Vector2f &tranlation, const Eigen::Matrix2f &rotMat)
{
    Eigen::Vector2f T;
    T[0] = 0;
    T[1] = 0;

    T -= center;
    T += tranlation;
    T = rotMat*T;

    T += center;

    Eigen::Matrix3f transfMat =  Eigen::Matrix3f::Identity(3,3);

    transfMat(0,0) = rotMat(0,0);
    transfMat(0,1) = rotMat(0,1);
    transfMat(1,0) = rotMat(1,0);
    transfMat(1,1) = rotMat(1,1);
    transfMat(0,2) = T[0];
    transfMat(1,2) = T[1];

    return transfMat;
}
