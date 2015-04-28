#ifndef PB_ACTIONMODIFYAFFILIATIONSOPTIONS_H
#define PB_ACTIONMODIFYAFFILIATIONSOPTIONS_H

#include "ct_view/actions/abstract/ct_gabstractactionoptions.h"

class PB_ActionModifyAffiliations;

namespace Ui {
class PB_ActionModifyAffiliationsOptions;
}

class PB_ActionModifyAffiliationsOptions : public CT_GAbstractActionOptions
{
    Q_OBJECT

public:

    explicit PB_ActionModifyAffiliationsOptions(const PB_ActionModifyAffiliations *action);
    ~PB_ActionModifyAffiliationsOptions();

    bool sourceVisible();
    bool targetVisible();
    bool affiliationsLinesVisible();
    bool itemsVisible();
    bool centersVisible();
    bool onlySelectedVisible();
    bool selectionActivated();
    bool showAttributes();

public slots:
    void toggleSelection();

signals:
    void parametersChanged();
    void askForAffiliation();
    void askForBreakingAffiliation();

private:
    Ui::PB_ActionModifyAffiliationsOptions *ui;

};

#endif // PB_ACTIONMODIFYAFFILIATIONSOPTIONS_H
