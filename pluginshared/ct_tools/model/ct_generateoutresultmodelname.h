#ifndef CT_GENERATEOUTRESULTMODELNAME_H
#define CT_GENERATEOUTRESULTMODELNAME_H

#include "pluginShared_global.h"

#include <QList>
#include <QString>

class CT_OutResultModelManager;
class CT_OutAbstractResultModel;

class PLUGINSHAREDSHARED_EXPORT CT_GenerateOutResultModelName
{
public:
    CT_GenerateOutResultModelName();

    /*!
     *  \brief Retourne un nouveau nom pour un RESULTAT modèle de sortie
     *
     *  Vérifie si le nom généré n'existe dans aucun des RESULTAT modèles de sorties déjà ajouté au gestionnaire des résultats modèles de sortie, ni dans la liste des noms déjà générés
     *  passé en paramètre.
     *
     *  \param outResultModelManager : le gestionnaire des résultats modèles de sortie
     *  \param modelsNameAlreadyGenerated : une liste des noms déjà généré
     *  \param myFavoriteName : le nom qui sera généré sera peut être celui-ci si il n'existe pas dans la structure et dans la liste
     */
    QString getNewResultModelNameThatDontExistIn(CT_OutResultModelManager &outResultModelManager,
                                                 const QList<QString> &resultModelsNameAlreadyGenerated = QList<QString>(),
                                                 QString myFavoriteName = "") const;

private:

    bool existInList(const QList<CT_OutAbstractResultModel*> &list, const QString &modelName) const;
    QString generateNextName(int &index, char &letter, QString &letters) const;
};

#endif // CT_GENERATEOUTRESULTMODELNAME_H
