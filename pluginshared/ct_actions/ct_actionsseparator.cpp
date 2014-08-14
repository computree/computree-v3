#include "ct_actionsseparator.h"
#include "ct_actions/abstract/ct_abstractaction.h"

CT_ActionsSeparator::CT_ActionsSeparator(QString title)
{
    m_title = title;
}

CT_ActionsSeparator::~CT_ActionsSeparator()
{
    qDeleteAll(m_actions.begin(), m_actions.end());
}

bool CT_ActionsSeparator::addAction(CT_AbstractAction *action)
{
    if(action != NULL)
        m_actions.append(action);

    return (action != NULL);
}

QString CT_ActionsSeparator::title() const
{
    return m_title;
}

QList<CT_AbstractAction*> CT_ActionsSeparator::actions() const
{
    return m_actions;
}
