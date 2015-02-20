#include "pb_stepmatchitemspositions.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_itemdrawable/ct_referencepoint.h"
#include "ct_itemdrawable/ct_line.h"
#include "ct_itemdrawable/ct_attributeslist.h"
#include "ct_itemdrawable/ct_transformationmatrix.h"
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
#include <limits>

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
#define DEFout_trMat "trMat"
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
    _maxTheta = 180;
    _possiblyInvertedDirection = false;
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
    CT_InResultModelGroup *resIn_refpos = createNewInResultModel(DEFin_Resrefpos, tr("Positions de référence"), "", true);
    resIn_refpos->setZeroOrMoreRootGroup();
    resIn_refpos->addGroupModel("", DEFin_grpref, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_refpos->addItemModel(DEFin_grpref, DEFin_refpos, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item de référence"));
    resIn_refpos->addItemAttributeModel(DEFin_refpos, DEFin_refx, QList<QString>() << CT_AbstractCategory::DATA_X, CT_AbstractCategory::DOUBLE, tr("Coordonnée X"));
    resIn_refpos->addItemAttributeModel(DEFin_refpos, DEFin_refy, QList<QString>() << CT_AbstractCategory::DATA_Y, CT_AbstractCategory::DOUBLE, tr("Coordonnée Y"));
    resIn_refpos->addItemAttributeModel(DEFin_refpos, DEFin_refid, QList<QString>() << CT_AbstractCategory::DATA_ID, CT_AbstractCategory::ANY, tr("ID"));
    resIn_refpos->addItemAttributeModel(DEFin_refpos, DEFin_refvalue, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::NUMBER, tr("Valeur"));

    CT_InResultModelGroup *resIn_transpos = createNewInResultModel(DEFin_Restranspos, tr("Positions à transformer"), "", true);
    resIn_transpos->setZeroOrMoreRootGroup();
    resIn_transpos->addGroupModel("", DEFin_grptrans, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_transpos->addItemModel(DEFin_grptrans, DEFin_transpos, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item à transformer"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transx, QList<QString>() << CT_AbstractCategory::DATA_X, CT_AbstractCategory::DOUBLE, tr("Coordonnée X"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transy, QList<QString>() << CT_AbstractCategory::DATA_Y, CT_AbstractCategory::DOUBLE, tr("Coordonnée Y"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transid, QList<QString>() << CT_AbstractCategory::DATA_ID, CT_AbstractCategory::ANY, tr("ID"));
    resIn_transpos->addItemAttributeModel(DEFin_transpos, DEFin_transvalue, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::NUMBER, tr("Valeur"));

}

// Creation and affiliation of OUT models
void PB_StepMatchItemsPositions::createOutResultModelListProtected()
{
    CT_OutResultModelGroup *res_trans2 = createNewOutResultModel(DEFout_trans2, tr("Positions transformées"));
    res_trans2->setRootGroup(DEFout_rootGrp, new CT_StandardItemGroup(), tr("Groupe racine"));
    res_trans2->addItemModel(DEFout_rootGrp, DEFout_trMat, new CT_TransformationMatrix(), tr("Matrice de transformation"));
    res_trans2->addItemModel(DEFout_rootGrp, DEFout_attributes, new CT_AttributesList(), tr("Qualité de Matching"));
    res_trans2->addItemAttributeModel(DEFout_attributes, DEFout_attRmseDist,
                                      new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_VALUE),
                                      tr("RMSE Dist"));
    res_trans2->addItemAttributeModel(DEFout_attributes, DEFout_attRmseVal,
                                      new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_VALUE),
                                      tr("RMSE Val"));
    res_trans2->addItemAttributeModel(DEFout_attributes, DEFout_attMaxDist,
                                      new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_VALUE),
                                      tr("Max Dist"));
    res_trans2->addItemAttributeModel(DEFout_attributes, DEFout_attMaxDistDiff,
                                      new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_VALUE),
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
                                      new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_VALUE),
                                      tr("Ecart ValTrans - ValRef"));
    res_trans2->addItemAttributeModel(DEFout_transpos2, DEFout_distance,
                                      new CT_StdItemAttributeT<double>(CT_AbstractCategory::DATA_VALUE),
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

    configDialog->addTitle(tr("<b>Critères d'affiliation des positions :</b>"));

    configDialog->addDouble(tr("Distance maximale entre points appariés :"), "m", 0, 100, 3, _distThreshold, 1);
    configDialog->addDouble(tr("Seuil de taille relative minimum entre items appariés :"), "", 0, 1, 2, _relativeSizeThreshold, 1);
    configDialog->addDouble(tr("Taille relative minimale :"), "", 0, 1, 2, _minRelativeSize, 1);
    configDialog->addDouble(tr("Rotation maximale autorisée :"), "°", 0, 180, 2, _maxTheta);
    configDialog->addBool(tr("Inversion de direction possible (+- 180°)"), "", "", _possiblyInvertedDirection);

    configDialog->addEmpty();
    configDialog->addTitle(tr("<b>Critères de qualité de matching :</b>"));

    configDialog->addDouble(tr("Poid du critère Nb. pos. de référence ayant une pos. transformée proche :"), "", 0, 1000, 2, _coef_nbRwc);
    configDialog->addDouble(tr("Poid du critère Nb. pos. transformées ayant une pos. de référence proche :"), "", 0, 1000, 2, _coef_nbTwc);
    configDialog->addDouble(tr("Poid du critère Nb. pos. transformées ayant une pos. de référence proche avec une taille similaire :"), "", 0, 1000, 2, _coef_nbSim);

    configDialog->addEmpty();
    configDialog->addTitle(tr("<b>Mode de représentation :</b>"));

    configDialog->addTitle(tr("Type de représentation :"));
    CT_ButtonGroup &bg_drawMode = configDialog->addButtonGroup(_drawMode);

    configDialog->addExcludeValue("", "", tr("Valeur Z"), bg_drawMode, 0);
    configDialog->addExcludeValue("", "", tr("Cercle"), bg_drawMode, 1);

    configDialog->addTitle(tr("Comment représenter en Z la variable de taille ?"));

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
    double maxThetaRad = M_PI*_maxTheta / 180.0;

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_refpos = inResultList.at(0);
    CT_ResultGroup* resIn_transpos = inResultList.at(1);

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_trans = outResultList.at(1);
    CT_ResultGroup* res_trans2 = outResultList.at(0);
    CT_StandardItemGroup *rootGroup = new CT_StandardItemGroup(DEFout_rootGrp,res_trans2);
    res_trans2->addGroup(rootGroup);

    QList<QPair<Eigen::Vector2d, double> > refPositions;
    QList<QPair<Eigen::Vector2d, double> > transPositions;
    QList<QPair<Eigen::Vector2d, double> > transPositionsTmp;

    QMap<int, QString> refIds;
    QMap<int, QString> transIds;
    QMap<int, double> deltaDistMap;
    QMap<int, double> deltaValMap;

    double minRefValue = std::numeric_limits<double>::max();
    double maxRefValue = -std::numeric_limits<double>::max();
    double minTransValue = std::numeric_limits<double>::max();
    double maxTransValue = -std::numeric_limits<double>::max();

    // create refPositions List
    int cptRef = 0;
    CT_ResultGroupIterator itIn_grpref(resIn_refpos, this, DEFin_grpref);
    while (itIn_grpref.hasNext() && !isStopped())
    {
        const CT_AbstractItemGroup* grpIn_grpref = (CT_AbstractItemGroup*) itIn_grpref.next();
        
        const CT_AbstractSingularItemDrawable* itemIn_refpos = (CT_AbstractSingularItemDrawable*)grpIn_grpref->firstItemByINModelName(this, DEFin_refpos);
        if (itemIn_refpos != NULL)
        {
            double xRef = itemIn_refpos->firstItemAttributeByINModelName(resIn_refpos, this, DEFin_refx)->toDouble(itemIn_refpos, NULL);
            double yRef = itemIn_refpos->firstItemAttributeByINModelName(resIn_refpos, this, DEFin_refy)->toDouble(itemIn_refpos, NULL);
            double valRef = itemIn_refpos->firstItemAttributeByINModelName(resIn_refpos, this, DEFin_refvalue)->toDouble(itemIn_refpos, NULL);
            QString idRef = itemIn_refpos->firstItemAttributeByINModelName(resIn_refpos, this, DEFin_refid)->toString(itemIn_refpos, NULL);

            if (valRef < minRefValue) {minRefValue = valRef;}
            if (valRef > maxRefValue) {maxRefValue = valRef;}

            refPositions.append(QPair<Eigen::Vector2d, double>(Eigen::Vector2d(xRef, yRef), valRef));
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
            double xTrans = itemIn_transpos->firstItemAttributeByINModelName(resIn_transpos, this, DEFin_transx)->toDouble(itemIn_transpos, NULL);
            double yTrans = itemIn_transpos->firstItemAttributeByINModelName(resIn_transpos, this, DEFin_transy)->toDouble(itemIn_transpos, NULL);
            double valTrans = itemIn_transpos->firstItemAttributeByINModelName(resIn_transpos, this, DEFin_transvalue)->toDouble(itemIn_transpos, NULL);
            QString idTrans = itemIn_transpos->firstItemAttributeByINModelName(resIn_transpos, this, DEFin_transid)->toString(itemIn_transpos, NULL);

            if (valTrans < minTransValue) {minTransValue = valTrans;}
            if (valTrans > maxTransValue) {maxTransValue = valTrans;}

            transPositions.append(QPair<Eigen::Vector2d, double>(Eigen::Vector2d(xTrans, yTrans), valTrans));
            transPositionsTmp.append(QPair<Eigen::Vector2d, double>(Eigen::Vector2d(xTrans, yTrans), valTrans));
            transIds.insert(cptTrans, idTrans);
        }
        cptTrans++;
    }
    
    Eigen::Vector2d minRef, maxRef;

    // Ref Bounding Box
    minRef(0) = std::numeric_limits<double>::max();
    minRef(1) = std::numeric_limits<double>::max();
    maxRef(0) = -std::numeric_limits<double>::max();
    maxRef(1) = -std::numeric_limits<double>::max();

    int refPositionSize = refPositions.size();
    int transPositionsSize = transPositions.size();


    // Convert values to relatives values
    double deltaRef = maxRefValue - minRefValue;
    for (int i = 0 ; i < refPositionSize ; i++)
    {
        refPositions[i].second = (refPositions.at(i).second - minRefValue) / deltaRef;

        if (refPositions[i].first(0) < minRef(0)) {minRef(0) = refPositions[i].first(0);}
        if (refPositions[i].first(1) < minRef(1)) {minRef(1) = refPositions[i].first(1);}
        if (refPositions[i].first(0) > maxRef(0)) {maxRef(0) = refPositions[i].first(0);}
        if (refPositions[i].first(1) > maxRef(1)) {maxRef(1) = refPositions[i].first(1);}
    }

    minRef(0) -= _distThreshold;
    minRef(1) -= _distThreshold;
    maxRef(0) += _distThreshold;
    maxRef(1) += _distThreshold;

    double deltaTrans = maxTransValue - minTransValue;
    for (int i = 0 ; i < transPositionsSize ; i++)
    {
        transPositions[i].second = (transPositions.at(i).second - minTransValue) / deltaTrans;
        transPositionsTmp[i].second = (transPositionsTmp.at(i).second - minTransValue) / deltaTrans;
    }


    double bestScore = 0;

    Eigen::Vector2d rotationCenter;
    Eigen::Vector2d translationVector;
    Eigen::Vector2d delta;
    Eigen::Vector2d vectRef;
    Eigen::Vector2d vectTrans;
    Eigen::Vector2d minTrans, maxTrans;


    double cosTheta, theta, orientation, vectTransNorm, dist, trVal, rfVal, sizeDiff, globalScore;
    int Nbref_with_Corresp, Nbtrans_with_Corresp, NbtransRef_Similarity;

    Eigen::Matrix2d rotationMatrix;
    Eigen::Matrix2d rotation;


    QVector<bool> refPosCount(refPositionSize);
    QVector<bool> transPosCount(transPositionsSize);
    QVector<bool> similarPosCount(transPositionsSize);

    const double M_PIx2 = 2.0 * M_PI;

    // Points Matching algorithm
    for (int refCounter = 0 ; refCounter < refPositionSize && !isStopped(); refCounter++)
    {
        const Eigen::Vector2d &refPos = refPositions.at(refCounter).first;

        for (int transCounter = 0 ; transCounter < transPositionsSize && !isStopped(); transCounter++)
        {
            const Eigen::Vector2d &transPos = transPositions.at(transCounter).first;
            double transVal = transPositions.at(transCounter).second;

            // Compute translation for trans points
            delta = refPos - transPos;

            if (transVal >= _relativeSizeThreshold)
            {
                // Search for second trans point
                for (int i = 0 ; i < transPositionsSize && !isStopped() ; i++)
                {

                    if (i != transCounter)
                    {
                        const Eigen::Vector2d &transPos_i = transPositions.at(i).first;
                        vectTrans = transPos_i - transPos;
                        vectTransNorm = vectTrans.norm();

                        // Search for second ref point
                        for (int j = 0 ; j < refPositionSize && !isStopped() ; j++)
                        {

                            if (j != refCounter)
                            {
                                const Eigen::Vector2d &refPos_j = refPositions.at(j).first;

                                vectRef = refPos_j - refPos;

                                // If distances have the same value +- _distThreshold
                                if (fabs(vectRef.norm() - vectTransNorm) < _distThreshold)
                                {

                                    // Compute rotation for trans points
                                    cosTheta = vectTrans.dot(vectRef)/(vectTrans.norm()*vectRef.norm());
                                    theta = acos(cosTheta);

                                    orientation = vectRef.x()*vectTrans.y() - vectRef.y()*vectTrans.x();


                                    if (orientation < 0) {theta = M_PIx2 - theta;}

                                    bool rotationPossible = true;
                                    if (maxThetaRad < M_PI)
                                    {
                                        rotationPossible = false;
                                        if ((theta <= maxThetaRad) || (theta >= (M_PIx2 - maxThetaRad))) {rotationPossible = true;}
                                        if (_possiblyInvertedDirection && ((theta >= (M_PI - maxThetaRad)) && (theta <= (M_PI + maxThetaRad)))) {rotationPossible = true;}
                                    }

                                    if (rotationPossible)
                                    {
                                        rotation << cos(theta), -sin(theta),
                                                sin(theta),  cos(theta);

                                        // Trans Bounding Box
                                        minTrans(0) = std::numeric_limits<double>::max();
                                        minTrans(1) = std::numeric_limits<double>::max();
                                        maxTrans(0) = -std::numeric_limits<double>::max();
                                        maxTrans(1) = -std::numeric_limits<double>::max();

                                        // Apply translation and rotation to all trans points
                                        for (int k = 0 ; k < transPositionsTmp.size() ; k++)
                                        {                                           
                                            transPositionsTmp[k].first = transPositions[k].first + delta;
                                            transPositionsTmp[k].first = rotation*(transPositionsTmp[k].first - refPos) + refPos;

                                            const QPair<Eigen::Vector2d, double> &pair = transPositionsTmp.at(k);

                                            if (pair.first(0) < minTrans(0)) {minTrans(0) = pair.first(0);}
                                            if (pair.first(1) < minTrans(1)) {minTrans(1) = pair.first(1);}
                                            if (pair.first(0) > maxTrans(0)) {maxTrans(0) = pair.first(0);}
                                            if (pair.first(1) > maxTrans(1)) {maxTrans(1) = pair.first(1);}
                                        }

                                        minTrans(0) -= _distThreshold;
                                        minTrans(1) -= _distThreshold;
                                        maxTrans(0) += _distThreshold;
                                        maxTrans(1) += _distThreshold;


                                        // Compute match scores

                                        // Number of ref points with a trans point within _distThreshold
                                        Nbref_with_Corresp = 0;
                                        // Number of trans points with a ref point within _distThreshold
                                        Nbtrans_with_Corresp = 0;
                                        // Number of trans points with a ref point within _distThreshold AND relatives sizes are similar (difference < _relativeSizeThreshold)
                                        NbtransRef_Similarity = 0;

                                        refPosCount.fill(false);
                                        transPosCount.fill(false);
                                        similarPosCount.fill(false);

                                        for (int tr = 0 ; tr < transPositionsSize; tr++)
                                        {
                                            const Eigen::Vector2d &trPos = transPositionsTmp.at(tr).first;
                                            trVal = transPositionsTmp.at(tr).second;

                                            if (trPos(0) > minRef(0) &&
                                                    trPos(1) > minRef(1) &&
                                                    trPos(0) < maxRef(0) &&
                                                    trPos(1) < maxRef(1))
                                            {

                                                for (int rf = 0 ; rf < refPositionSize; rf++)
                                                {
                                                    const Eigen::Vector2d &rfPos = refPositions.at(rf).first;

                                                    if (rfPos(0) > minTrans(0) &&
                                                            rfPos(1) > minTrans(1) &&
                                                            rfPos(0) < maxTrans(0) &&
                                                            rfPos(1) < maxTrans(1))
                                                    {
                                                        rfVal = refPositions.at(rf).second;
                                                        dist = (trPos - rfPos).norm();

                                                        if (dist < _distThreshold)
                                                        {
                                                            refPosCount[rf] = true;
                                                            transPosCount[tr] = true;

                                                            sizeDiff = fabs(rfVal - trVal);
                                                            if (sizeDiff < _relativeSizeThreshold)
                                                            {
                                                                similarPosCount[tr] = true;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        for (int rf = 0 ; rf < refPositionSize; rf++)
                                        {
                                            if (refPosCount[rf]) {Nbref_with_Corresp++;}
                                        }

                                        for (int tr = 0 ; tr < transPositionsSize; tr++)
                                        {
                                            if (transPosCount[tr]) {Nbtrans_with_Corresp++;}
                                            if (similarPosCount[tr]) {NbtransRef_Similarity++;}
                                        }


                                        // Compute global score
                                        globalScore =   _coef_nbRwc * ((double)Nbref_with_Corresp    / (double)refPositions.size())   +
                                                        _coef_nbTwc * ((double)Nbtrans_with_Corresp  / (double)transPositionsTmp.size()) +
                                                        _coef_nbSim * ((double)NbtransRef_Similarity / (double)transPositionsTmp.size()) ;

                                        if (globalScore > bestScore || bestScore == 0)
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
        setProgress(80.0 * (double)refCounter / (double)refPositions.size());
    }

    // Compute transformation matrix
    Eigen::Matrix3d transformationMatrix = computeTransfMatrix2D(rotationCenter, translationVector, rotationMatrix);

    // Apply selected transformation to trans data
    for (int i = 0 ; i < transPositions.size() ; i++)
    {
        Eigen::Vector3d tmp;
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
    QMultiMap<double, QPair<int, int> > distances;
    for (int refCounter = 0 ; refCounter < refPositions.size() ; refCounter++)
    {
        const Eigen::Vector2d &refPos = refPositions.at(refCounter).first;

        for (int transCounter = 0 ; transCounter < transPositions.size() ; transCounter++)
        {
            const Eigen::Vector2d &transPos = transPositions.at(transCounter).first;

            Eigen::Vector2d delta = (transPos - refPos);

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

    Eigen::MatrixXd refMat(nbMatches, 3);
    Eigen::MatrixXd transMat(nbMatches, 3);

    // put coordinates in matrices
    QMapIterator<int, int> it(correspondances);
    int cpt = 0;
    while (it.hasNext())
    {
        it.next();

        const Eigen::Vector2d &refPos = refPositions.at(it.key()).first;
        const Eigen::Vector2d &transPos = transPositions.at(it.value()).first;

        refMat(cpt, 0) = refPos(0);
        refMat(cpt, 1) = refPos(1);
        refMat(cpt, 2) = 0;

        transMat(cpt, 0) = transPos(0);
        transMat(cpt, 1) = transPos(1);
        transMat(cpt, 2) = 0;
        ++cpt;
    }

    Eigen::Vector3d center3D;
    Eigen::Vector3d translation3D;

    Eigen::Matrix3d rotationMatrix3D = Kabsch(refMat, transMat, nbMatches, center3D, translation3D);
    Eigen::Matrix2d rotationMatrix2D = rotationMatrix3D.block<2,2>(0,0);

    Eigen::Vector2d rotationCenter2D;
    Eigen::Vector2d translationVector2D;
    rotationCenter2D[0] = center3D[0];
    rotationCenter2D[1] = center3D[1];
    translationVector2D[0] = translation3D[0];
    translationVector2D[1] = translation3D[1];

    Eigen::Matrix3d transformationMatrix2 = computeTransfMatrix2D(rotationCenter2D, translationVector2D, rotationMatrix2D);

    // Apply selected transformation to trans data
    for (int i = 0 ; i < transPositions.size() ; i++)
    {
        Eigen::Vector3d tmp;
        tmp[0] = transPositions[i].first[0];
        tmp[1] = transPositions[i].first[1];
        tmp[2] = 1;
        tmp = transformationMatrix2*tmp;

        transPositions[i].first[0] = tmp[0];
        transPositions[i].first[1] = tmp[1];
    }

    // Quality criteria
    double rmseDist = 0;
    double rmseVal = 0;
    double maxDist = 0;
    double maxVal = 0;

    // Export transformed coordinates
    for (int i = 0 ; i < transPositions.size() ; i++)
    {
        const Eigen::Vector2d &transPos = transPositions.at(i).first;
        double transVal = transPositions.at(i).second;
        QString transId = transIds.value(i, "");

        // OUT results creation (move it to the appropried place in the code)
        CT_StandardItemGroup* grp_grp2= new CT_StandardItemGroup(DEFout_grp2, res_trans2);
        rootGroup->addGroup(grp_grp2);

        CT_ReferencePoint* item_transpos2;
        double zValTrans = transVal;
        double transValAbs = transVal*deltaTrans + minTransValue;
        double delta = _maxval - _minval;

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
            const Eigen::Vector2d &refPos = refPositions.at(refIndice).first;
            double refVal = refPositions.at(refIndice).second;
            QString refId = refIds.value(refIndice);
            double distance = (refPos - transPos).norm();

            CT_ReferencePoint* item_refposCorresp;
            double zValRef = refVal;
            double refValAbs = refVal*deltaRef + minRefValue;

            double deltaVal = transValAbs - refValAbs;

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
            item_transpos2->addItemAttribute(new CT_StdItemAttributeT<double>(DEFout_deltaValue,CT_AbstractCategory::DATA_VALUE,res_trans2, deltaVal));
            item_transpos2->addItemAttribute(new CT_StdItemAttributeT<double>(DEFout_distance,CT_AbstractCategory::DATA_VALUE,res_trans2, distance));

            deltaDistMap.insert(refIndice, distance);
            deltaValMap.insert(refIndice, deltaVal);

            rmseDist += distance*distance;
            rmseVal += deltaVal*deltaVal;

            if (distance > maxDist) {maxDist = distance;}
            if (fabs(deltaVal) > maxVal) {maxVal = fabs(deltaVal);}

            CT_Line *line;
            if (_drawMode == 0)
            {
                line = new CT_Line(DEFout_correspLine, res_trans2, new CT_LineData(Eigen::Vector3d(transPos[0], transPos[1], zValTrans), Eigen::Vector3d(refPos[0], refPos[1], zValRef)));
            } else {
                line = new CT_Line(DEFout_correspLine, res_trans2, new CT_LineData(Eigen::Vector3d(transPos[0], transPos[1], 0), Eigen::Vector3d(refPos[0], refPos[1], 0)));
            }
            grp_grp2->addItemDrawable(line);

        }
    }

    rmseDist = sqrt(rmseDist/(double)nbMatches);
    rmseVal  = sqrt(rmseVal/(double)nbMatches);

    CT_AttributesList *attributes = new CT_AttributesList(DEFout_attributes, res_trans2);
    rootGroup->addItemDrawable(attributes);

    attributes->addItemAttribute(new CT_StdItemAttributeT<double>(DEFout_attRmseDist,CT_AbstractCategory::DATA_VALUE,res_trans2, rmseDist));
    attributes->addItemAttribute(new CT_StdItemAttributeT<double>(DEFout_attRmseVal,CT_AbstractCategory::DATA_VALUE,res_trans2, rmseVal));
    attributes->addItemAttribute(new CT_StdItemAttributeT<double>(DEFout_attMaxDist,CT_AbstractCategory::DATA_VALUE,res_trans2, maxDist));
    attributes->addItemAttribute(new CT_StdItemAttributeT<double>(DEFout_attMaxDistDiff,CT_AbstractCategory::DATA_VALUE,res_trans2, maxVal));

    Eigen::Matrix3d resultingMatrix = transformationMatrix2*transformationMatrix;

    Eigen::Matrix4d transf3D = Eigen::Matrix4d::Identity(4,4);
    transf3D(0,0) = resultingMatrix(0,0);
    transf3D(0,1) = resultingMatrix(0,1);
    transf3D(1,0) = resultingMatrix(1,0);
    transf3D(1,1) = resultingMatrix(1,1);

    transf3D(0,3) = resultingMatrix(0,2);
    transf3D(1,3) = resultingMatrix(1,2);

    CT_TransformationMatrix *transfMatItem = new CT_TransformationMatrix(DEFout_trMat, res_trans2, transf3D);
    rootGroup->addItemDrawable(transfMatItem);

    QFile f(_reportFileName.first());
    if (f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&f);

        stream << "# Computree Matching report (generated by PluginBase/PB_StepMatchItemsPositions)\n";
        stream << "\n";
        stream << "# 2D Transformation Matrix:\n";
        stream << QString::number(resultingMatrix(0,0), 'f', 10) << "\t" << QString::number(resultingMatrix(0,1), 'f', 10) << "\t" << QString::number(resultingMatrix(0,2), 'f', 10) << "\n";
        stream << QString::number(resultingMatrix(1,0), 'f', 10) << "\t" << QString::number(resultingMatrix(1,1), 'f', 10) << "\t" << QString::number(resultingMatrix(1,2), 'f', 10) << "\n";
        stream << QString::number(resultingMatrix(2,0), 'f', 10) << "\t" << QString::number(resultingMatrix(2,1), 'f', 10) << "\t" << QString::number(resultingMatrix(2,2), 'f', 10) << "\n";
        stream << "\n";
        stream << "# 3D Transformation Matrix:\n";
        stream << transfMatItem->getTransformationMatrixAsString();
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
        stream << "\n";

        stream << "Algorithm from:\n";
        stream << "Marius Hauglin, Vegard Lien, Erik Naesset & Terje Gobakken (2014)\n";
        stream << "Geo-referencing forest field plots by co-registration of terrestrial and airborne laser scanning data,\n";
        stream << "International Journal of Remote Sensing, 35:9, 3135-3149, DOI: 10.1080/01431161.2014.903440\n";

        stream << "\n";
        stream << "\n";

        stream << "# Positions data:\n";
        stream << "IDref\tXref\tYref\tValref\tIDtrans\tXtrans\tYtrans\tValtrans\tDeltaDist\tDeltaVal\n";



        for (int refCounter = 0 ; refCounter < refPositions.size() ; refCounter++)
        {
            const Eigen::Vector2d &refPos = refPositions.at(refCounter).first;
            double refVal = refPositions.at(refCounter).second;
            double refValAbs = refVal*deltaRef + minRefValue;

            stream << refIds.value(refCounter, "");
            stream << "\t" << QString::number(refPos[0], 'f', 4);
            stream << "\t" << QString::number(refPos[1], 'f', 4);
            stream << "\t" << refValAbs;

            int transIndice = correspondances.value(refCounter, -1);
            if (transIndice >= 0)
            {
                const Eigen::Vector2d &transPos = transPositions.at(transIndice).first;
                double transVal = transPositions.at(transIndice).second;
                double transValAbs = transVal*deltaTrans + minTransValue;

                stream << "\t" << transIds.value(transIndice, "");
                stream << "\t" << QString::number(transPos[0], 'f', 4);
                stream << "\t" << QString::number(transPos[1], 'f', 4);
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
            const Eigen::Vector2d &transPos = transPositions.at(transCounter).first;
            double transVal = transPositions.at(transCounter).second;
            double transValAbs = transVal*deltaTrans + minTransValue;

            if (correspondances.key(transCounter, -1) < 0)
            {
                stream << "\t\t\t";
                stream << "\t" << transIds.value(transCounter, "");
                stream << "\t" << QString::number(transPos[0], 'f', 4);
                stream << "\t" << QString::number(transPos[1], 'f', 4);
                stream << "\t" << transValAbs;
                stream << "\t\t\n";
            }
        }

        stream << "\n";
        f.close();
    }

}

// Algorithm from https://github.com/MichalNowicki/Scientific/tree/master/TrackSLAM/Matching
Eigen::Matrix3d PB_StepMatchItemsPositions::Kabsch(Eigen::MatrixXd &refPositions, Eigen::MatrixXd &transPositions, int pointsNumber, Eigen::Vector3d &center, Eigen::Vector3d &translation)
{        
    double means[6];
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
    Eigen::MatrixXd A = transPositions.transpose() * refPositions;

    // SVD
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::MatrixXd V = svd.matrixU();
    Eigen::MatrixXd W = svd.matrixV();

    // Do a right-handed
    Eigen::Matrix3d U = Eigen::Matrix3d::Identity(3,3);
    U(2,2) = sgn(A.determinant());

    // Rotation matrix is:
    U = W * U * V.transpose();

    for(int h=0;h<3;h++) center[h] = means[h+3];
    for(int h=0;h<3;h++) translation[h] = means[h+3] - means[h];

    return U;
}

Eigen::Matrix3d PB_StepMatchItemsPositions::computeTransfMatrix2D(const Eigen::Vector2d &center, const Eigen::Vector2d &tranlation, const Eigen::Matrix2d &rotMat)
{
    Eigen::Vector2d T;
    T[0] = 0;
    T[1] = 0;

    T -= center;
    T += tranlation;
    T = rotMat*T;

    T += center;

    Eigen::Matrix3d transfMat =  Eigen::Matrix3d::Identity(3,3);

    transfMat(0,0) = rotMat(0,0);
    transfMat(0,1) = rotMat(0,1);
    transfMat(1,0) = rotMat(1,0);
    transfMat(1,1) = rotMat(1,1);
    transfMat(0,2) = T[0];
    transfMat(1,2) = T[1];

    return transfMat;
}
