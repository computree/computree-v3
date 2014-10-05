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

    ui->content->append(tr("Pour de l'aide sur <b>l'utilisation de l'interface graphique</b> :"));
    ui->content->append(tr("-> <a href=\"http://rdinnovation.onf.fr/projects/computree/wiki/Fr_computreeGUI\">Wiki sur l'interface utilisateur</a>"));
    ui->content->append("<br>");

    ui->content->append(tr("Pour de l'aide sur <b>une étape insérée</b> :"));
    ui->content->append(tr("-> Faire un clic droit sur l'étape insérée, puis <i>Informations sur l'étape</i>"));
    ui->content->append("<br>");

    ui->content->append(tr("Pour de l'aide sur <b>une étape dans un plugin (pas encore insérée)</b> :"));
    ui->content->append(tr("-> Dans la barre de menu, cliquer sur <i>Aide</i>, puis <i>A propos des Plugins</i>"));
    ui->content->append(tr("-> Déplier <i>Plugins d'étapes</i>, déplier le plugin d'intérêt, séléctionner l'étape, puis cliquer sur <i>Info Etape</i>"));
    ui->content->append("<br>");

    ui->content->append(tr("Pour de l'aide sur <b>comment réaliser une tâche précise à l'aide des étapes disponibles</b> :"));
    ui->content->append(tr("-> Consulter les tutoriels disponibles ici : <a href=\"http://rdinnovation.onf.fr/projects/computree/wiki/Fr_tutorials\">Tutoriels thématiques (How To's)</a>"));
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
