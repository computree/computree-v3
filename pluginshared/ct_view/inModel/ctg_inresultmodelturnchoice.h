#ifndef CTG_INRESULTMODELTURNCHOICE_H
#define CTG_INRESULTMODELTURNCHOICE_H

#include <QWidget>
#include <QStandardItemModel>

class CT_InTurn;
class CT_InAbstractResultModel;
class CT_InStdResultModelPossibility;

namespace Ui {
class CTG_InResultModelTurnChoice;
}

/*!
 *  Classe utilisé par le widget pour garder certaines informations dans l'arbre
 */
class CTG_InResultModelTurnChoiceItem : public QStandardItem
{
public:

    CTG_InResultModelTurnChoiceItem(const QString & text) : QStandardItem(text) { _possibility = NULL; }

    CT_InStdResultModelPossibility     *_possibility;
};

/*!
 *  Widget permettant d'afficher les résultats modèles d'entrées
 *  d'un tour sous forme d'un arbre. Un drop d'un modèle de sortie
 *  sur l'arbre est possible si celui-ci fait partie des possibilités
 *  d'un des modèles d'entrée. Si tel est le cas la possibilité
 *  est sélectionnée et le résultat de sortie correspondant est
 *  affiché dans l'arbre.
 *
 *  Le drop n'est accepté que si la possibilité est sélectionnable. Si
 *  une seule parmis toutes est sélectionnable, elle sera remplacé par
 *  le nouveau drop. Sinon le nouveau drop sera ajouté au modèle d'entrée.
 */
class CTG_InResultModelTurnChoice : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTG_InResultModelTurnChoice(QWidget *parent = 0);
    ~CTG_InResultModelTurnChoice();
    
    /*!
     *  \brief Définir le tour à afficher
     */
    void setTurn(const CT_InTurn *turn);

    void setReadOnly(bool enabled);

protected:

    /*!
     *  \brief Vérifie si l'élément à dropper est valide et accepte ou non le drop en fonction
     */
    void dragEnterEvent(QDragEnterEvent *event);

    /*!
     *  \brief Traite le drop accepté
     */
    void dropEvent(QDropEvent *event);

private:
    Ui::CTG_InResultModelTurnChoice     *ui;
    CT_InTurn                           *_turn;

    QStandardItemModel                  _viewModel;
    CT_InStdResultModelPossibility      *m_possibilityToSelect;
    QStandardItem                       *m_itemToSelect;

    /*!
     *  \brief Efface le modèle de la QTreeView
     */
    void clearModel();

    /*!
     *  \brief Construit le modèle à afficher par la QTreeView (clearModel(); constructHeader(); puis construit le modèle si un tour est défini)
     */
    void constructModel();

    /*!
     *  \brief Construit l'en-tête de la QTreeView
     */
    void constructHeader();

    /*!
     *  \brief Retourne les QStandardItem à ajouté au modèle en fonction d'un CT_InAbstractResultModel
     *
     *  \param resModel : le modèle à convertir pour être affiché dans l'arbre
     */
    QList<QStandardItem*> createItemsForResultModel(CT_InAbstractResultModel *resModel);

private slots:

    bool acceptResult(const QString &mimeData);
    void currentSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void showViewContextMenu(const QPoint &point);
    void removeSelectedPossibility();

public slots:
    void setResultChoosed(const QString &mimeData);

signals:

    void selectedPossibilityChanged(const CT_InStdResultModelPossibility *possibility);
};

#endif // CTG_INRESULTMODELTURNCHOICE_H
