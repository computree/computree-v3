// Balises C++, évitant les doublons d'includes
#ifndef TUFR_STEPTUTORIAL02_H
#define TUFR_STEPTUTORIAL02_H


// Inclusion de la classe parente des étapes
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"


/*! \class TUFR_StepTutorial02
 * \ingroup Steps_TUFR
 *
 * \brief Cette classe génère une scène de points de positions aléatoires
 *
 * \param _n Nombre de points aléatoires à générer
 * \param _xmin X maximum de la scène à générer
 * \param _xmax X minimum de la scène à générer
 * \param _ymin Y maximum de la scène à générer
 * \param _ymax Y minimum de la scène à générer
 * \param _zmin Z minimum de la scène à générer
 * \param _zmax Z maximum de la scène à générer
 *
 * Modèle IN : Aucun
 *
 * Modèle OUT : \n
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_Scene \n
 *
 */
// Une étape doit hériter de CT_AbstractStep (ou d'une classe en héritant)
// Ici, l'étape ne prend pas de données en entrée, on hérite donc de CT_AbstractStepCanBeAddedFirst
class TUFR_StepTutorial02 : public CT_AbstractStepCanBeAddedFirst
{
    // Macro Qt, donnant accès à certaines fonctionnalités
    // En l'occurence, permet de récupérer le nom de l'étape
    Q_OBJECT

public:

    // Constructeur de l'étape
    TUFR_StepTutorial02(CT_StepInitializeData &dataInit);

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
    int     _n;      /*!< Nombre de points aléatoires à générer */
    double _xmin;   /*!< X maximum de la scène à générer */
    double _xmax;   /*!< X minimum de la scène à générer */
    double _ymin;   /*!< Y maximum de la scène à générer */
    double _ymax;   /*!< Y minimum de la scène à générer */
    double _zmin;   /*!< Z minimum de la scène à générer */
    double _zmax;   /*!< Z maximum de la scène à générer */

};

// Fin de balise
#endif // TUFR_STEPTUTORIAL02_H
