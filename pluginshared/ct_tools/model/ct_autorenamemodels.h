#ifndef CT_AUTORENAMEMODELS_H
#define CT_AUTORENAMEMODELS_H

#include "pluginShared_global.h"

#include <QString>
#include <QList>

class CT_AbstractModel;

class PLUGINSHAREDSHARED_EXPORT CT_AutoRenameModels
{
public:
    CT_AutoRenameModels();
    ~CT_AutoRenameModels();

    QString name() const;
    int extra() const;
    QString completeName() const;
    bool hasBeenModified() const;

protected:

    template< class ItemT, class InheritedT > friend class CT_AbstractGroupModelT;
    friend class CT_AbstractModel;

    void setName(QString name);
    void setExtra(int extra);

    bool renameAllModels();
    void addModel(const CT_AbstractModel *model);
    bool removeModel(CT_AbstractModel *model);
    const QList<CT_AbstractModel*>& models() const;

    void clear();

private:

    QString                     _name;
    int                         _extra;

    QList<CT_AbstractModel*>    _models;
};

#endif // CT_AUTORENAMEMODELS_H
