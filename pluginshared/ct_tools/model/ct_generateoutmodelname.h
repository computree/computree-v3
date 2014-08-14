#ifndef CT_GENERATEOUTMODELNAME_H
#define CT_GENERATEOUTMODELNAME_H

#include "pluginShared_global.h"

#include <QList>
#include <QString>

class CT_InTurnManager;
class CT_InTurn;

class PLUGINSHAREDSHARED_EXPORT CT_GenerateOutModelName
{
public:
    CT_GenerateOutModelName();

    /*!
     *  \brief Retourne un nouveau nom pour un modèle de sortie
     *
     *  Vérifie si le nom généré n'existe dans aucun des modèles d'entrés de tous les tours, ni dans la liste des noms déjà générés
     *  passé en paramètre.
     *
     *  \param turnManager : le gestionnaire des tours d'entrée
     *  \param modelsNameAlreadyGenerated : une liste des noms déjà généré
     *  \param myFavoriteName : le nom qui sera généré sera peut être celui-ci si il n'existe pas dans la structure et dans la liste
     */
    QString getNewModelNameThatDontExistIn(CT_InTurnManager &turnManager,
                                           const QList<QString> &modelsNameAlreadyGenerated = QList<QString>(),
                                           QString myFavoriteName = "") const;

private:

    bool existInTurn(CT_InTurn *turn, const QString &modelName) const;
    QString generateNextName(int &index, char &letter, QString &letters) const;
};

#endif // CT_GENERATEOUTMODELNAME_H
