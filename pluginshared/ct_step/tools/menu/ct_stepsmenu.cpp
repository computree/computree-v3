#include "ct_stepsmenu.h"

#include "ct_menulevel.h"

#include <QMetaEnum>

CT_StepsMenu::CT_StepsMenu()
{
}

CT_StepsMenu::~CT_StepsMenu()
{
    QMapIterator<LevelOperationType, CT_MenuLevel*> it(m_rootLevels);

    while(it.hasNext())
        delete it.next().value();
}

CT_MenuLevel *CT_StepsMenu::getOrCreateRootLevel(CT_StepsMenu::LevelOperationType operation)
{
    if(operation == LO_InternalUse)
        return NULL;

    CT_MenuLevel *level = levelFromOperation(operation);

    if(level == NULL) {
        level = new CT_MenuLevel(staticOperationToDisplayableName(operation));
        m_rootLevels.insert(operation, level);
    }

    return level;
}

CT_MenuLevel *CT_StepsMenu::levelFromOperation(CT_StepsMenu::LevelOperationType operation) const
{
    if(operation == LO_InternalUse)
        return NULL;

    return m_rootLevels.value(operation, NULL);;
}

QList<CT_MenuLevel *> CT_StepsMenu::levels() const
{
    return m_rootLevels.values();
}

bool CT_StepsMenu::removeLevelFromFavorites(CT_MenuLevel *level)
{
    CT_MenuLevel *favorites = levelFromOperation(LO_Favorites);

    if(favorites != NULL) {
        CT_MenuLevel *sLevel = level;

        while(sLevel->parentLevel() != NULL) {

            if(sLevel->parentLevel() == favorites) {
                delete level;
                return true;
            }

            sLevel = sLevel->parentLevel();
        }

        return false;
    }

    return false;
}

QString CT_StepsMenu::staticOperationToDisplayableName(CT_StepsMenu::LevelOperationType operation)
{
    switch(operation) {
    case LO_Load : return QObject::tr("Charger");
    case LO_Export : return QObject::tr("Exporter");
    case LO_Points : return QObject::tr("Points");
    case LO_GeometricalShapes : return QObject::tr("Formes géométriques");
    case LO_Voxels : return QObject::tr("Voxels");
    case LO_Rasters : return QObject::tr("Rasters");
    case LO_Meshes : return QObject::tr("Maillages");
    case LO_WorkFlow : return QObject::tr("Flux");
    case LO_Other : return QObject::tr("Autres");
    case LO_Favorites : return QObject::tr("Favoris");
    case LO_InternalUse : return "Internal must not be use by programmers";
    }

    return QObject::tr("Inconnu TODO !");;
}

QString CT_StepsMenu::staticPredefinedToString(CT_StepsMenu::LevelPredefined predefined)
{           
    switch(predefined) {
    case LP_Filter: return QObject::tr("Filtrer");
    case LP_Create: return QObject::tr("Créer");
    case LP_Extract: return QObject::tr("Extraire");
    case LP_Detect: return QObject::tr("Détecter");
    case LP_Transform: return QObject::tr("Transformer");
    case LP_Classify: return QObject::tr("Classifier");
    case LP_Analyze: return QObject::tr("Analyser");
    case LP_Fit: return QObject::tr("Ajuster");
    case LP_Colorize: return QObject::tr("Coloriser");
    case LP_Clusters: return QObject::tr("Clusters");
    case LP_Loops: return QObject::tr("Boucles");
    case LP_Crowns: return QObject::tr("Houppiers");
    case LP_DEM: return QObject::tr("Modèles Num. d'Elévation");
    case LP_Stems: return QObject::tr("Tiges");
    case LP_DBH: return QObject::tr("Diamètres à 1.30 m");
    case LP_ItemAttributes: return QObject::tr("Attributs d'items");
    }

    return QObject::tr("Inconnu (TODO)");
}

bool CT_StepsMenu::staticIsPredefinedLevelString(const QString &name)
{
    const QMetaObject &mo = CT_StepsMenu::staticMetaObject;

    int index = mo.indexOfEnumerator("LevelPredefined");

    if(index >= 0) {
        QMetaEnum metaEnum = mo.enumerator(index);

        int s = metaEnum.keyCount();

        for(int i=0; i<s; ++i) {
            if(name == staticPredefinedToString((CT_StepsMenu::LevelPredefined)metaEnum.value(i)))
                return true;
        }
    }

    return false;
}
