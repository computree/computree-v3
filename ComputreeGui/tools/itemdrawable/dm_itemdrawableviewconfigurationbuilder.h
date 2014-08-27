#ifndef DM_ITEMDRAWABLEVIEWCONFIGURATIONBUILDER_H
#define DM_ITEMDRAWABLEVIEWCONFIGURATIONBUILDER_H

#include "ct_itemdrawable/ct_itemdrawableconfiguration.h"

#include <QMutex>

class DM_ItemDrawableViewConfiguration
{
public:
    DM_ItemDrawableViewConfiguration() { m_config = NULL; }
    DM_ItemDrawableViewConfiguration(const CT_ItemDrawableConfiguration *config);

    CT_ItemDrawableConfiguration* configuration() const;

private:
    CT_ItemDrawableConfiguration *m_config;
};

Q_DECLARE_METATYPE(DM_ItemDrawableViewConfiguration)

/**
 * @brief Class tools to create a list of differents view configuration of CT_AbstractItemDrawable [item.getViewConfiguration()] added to the builder.
 *        A signal [listChanged] is emitted when the list changed.
 */
class DM_ItemDrawableViewConfigurationBuilder : public QObject
{
    Q_OBJECT

public:
    DM_ItemDrawableViewConfigurationBuilder(bool searchInChildren = false);

    /**
     * @brief Return the configuration with the name 'name'
     */
    DM_ItemDrawableViewConfiguration getConfigurationWithName(const QString &name) const;

    /**
     * @brief Return the differents configuration added to the builder
     */
    QList<DM_ItemDrawableViewConfiguration> configurations() const;

public slots:

    void addItemDrawable(CT_AbstractItemDrawable &item);
    void removeItemDrawable(CT_AbstractItemDrawable &item);
    void clear();

private:
    QMap<CT_ItemDrawableConfiguration*, size_t>    m_config;
    bool                                        m_searchInChildren;
    QMutex                                      m_mutex;

signals:
    void listChanged();
    void added(DM_ItemDrawableViewConfiguration config);
    void removed(DM_ItemDrawableViewConfiguration config);
};

#endif // DM_ITEMDRAWABLEVIEWCONFIGURATIONBUILDER_H
