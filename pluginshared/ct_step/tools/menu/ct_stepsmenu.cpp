#include "ct_stepsmenu.h"

#include "ct_menulevel.h"

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
    case LO_Create : return QObject::tr("Créer");
    case LO_Filter : return QObject::tr("Filtrer");
    case LO_Extract : return QObject::tr("Extraire");
    case LO_GeometryTransform : return QObject::tr("Transformation géométrique");
    case LO_Analyze : return QObject::tr("Analyzer");
    case LO_Detect : return QObject::tr("Detecter");
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
    case LP_Rotate: return QObject::tr("Rotation");
    case LP_Translate: return QObject::tr("Translation");
    case LP_Scale: return QObject::tr("Mise à l'échelle");
    case LP_Colorize: return QObject::tr("Colorier");
    case LP_Triangulate: return QObject::tr("Trianguler");
    case LP_Smooth: return QObject::tr("Lisser");
    case LP_Deforme: return QObject::tr("Déformer");
    case LP_Repair: return QObject::tr("Réparer");
    case LP_Simplify: return QObject::tr("Simplifier");
    case LP_Texturize: return QObject::tr("Texturer");
    case LP_Beam: return QObject::tr("Rayon");
    case LP_Box: return QObject::tr("Boîte");
    case LP_Circle: return QObject::tr("Cercle");
    case LP_Cluster: return QObject::tr("Cluster");
    case LP_Cylinder: return QObject::tr("Cylindre");
    case LP_DataSource: return QObject::tr("Source de données");
    case LP_DataSourceGeo: return QObject::tr("Source de données géo");
    case LP_EdgeAttributes: return QObject::tr("Attributs d'arêtes");
    case LP_Ellipse: return QObject::tr("Ellipse");
    case LP_FaceAttributes: return QObject::tr("Attributs de faces");
    case LP_Grid: return QObject::tr("Grille");
    case LP_Image: return QObject::tr("Image");
    case LP_Line: return QObject::tr("Ligne");
    case LP_Mesh: return QObject::tr("Maillage");
    case LP_BSpline: return QObject::tr("B-Spline");
    case LP_Point: return QObject::tr("Point");
    case LP_PointCloud: return QObject::tr("Nuage de points");
    case LP_PointAttributes: return QObject::tr("Attributs de points");
    case LP_Polygon: return QObject::tr("Polygone");
    case LP_Polyline: return QObject::tr("Polyligne");
    case LP_Profile: return QObject::tr("Profile");
    case LP_Scanner: return QObject::tr("Scanner");
    case LP_Sphere: return QObject::tr("Sphère");
    case LP_Normal: return QObject::tr("Normale");
    case LP_ItemAttributes: return QObject::tr("Attributs d'items");
    }

    return QObject::tr("Inconnu (TODO)");
}
