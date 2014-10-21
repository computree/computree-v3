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
     * @brief Returns true if this manager use the transform matrix of the item in method draw. If false
     *        the preDraw method multiply the current matrix with the matrix of the itemDrawable. By default returns false.
     */
    virtual bool useItemTransformMatrix() const;

    /**
     * @brief Pre draw
     */
    virtual void preDraw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable);

    /**
      * \brief Dessine l'ItemDrawable passé en paramètre.
      */
    virtual void draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const = 0;

    /**
     * @brief Post draw
     */
    virtual void postDraw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable);

    /**
      * \brief Retourne la configuration du dessin.
      */
    CT_ItemDrawableConfiguration *getDrawConfiguration() const;

protected:

    /**
      * \brief Retourne une nouvelle configuration du dessin.
      */
    virtual CT_ItemDrawableConfiguration createDrawConfiguration(QString drawConfigurationName) const;

    /**
     * @brief Set a draw configuration
     */
    void internalSetDrawConfiguration(CT_ItemDrawableConfiguration *configuration);

    /**
     * @brief Enable delete draw configuration. true by default.
     */
    void internalSetAutoDeleteDrawConfiguration(bool e);

private:

    QString                         _drawConfigurationNameBackup;
    CT_ItemDrawableConfiguration    *_configuration;
    bool                            m_autoDeleteConfig;
};

#endif // CT_ABSTRACTITEMDRAWABLEDRAWMANAGER_H
