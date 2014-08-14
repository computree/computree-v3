#ifndef CTG_INTURNMANAGER_H
#define CTG_INTURNMANAGER_H

#include <QWidget>

class CT_InTurnManager;
class CT_InTurn;

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

private:
    Ui::CTG_InTurnManager   *ui;
    CT_InTurnManager        *_manager;

    bool                    _constructEnabled;
    int                     _lastIndex;
    bool                    m_readOnly;

    void clearTabs();
    void constructTabs();

    QWidget* createTabForTurn(CT_InTurn *turn, int index);
    bool isAddTab(int index) const;

private slots:

    void currentTabChanged(int index);
    void removeCurrentTurn();

public slots:
    void setResultChoosed(const QString &mimeData);
};

#endif // CTG_INTURNMANAGER_H
