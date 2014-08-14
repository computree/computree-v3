#include "pb_actionmodifyaffiliationsoptions.h"
#include "ui_pb_actionmodifyaffiliationsoptions.h"
#include "ct_global/ct_context.h"

#include "actions/pb_actionmodifyaffiliations.h"

PB_ActionModifyAffiliationsOptions::PB_ActionModifyAffiliationsOptions(const PB_ActionModifyAffiliations *action) :
    CT_GAbstractActionOptions(action),
    ui(new Ui::PB_ActionModifyAffiliationsOptions)
{
    ui->setupUi(this);


    connect(ui->cb_source, SIGNAL(clicked()), this, SIGNAL(parametersChanged()));
    connect(ui->cb_target, SIGNAL(clicked()), this, SIGNAL(parametersChanged()));
    connect(ui->cb_affiliations, SIGNAL(clicked()), this, SIGNAL(parametersChanged()));
    connect(ui->cb_showItems, SIGNAL(clicked()), this, SIGNAL(parametersChanged()));
    connect(ui->cb_showCenters, SIGNAL(clicked()), this, SIGNAL(parametersChanged()));
    connect(ui->cb_selectedOnly, SIGNAL(clicked()), this, SIGNAL(parametersChanged()));
    connect(ui->pb_affiliate, SIGNAL(clicked()), this, SIGNAL(askForAffiliation()));
    connect(ui->pb_break, SIGNAL(clicked()), this, SIGNAL(askForBreakingAffiliation()));

    ui->cb_affiliations->setToolTip(tr("Afficher les lignes reliant les items affiliés"));
    ui->cb_selectedOnly->setToolTip(tr("N'afficher que les items sélectionnés (de référence et à affilier"));
    ui->cb_showCenters->setToolTip(tr("Afficher les centres des items"));
    ui->cb_showItems->setToolTip(tr("Afficher les items"));
    ui->cb_source->setToolTip(tr("Afficher les items de référence"));
    ui->cb_target->setToolTip(tr("Afficher les items à affilier"));
    ui->pb_affiliate->setToolTip(tr("Affilier l'item de référence et l'item à affilier sélectionnés (A)"));
    ui->pb_break->setToolTip(tr("Supprimer l'affiliation de l'item à affilier sélectionné (Z)"));

    ui->pb_selection->setToolTip(tr("Activer la selection (S) :\n"
                                    "- Clic gauche = sélectionner un item de référence\n"
                                    "- Clic droit  = sélectionner un item à affilier"));
}

PB_ActionModifyAffiliationsOptions::~PB_ActionModifyAffiliationsOptions()
{
    delete ui;
}

bool PB_ActionModifyAffiliationsOptions::sourceVisible()
{
    return ui->cb_source->isChecked();
}

bool PB_ActionModifyAffiliationsOptions::targetVisible()
{
    return ui->cb_target->isChecked();
}

bool PB_ActionModifyAffiliationsOptions::affiliationsLinesVisible()
{
    return ui->cb_affiliations->isChecked();
}

bool PB_ActionModifyAffiliationsOptions::itemsVisible()
{
    return ui->cb_showItems->isChecked();
}

bool PB_ActionModifyAffiliationsOptions::centersVisible()
{
    return ui->cb_showCenters->isChecked();
}

bool PB_ActionModifyAffiliationsOptions::onlySelectedVisible()
{
    return ui->cb_selectedOnly->isChecked();
}

bool PB_ActionModifyAffiliationsOptions::selectionActivated()
{
    return ui->pb_selection->isChecked();
}

void PB_ActionModifyAffiliationsOptions::toggleSelection()
{
    ui->pb_selection->setChecked(!ui->pb_selection->isChecked());
}



