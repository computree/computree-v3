#ifndef DM_ABSTRACTASYNCOPERATIONOPTIONS_H
#define DM_ABSTRACTASYNCOPERATIONOPTIONS_H

#include <QObject>

/**
 * @brief When you request a async operation you pass a async options that checks several things to know if
 *        the context is able to made the async operation. Per example if they are another exclusive async operation in progress
 *        the options return false.
 */
class DM_AbstractAsyncOperationOptions : public QObject
{
    Q_OBJECT

public:
    DM_AbstractAsyncOperationOptions();

    /**
     * @brief true by default
     */
    void setWaitUntilAsyncOperationReleased(bool enable);

    /**
     * @brief false by default
     */
    void setWaitUntilStepManagerIsFinished(bool enable);

    /**
     * @brief return true if the context is able to made the async operation
     */
    virtual bool pass() const;

private:
    bool m_waitUntilAsyncOperationReleased;
    bool m_waitUntilStepManagerIsFinished;

signals:
    /**
     * @brief emitted when the context is able to made the async operation
     */
    void passed();

private slots:
    void slotTestPass();
};

#endif // DM_ABSTRACTASYNCOPERATIONOPTIONS_H
