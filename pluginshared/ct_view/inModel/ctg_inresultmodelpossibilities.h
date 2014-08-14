#ifndef CTG_INRESULTMODELPOSSIBILITIES_H
#define CTG_INRESULTMODELPOSSIBILITIES_H

#include <QWidget>
#include <QStandardItemModel>

class CT_InResultModelManager;
class CT_InAbstractResultModel;

namespace Ui {
class CTG_InResultModelPossibilities;
}

/*!
 *  Classe héritant de QStandardItemModel afin de retourner une QMimeData
 *  utilisable pour le drag d'un QStandardItem
 */
class CTG_InResultModelPossibilitiesDragModel : public QStandardItemModel
{
public:
    QMimeData* mimeData(const QModelIndexList &indexes) const;
};

/*!
 *  Widget permettant d'afficher les possibilités disponibles (résultats modèle
 *  de sortie) pour les résultats modèles d'entrée du gestionnaire de résultats modèles
 *  d'entrée.
 *
 *  L'utilisateur peut faire un drag des possibilités vers un autre widget (CTG_InResultModelTurnChoice).
 */
class CTG_InResultModelPossibilities : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTG_InResultModelPossibilities(QWidget *parent = 0);
    ~CTG_InResultModelPossibilities();

    /*!
     *  \brief Définir le gestionnaire des résultats modèles d'entrée
     */
    void setInResultModelManager(const CT_InResultModelManager *manager);
    
    void setReadOnly(bool enabled);

private:
    Ui::CTG_InResultModelPossibilities          *ui;
    CT_InResultModelManager                     *_manager;

    CTG_InResultModelPossibilitiesDragModel     _viewModel;

    void clearModel();
    void constructModel();
    void constructHeader();

    QList<QStandardItem*> createItemsForResultModel(CT_InAbstractResultModel *resModel);

signals:

    void resultChoosed(const QString &mimeData);

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);
};

#endif // CTG_INRESULTMODELPOSSIBILITIES_H
