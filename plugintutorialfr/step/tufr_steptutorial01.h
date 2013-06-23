// Balises C++, évitant les doublons d'includes
#ifndef TUFR_STEPTUTORIAL01_H
#define TUFR_STEPTUTORIAL01_H


// Inclusion de la classe parente des étapes
#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"

/*! \class TUFR_StepTutorial01
 *
 * \brief Cette classe ne fait rien
 *
 * Son but est de montrer la gestion des boites de dialogues de paramétrage
 *
 * \param _intParam Paramètre entier
 * \param _doubleParam Paramètre double
 * \param _boolParam Paramètre booléen
 * \param _listParam Paramètre liste
 * \param _choiceParam Paramètre de choix
 * \param _fileNameParam Paramètre de nom de fichier
 *
 * Modèle IN : Aucun
 *
 * Modèle OUT : \n
 *  - CT_ResultGroup \n
 *
 */
// Une étape doit hériter de CT_AbstractStep (ou d'une classe en héritant)
// Ici, l'étape ne prend pas de données en entrée, on hérite donc de CT_AbstractStepCanBeAddedFirst
class TUFR_StepTutorial01 : public CT_AbstractStepCanBeAddedFirst
{
    // Macro Qt, donnant accès à certaines fonctionnalités
    // En l'occurence, permet de récupérer le nom de l'étape
    Q_OBJECT

public:

    // Constructeur de l'étape
    TUFR_StepTutorial01(CT_StepInitializeData &dataInit);

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
    int     _intParam;       /*!< Paramètre entier */
    double  _doubleParam;    /*!< Paramètre double */
    bool    _boolParam;      /*!< Paramètre booléen */
    QString _listParam;      /*!< Paramètre liste */
    int     _choiceParam;    /*!< Paramètre de choix */
    QString _fileNameParam;  /*!< Paramètre de nom de fichier */

};

// Fin de balise
#endif // TUFR_STEPTUTORIAL01_H
