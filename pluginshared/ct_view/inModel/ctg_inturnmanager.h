#ifndef CTG_INTURNMANAGER_H
#define CTG_INTURNMANAGER_H

#include <QWidget>

class CT_InTurnManager;
class CT_InTurn;
class CT_InAbstractResultModel;
class CT_InStdModelPossibility;

namespace Ui {
class CTG_InTurnManager;
}

class CTG_InTurnManager : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTG_InTurnManager(QWidget *parent = 0);
    ~CTG_InTurnManager();
    
    void setInTurnManager(const CT_InTurnManager *manager);

    void reConstruct();

    void setReadOnly(bool enabled);

    CT_InAbstractResultModel* getModelFromMimeDataForCurrentTurn(const QString &mimeData);
    CT_InStdModelPossibility *getPossibilityFromMimeDataForCurrentTurn(const QString &mimeData, int *outPossibilityIndex);

private:
    enum ActionType {
        Enable,
        Disable,
        Show
    };

    Ui::CTG_InTurnManager   *ui;
    CT_InTurnManager        *_manager;

    bool                    _constructEnabled;
    int                     _lastIndex;
    bool                    m_canAddANewTab;
    bool                    m_readOnly;

    void clearTabs();
    void constructTabs();

    QWidget* createTabForTurn(CT_InTurn *turn, int index);
    bool isAddTab(int index) const;

    void setResultPossibility(const QString &mimeData, ActionType action);
private slots:

    void currentTabChanged(int index);
    void removeCurrentTurn();

public slots:
    void enableResultPossibility(const QString &mimeData);
    void disableResultPossibility(const QString &mimeData);
    void showResultPossibility(const QString &mimeData);
};

#endif // CTG_INTURNMANAGER_H
