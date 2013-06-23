// Balises C++, évitant les doublons d'includes
#ifndef TUFR_STEPTUTORIAL04_H
#define TUFR_STEPTUTORIAL04_H

// Inclusion de la classe parente des étapes
#include "ct_step/abstract/ct_abstractstep.h"

// Inclusion du système d'indexation automatique
#include "ct_tools/model/ct_autorenamemodels.h"

/*! \class TUFR_StepTutorial04
 *
 * \brief Cette classe créée un CT_ReferencePoint pour chaque CT_PointCluster
 *
 * Modèle IN : Aucun
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_PointCluster \n
 *
 * Modèle OUT : \n
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - cpy  CT_PointCluster \n
 *          - cpy+ CT_ReferencePoint \n
 *
 */

 // Une étape doit hériter de CT_AbstractStep
class TUFR_StepTutorial04 : public CT_AbstractStep
{
    // Macro Qt, donnant accès à certaines fonctionnalités
    // En l'occurence, permet de récupérer le nom de l'étape
    Q_OBJECT

public:

    // Constructeur de l'étape
    TUFR_StepTutorial04(CT_StepInitializeData &dataInit);

    // Méthode fournissant la description de l'étape (tooltip dans le menu contectuel)
    QString getStepDescription() const;

    // Méthode de copie de l'étape, utilisée lors de l'ajout d'une étapes à partir du menu contextuel
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:

    // Spécifications des résultats d'entrée souhaités (IN)
    void createInResultModelListProtected();

    // Boite de saisie des paramètres de l'étape
    void createPostConfigurationDialog();

    // Spécification des résultats de sortie créés par l'étape (OUT)
    void createOutResultModelListProtected();

    // Etape de calcul, créant les données des résultats de sortie
    void compute();

private:

    // Attributs permettant l'indexation des cercles qu'on va ajouter
    CT_AutoRenameModels     _outRefPointModelName;

};


// Fin de balise
#endif // TUFR_STEPTUTORIAL04_H
