#ifndef CT_POLYLINESALGORITHMS_H
#define CT_POLYLINESALGORITHMS_H

#include "ct_itemdrawable/ct_pointcluster.h"
#include <QVector2D>

class PLUGINSHAREDSHARED_EXPORT CT_PolylinesAlgorithms
{
public:

    static void createPolyline2D(const CT_PointCluster *baseCluster, CT_PointCluster *polylineCluster);

    static QVector2D compute2DArcData(const CT_PointCluster *polyline, float &sagitta, float &chord, float &radius);


    /**
      * \struct CandidatePoint
      * \brief  Elments caractristiques d'un point candidat  l'ajout
      */
    struct CandidatePoint {
        size_t indexInCloud;		/*!< Indice du point dans le nuage de points global.*/
        size_t indexOfMinDistance;	/*!< Indice du point dans le nuage global (  l'interieur du mme groupe) qui minimise la distance   la ligne*/
        float distanceToLine;	/*!< Distance du point a la ligne*/
    };

};

#endif // CT_POLYLINESALGORITHMS_H
