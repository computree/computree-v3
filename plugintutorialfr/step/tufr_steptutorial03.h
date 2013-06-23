// Balises C++, évitant les doublons d'includes
#ifndef TUFR_STEPTUTORIAL03_H
#define TUFR_STEPTUTORIAL03_H


// Inclusion de la classe parente des étapes
#include "ct_step/abstract/ct_abstractstep.h"

/*! \class TUFR_StepTutorial03
 * \ingroup Steps_TUFR
 *
 * \brief Cette classe découpe une scène de points en groupes paralellepipediques
 *
 * \param _nx Nombre de clusters selon x
 * \param _ny Nombre de clusters selon y
 * \param _nz Nombre de clusters selon z

 *
 * Modèle IN : Aucun
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_Scene \n
 *
 * Modèle OUT : \n
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_PointCluster \n
 *
 */

// Une étape doit hériter de CT_AbstractStep
class TUFR_StepTutorial03 : public CT_AbstractStep
{
    // Macro Qt, donnant accès à certaines fonctionnalités
    // En l'occurence, permet de récupérer le nom de l'étape
    Q_OBJECT

public:

    // Constructeur de l'étape
    TUFR_StepTutorial03(CT_StepInitializeData &dataInit);

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

    // Dans la zone privée : les paramètres de l'étapes (initilisés dans le constructeur)
    int _nx;      /*!< Nombre de clusters selon x */
    int _ny;      /*!< Nombre de clusters selon y */
    int _nz;      /*!< Nombre de clusters selon z */

};


// Fin de balise
#endif // TUFR_STEPTUTORIAL03_H
