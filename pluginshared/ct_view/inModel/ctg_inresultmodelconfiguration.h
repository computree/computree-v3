#ifndef CTG_INRESULTMODELCONFIGURATION_H
#define CTG_INRESULTMODELCONFIGURATION_H

#include <QDialog>

class CT_InManager;

namespace Ui {
class CTG_InResultModelConfiguration;
}

/*!
 *  \brief Fenêtre de configuration des tours et modèles d'entrées.
 *
 *  L'utilisateur peut, par le biais de cette fenêtre, configurer les résultats
 *  d'entrées, les ItemDrawable à utiliser et le nombre de tour à effectuer.
 */
class CTG_InResultModelConfiguration : public QDialog
{
    Q_OBJECT
    
public:
    explicit CTG_InResultModelConfiguration(QWidget *parent = 0);
    ~CTG_InResultModelConfiguration();
    
    /*!
     *  \brief Définir le Manager d'entrée
     */
    void setInManager(const CT_InManager *manager);

    void setReadOnly(bool enabled);

private:
    Ui::CTG_InResultModelConfiguration  *ui;

    CT_InManager                        *_inManager;

public slots:

    /*!
     *  \brief Affiche la fenêtre
     */
    int exec();

    void accept();
};

#endif // CTG_INRESULTMODELCONFIGURATION_H
