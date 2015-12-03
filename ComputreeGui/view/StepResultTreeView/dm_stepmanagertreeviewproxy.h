#ifndef DM_STEPMANAGERTREEVIEWPROXY_H
#define DM_STEPMANAGERTREEVIEWPROXY_H

#include <QSortFilterProxyModel>
#include <QPropertyAnimation>

#include "view/Steps/gstepviewdefault.h"

/**
 * @brief Use this proxy model with a model constructed by "GStepManager".
 *        This class can personnalize the name of steps displayed
 */
class DM_StepManagerTreeViewProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    Q_PROPERTY(int ProgressValue READ ProgressValue WRITE setProgressValue)

    DM_StepManagerTreeViewProxy(QObject *parent = 0);

    /**
     * @brief Returns the personalized value of the index for the role passed in parameter
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    int ProgressValue() const;

public slots:
    void setProgressValue(int p);

private:
    GStepViewDefault::DisplayNameConfigs    m_stepNameConfig;
    QPropertyAnimation                      m_progressAnimation;
    int                                     m_progressValue;

public slots:
    void setStepNameConfig(GStepViewDefault::DisplayNameConfigs config);
};

#endif // DM_STEPMANAGERTREEVIEWPROXY_H
