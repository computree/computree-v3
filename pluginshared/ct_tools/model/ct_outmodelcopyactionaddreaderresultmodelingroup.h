#ifndef CT_OUTMODELCOPYACTIONADDREADERRESULTMODELINGROUP_H
#define CT_OUTMODELCOPYACTIONADDREADERRESULTMODELINGROUP_H

#include "ct_tools/model/abstract/ct_abstractoutmodelcopyaction.h"
#include "ct_tools/model/ct_autorenamemodels.h"
#include "ct_itemdrawable/model/outModel/abstract/def_ct_abstractgroupmodelout.h"

#include <QHash>

class CT_AbstractReader;

class PLUGINSHAREDSHARED_EXPORT CT_OutModelCopyActionAddReaderResultModelInGroup : public CT_AbstractOutModelCopyAction
{
public:
    CT_OutModelCopyActionAddReaderResultModelInGroup(const QString &inParentGroupModelName,
                                                     const QString &inReaderModelName);

    CT_OutModelCopyActionAddReaderResultModelInGroup(const CT_AutoRenameModels &inParentGroupModelAutoRename,
                                                     CT_AbstractReader *reader);

    ~CT_OutModelCopyActionAddReaderResultModelInGroup();

    bool execute(const CT_VirtualAbstractStep *step, CT_OutResultModelGroupToCopyPossibility *modelCopy);

    QHash<CT_AbstractModel*, QHash<QString, CT_AutoRenameModels*> > takeAutoRenameLinksByGroupModel();

    static bool staticAddElementOfReaderToGroup(CT_AbstractReader *reader,
                                                DEF_CT_AbstractGroupModelOut *outGroupModel,
                                                QHash<CT_AbstractModel *, QHash<QString, CT_AutoRenameModels *> > &outLinks);

private:
    QString                                                             _inParentGroupModelName;
    CT_AutoRenameModels                                                 *_inAutoRenameGroupModel;
    QString                                                             _inReaderModelName;
    CT_AbstractReader                                                   *_reader;
    QHash<CT_AbstractModel*, QHash<QString, CT_AutoRenameModels*> >     m_autoRenameLinks;
};

#endif // CT_OUTMODELCOPYACTIONADDREADERRESULTMODELINGROUP_H
