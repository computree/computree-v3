#ifndef DM_GUIMANAGERTOKEN_H
#define DM_GUIMANAGERTOKEN_H

#include <QObject>

#include "tools/guimanager/dm_abstractasyncoperationoptions.h"

/**
 * @brief A token used by the gui manager to manage exclusive async operation and block the workflows. Only one async operation
 *        can be done both. When a element request to do a async operation the gui manager create a token, append them
 *        in a list and blocks the workflow (without block the gui) until they are other token before them and until the
 *        async options don't pass. When it is the turn for this token and when the async options passed the workflows continue.
 */
class DM_GuiManagerToken : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Create a token with a async options. When the options signal 'passed' is emitted this token
     *        emit the signal 'passed'. options can't be NULL.
     */
    DM_GuiManagerToken(const DM_AbstractAsyncOperationOptions *options);

    DM_AbstractAsyncOperationOptions* options() const;

private:
    DM_AbstractAsyncOperationOptions    *m_options;

signals:

    /**
     * @brief passed
     */
    void passed();
};

#endif // DM_GUIMANAGERTOKEN_H
