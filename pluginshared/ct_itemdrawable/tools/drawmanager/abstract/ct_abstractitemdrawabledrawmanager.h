#ifndef CT_ABSTRACTITEMDRAWABLEDRAWMANAGER_H
#define CT_ABSTRACTITEMDRAWABLEDRAWMANAGER_H

#include "pluginShared_global.h"
#include "interfaces.h"

#include "ct_itemdrawable/ct_itemdrawableconfiguration.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractItemDrawableDrawManager
{
public:
    CT_AbstractItemDrawableDrawManager(const QString &drawConfigurationName = "");
    virtual ~CT_AbstractItemDrawableDrawManager();

    /**
      * \brief Initialise de la configuration du dessin.
      */
    void initDrawConfiguration();

    /**
      * \brief Dessine l'ItemDrawable passé en paramètre.
      */
    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const = 0;

    /**
      * \brief Retourne la configuration du dessin.
      */
    CT_ItemDrawableConfiguration *getDrawConfiguration() const;

protected:

    /**
      * \brief Retourne une nouvelle configuration du dessin.
      */
    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

private:

    QString                         _drawConfigurationNameBackup;
    CT_ItemDrawableConfiguration    *_configuration;
};

#endif // CT_ABSTRACTITEMDRAWABLEDRAWMANAGER_H
