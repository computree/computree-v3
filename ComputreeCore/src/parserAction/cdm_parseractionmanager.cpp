#include "cdm_parseractionmanager.h"

CDM_ParserActionManager::CDM_ParserActionManager()
{
}

CDM_ParserActionManager::~CDM_ParserActionManager()
{
    while(!_actions.isEmpty())
        delete _actions.takeFirst().second;
}

bool CDM_ParserActionManager::addAction(const QString &cmd, CDM_IParserAction *action)
{
    QString newCmd;

    if((action == NULL)
            || (findAction(cmd) != NULL))
        return false;

    _actions.append(qMakePair(cmd, action));

    return true;
}

CDM_IParserAction* CDM_ParserActionManager::findAction(const QString &fullCmd, QString *newCmd) const
{
    QListIterator< QPair<QString, CDM_IParserAction*> > it(_actions);

    while(it.hasNext())
    {
        const QPair<QString, CDM_IParserAction*> &pair= it.next();

        if(fullCmd.startsWith(pair.first))
        {
            if(newCmd != NULL)
            {
                QString tmp = fullCmd;

                newCmd->clear();
                newCmd->append(tmp.remove(0, pair.first.length()));
            }

            return pair.second;
        }
    }

    return NULL;
}

QStringList CDM_ParserActionManager::getAllActionCmd() const
{
    QStringList li;

    QListIterator< QPair<QString, CDM_IParserAction*> > it(_actions);

    while(it.hasNext())
    {
        li.append(it.next().first);
    }

    return li;
}
