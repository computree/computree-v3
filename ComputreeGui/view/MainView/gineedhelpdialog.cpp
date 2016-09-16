#include "gineedhelpdialog.h"
#include "ui_gineedhelpdialog.h"

GINeedHelpDialog::GINeedHelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GINeedHelpDialog)
{
    ui->setupUi(this);


    ui->content->append(tr("Pour le <b>sommaire de l'aide Computree</b> :"));
    ui->content->append(tr("-> <a href=\"http://rdinnovation.onf.fr/projects/computree/wiki\">Sommaire de la Wiki Computree</a>"));
    ui->content->append("<br>");

    ui->content->append(tr("Pour de l'aide sur <b>une étape insérée</b> :"));
    ui->content->append(tr("-> Faire un clic droit sur l'étape insérée, puis <i>Documentation de l'étape</i>"));
    ui->content->append("<br>");

    ui->content->append(tr("Pour de l'aide sur <b>une étape du menu (pas encore insérée)</b> :"));
    ui->content->append(tr("-> Faire un clic droit sur l'étape dans le menu, puis <i>Documentation de l'étape</i>"));
    ui->content->append("<br>");

    ui->content->append(tr("Pour <b>signaler un bug</b> ou <b>suggérer une amélioration</b> :"));
    ui->content->append(tr("-> Créer une nouvelle demande : <a href=\"http://rdinnovation.onf.fr/projects/computree/issues/new\">Nouvelle demande</a>"));
    ui->content->append("<br>");

    ui->content->append(tr("Pour <b>poster un message sur le forum Computree</b> :"));
    ui->content->append(tr("-> <a href=\"http://rdinnovation.onf.fr/projects/computree/boards\">Forum Computree</a>"));
    ui->content->append("<br>");
}

GINeedHelpDialog::~GINeedHelpDialog()
{
    delete ui;
}
