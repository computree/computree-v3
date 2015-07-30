#ifndef DM_ITEMDRAWABLECONFIGURATIONANDGRAPHICSVIEWSYNCHRONIZER_H
#define DM_ITEMDRAWABLECONFIGURATIONANDGRAPHICSVIEWSYNCHRONIZER_H

#include "gitemdrawableconfigurationmanagerview.h"

/**
 * @brief Class that will update graphics view if a value changed in a itemdrawable configuration that was managed by "GItemDrawableConfigurationManagerView"
 */
class DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer : public QObject
{
    Q_OBJECT

public:
    DM_ItemDrawableConfigurationAndGraphicsViewSynchronizer();

    /**
     * @brief Set the manager to use to update graphics view present in the document of this manager when a value has changed in a configuration manger by this element
     */
    void setItemDrawableConfigurationView(GItemDrawableConfigurationManagerView *managerView);

private:
    GItemDrawableConfigurationManagerView *m_managerView;

private slots:
    /**
     * @brief Called when a value of a configuration manage by the manager view has changed
     */
    void valueHasChangedForConfiguration(CT_ItemDrawableConfiguration *config);
};

#endif // DM_ITEMDRAWABLECONFIGURATIONANDGRAPHICSVIEWSYNCHRONIZER_H
