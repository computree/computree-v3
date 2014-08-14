#ifndef CT_ACTIONSSEPARATOR_H
#define CT_ACTIONSSEPARATOR_H

#include "interfaces.h"
#include "pluginShared_global.h"

class PLUGINSHAREDSHARED_EXPORT CT_ActionsSeparator
{
public:
    CT_ActionsSeparator(QString title = "");
    virtual ~CT_ActionsSeparator();

    bool addAction(CT_AbstractAction *action);

    QString title() const;
    QList<CT_AbstractAction*> actions() const;

private:

    QList<CT_AbstractAction*>     m_actions;
    QString                     m_title;
};

#endif // CT_ACTIONSSEPARATOR_H
