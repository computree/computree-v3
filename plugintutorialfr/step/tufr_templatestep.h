#ifndef TUFR_TEMPLATESTEP_H
#define TUFR_TEMPLATESTEP_H

#include "ct_step/abstract/ct_abstractstep.h"

class TUFR_TemplateStep : public CT_AbstractStep
{
    Q_OBJECT

public:

    // Constructeur de l'étape
    TUFR_TemplateStep(CT_StepInitializeData &dataInit);

    // Méthode fournissant la description de l'étape
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

    // paramètres de l'étapes
    double _parametre;   /*!< paramètre */

};

#endif // TUFR_TEMPLATESTEP_H
