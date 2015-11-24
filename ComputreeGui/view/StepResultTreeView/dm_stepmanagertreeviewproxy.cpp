#include "dm_stepmanagertreeviewproxy.h"

#include "myqstandarditem.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"

#include <QFileInfo>

DM_StepManagerTreeViewProxy::DM_StepManagerTreeViewProxy(QObject *parent) : QSortFilterProxyModel(parent)
{
    m_stepNameConfig = GStepViewDefault::DNC_StepShortDescription;
}

QVariant DM_StepManagerTreeViewProxy::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        int columnType = index.data(MyQStandardItem::ColumnTypeRole).toInt();

        if(columnType == MyQStandardItem::StepName) {
            MyQStandardItem *it = dynamic_cast<MyQStandardItem*>(((QStandardItemModel*)sourceModel())->itemFromIndex(mapToSource(index)));

            if((it != NULL) && (it->step() != NULL)) {
                QString name = GStepViewDefault::staticGetStepNameFromConfiguration(it->step(), m_stepNameConfig);

                if(it->step()->isSettingsModified())
                    name = "*" + name;

                CT_AbstractStepLoadFile *stepLF = dynamic_cast<CT_AbstractStepLoadFile*>(it->step());

                if(stepLF != NULL)
                {
                    QString filePath = stepLF->getFilePath();
                    QFileInfo info(filePath);

                   if(!info.fileName().isEmpty())
                       name += tr("( %1 )").arg(info.fileName());
                }

                return name;
            }
        }
    }

    return QSortFilterProxyModel::data(index, role);
}

void DM_StepManagerTreeViewProxy::setStepNameConfig(GStepViewDefault::DisplayNameConfigs config)
{
    m_stepNameConfig = config;
    invalidate();
}
