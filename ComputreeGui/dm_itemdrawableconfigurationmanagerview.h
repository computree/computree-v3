#ifndef DM_ITEMDRAWABLECONFIGURATIONMANAGERVIEW_H
#define DM_ITEMDRAWABLECONFIGURATIONMANAGERVIEW_H

class DM_DocumentView;

/**
 * @brief Interface for the manager view of configuration of itemdrawable. The view will get all configuration present in the
 *        document and show it to change it's values.
 */
class DM_ItemDrawableConfigurationManagerView
{
public:
    virtual ~DM_ItemDrawableConfigurationManagerView() {}

    /**
     * @brief Get the document used
     */
    virtual DM_DocumentView* getDocumentView() const = 0;

    /**
     * @brief Set the document to use
     */
    virtual void setDocument(DM_DocumentView *doc) = 0;
};

#endif // DM_ITEMDRAWABLECONFIGURATIONMANAGERVIEW_H
