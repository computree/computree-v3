#ifndef CT_GABSTRACTACTIONOPTIONS_H
#define CT_GABSTRACTACTIONOPTIONS_H

#include "interfaces.h"
#include "pluginShared_global.h"

class PLUGINSHAREDSHARED_EXPORT CT_GAbstractActionOptions : public ActionOptionsInterface
{
    Q_INTERFACES(ActionOptionsInterface)
    Q_OBJECT
public:
    explicit CT_GAbstractActionOptions(const CT_AbstractAction *action);

     CT_AbstractAction* action() const;

private:
     CT_AbstractAction  *m_action;
};

#endif // CT_GABSTRACTACTIONOPTIONS_H
