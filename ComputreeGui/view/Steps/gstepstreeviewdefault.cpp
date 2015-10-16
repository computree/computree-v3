#include "gstepstreeviewdefault.h"

#include <QTimer>

GStepsTreeViewDefault::GStepsTreeViewDefault(QWidget *parent) :
    QTreeView(parent)
{
    // uncomment to disable the indentation and the arrow
    //setIndentation(0);
    setAnimated(true);
    setAutoExpandDelay(300);
    setExpandsOnDoubleClick(false);

    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(indexClicked(QModelIndex)));
}

void GStepsTreeViewDefault::indexClicked(const QModelIndex &index)
{
    if(index.isValid()
            && (index.model() != NULL))
    {
        QModelIndex newIndex = index.model()->sibling(index.row(), 0, index);

        if(newIndex.isValid()
            && (newIndex.model() != NULL)
            && (newIndex.model()->rowCount(newIndex) > 0)) {
            m_indexToExpand = newIndex;

            bool ok = false;

            if(!isExpanded(newIndex)) {

                QModelIndex parent = newIndex.parent();

                if(parent.isValid()) {
                    int s = model()->rowCount(parent);
                    int i = 0;

                    while((i < s) && !ok) {
                        QModelIndex child = parent.child(i, 0);

                        if(isExpanded(child))
                        {
                            collapse(child);
                            ok = true;
                        }

                        ++i;
                    }
                } else {
                    int s = model()->rowCount();
                    int i = 0;

                    while((i < s) && !ok) {
                        QModelIndex child = model()->index(i, 0);

                        if(isExpanded(child))
                        {
                            collapse(child);
                            ok = true;
                        }

                        ++i;
                    }
                }
            }

            if(ok)
                QTimer::singleShot(autoExpandDelay()+20, this, SLOT(expandLastIndex()));
            else
                expandLastIndex();
        }
    }
}

void GStepsTreeViewDefault::expandLastIndex()
{
    if(!isExpanded(m_indexToExpand))
        expand(m_indexToExpand);
}
