#ifndef CT_ABSTRACTUNDEFINEDSIZEPOINTCLOUD_H
#define CT_ABSTRACTUNDEFINEDSIZEPOINTCLOUD_H

#include <QObject>

#include "ct_point.h"

/**
 * A cloud of points used by the GlobalPointCloudManager when you want to create a cloud of points and you
 * don't know it's size by advance.
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractUndefinedSizePointCloud : public QObject
{
    Q_OBJECT

public:
    CT_AbstractUndefinedSizePointCloud();
    virtual ~CT_AbstractUndefinedSizePointCloud();

    /**
     * @return  l'index du premier point dans le nuage
     */
    virtual size_t beginIndex() const = 0;

    /**
     * @return la taille du nuage
     */
    virtual size_t size() const = 0;

    /**
     * @brief Ajoute un point au nuage. (Préféré la méthode addPoint qui renvoie une référence pour être plus rapide)
     * @param point : le point à ajouter
     */
    virtual void addPoint(const CT_Point &point) = 0;

    /**
     * @brief Ajoute un point au nuage et retoune une référence sur le point ajouté
     * @return une référence sur le point ajouté
     */
    virtual CT_Point& addPoint() = 0;

    /**
     * @brief Return a reference of the point at 'i'
     */
    virtual CT_Point& operator[](const size_t &i) = 0;

    /**
     * @brief Return a reference of the point at 'i'
     */
    virtual CT_Point& pointAt(const size_t &i) = 0;

    /**
     * @brief Return a const reference of the point at 'i'
     */
    virtual const CT_Point& operator[](const size_t &i) const = 0;

    /**
     * @brief Return a const reference of the point at 'i'
     */
    virtual const CT_Point& constPointAt(const size_t &i) const = 0;
};

#endif // CT_ABSTRACTUNDEFINEDSIZEPOINTCLOUD_H
