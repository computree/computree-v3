#include "myqmenu.h"

#include <QAction>
#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "view/MainView/gaboutstepdialog.h"
#include "view/StepResultTreeView/myqaction.h"


void MyQMenu::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F1)
    {
        QAction* action = this->activeAction();
        MyQAction* myAction = dynamic_cast<MyQAction*>(action);

        if (myAction != NULL)
        {
            CT_VirtualAbstractStep *step = myAction->step();
            GAboutStepDialog dialog(step);
            dialog.exec();
        }
    }
}
