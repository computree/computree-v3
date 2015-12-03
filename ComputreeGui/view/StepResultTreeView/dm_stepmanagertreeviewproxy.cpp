#include "dm_stepmanagertreeviewproxy.h"

#include "myqstandarditem.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"

#include <QFileInfo>

DM_StepManagerTreeViewProxy::DM_StepManagerTreeViewProxy(QObject *parent) : QSortFilterProxyModel(parent)
{
    m_stepNameConfig = GStepViewDefault::DNC_StepShortDescription;

    m_progressAnimation.setTargetObject(this);
    m_progressAnimation.setPropertyName("ProgressValue");
    m_progressAnimation.setStartValue(0);
    m_progressAnimation.setEndValue(360);
    m_progressAnimation.setDuration(500);
    m_progressAnimation.setLoopCount(-1);

    m_progressAnimation.start();
}

MyQStandardItem* getStepFromIndex(const QModelIndex &index, QAbstractItemModel *sModel, const QSortFilterProxyModel *proxy)
{
    return dynamic_cast<MyQStandardItem*>(((QStandardItemModel*)sModel)->itemFromIndex(proxy->mapToSource(index)));
}

QVariant DM_StepManagerTreeViewProxy::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        int columnType = index.data(MyQStandardItem::ColumnTypeRole).toInt();

        if(columnType == MyQStandardItem::StepName) {
            MyQStandardItem *it = getStepFromIndex(index, sourceModel(), this);

            if((it != NULL) && (it->step() != NULL)) {
                QString name = GStepViewDefault::staticGetStepNameFromConfiguration(it->step(), m_stepNameConfig);

                int progress = it->step()->getProgress();

                if(progress != 100)
                    name = "(" + QString().setNum(progress) + "%)" + name;

                /*if(it->step()->isSettingsModified())
                    name = "*" + name;*/

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
    else if(role == Qt::DecorationRole) {
        int columnType = index.data(MyQStandardItem::ColumnTypeRole).toInt();

        if(columnType == MyQStandardItem::StepName) {
            MyQStandardItem *it = getStepFromIndex(index, sourceModel(), this);

            if((it != NULL) && (it->step() != NULL)) {

                int progress = it->step()->getProgress();

                if(it->step()->isSettingsModified() && ((progress == 0) || (progress == 100))) {
                    return QIcon(":/Icones/Icones/view-refresh.png");
                }

                if(progress == 100) {
                    return QIcon(":/Icones/Icones/valid.png");

                    const_cast<QPropertyAnimation&>(m_progressAnimation).stop();
                } else if(m_progressAnimation.state() == QAbstractAnimation::Stopped) {
                    const_cast<QPropertyAnimation&>(m_progressAnimation).start();
                }

                QPixmap pixmap(":/Icones/Icones/in_progress.png");
                QMatrix rm;
                rm.rotate(m_progressValue);

                QPixmap rotated = pixmap.transformed(rm);
                int xoffset = (rotated.width() - pixmap.width()) / 2;
                int yoffset = (rotated.height() - pixmap.height()) / 2;

                return rotated.copy(xoffset, yoffset, pixmap.width(), pixmap.height()).scaled(18, 18, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            }
        }
    }

    return QSortFilterProxyModel::data(index, role);
}

int DM_StepManagerTreeViewProxy::ProgressValue() const
{
    return m_progressValue;
}

void DM_StepManagerTreeViewProxy::setProgressValue(int p)
{
    m_progressValue = p;
}

void DM_StepManagerTreeViewProxy::setStepNameConfig(GStepViewDefault::DisplayNameConfigs config)
{
    m_stepNameConfig = config;
    invalidate();
}
