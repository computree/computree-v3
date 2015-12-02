#ifndef GFAVORITESMENUDIALOG_H
#define GFAVORITESMENUDIALOG_H

#include <QDialog>
#include "interfaces.h"
#include "view/Steps/gstepviewdefault.h"

namespace Ui {
class GFavoritesMenuDialog;
}

class CT_MenuLevel;
class QTreeWidgetItem;

/**
 * @brief Dialog to add a step to a level in favorites or to edit favorites
 */
class GFavoritesMenuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GFavoritesMenuDialog(QWidget *parent = 0);
    ~GFavoritesMenuDialog();

    /**
     * @brief Init the view (tree of levels)
     */
    void init();

    /**
     * @brief Set the step to add and the config to display his name
     */
    void setStepToAdd(CT_VirtualAbstractStep *step, GStepViewDefault::DisplayNameConfigs config);

private:
    Ui::GFavoritesMenuDialog                *ui;
    CT_VirtualAbstractStep                  *m_step;
    GStepViewDefault::DisplayNameConfigs    m_nameConfig;

    /**
     * @brief Add a level to the treeview
     */
    QTreeWidgetItem* recursiveAddLevelToTreeWidget(CT_MenuLevel *level);

    /**
     * @brief Convert or remove childrens of parentItem to favorites
     */
    void recursiveAddOrRemoveLevelToFavorites(QTreeWidgetItem *parentItem, CT_MenuLevel *parentLevel);

    /**
     * @brief update the label who show the level to add the step and his name
     */
    void updateStepLabel();

    /**
     * @brief Set a CT_MenuLevel to a QTreeWidgetItem
     */
    void setLevelToItem(CT_MenuLevel *level, QTreeWidgetItem *item);

    /**
     * @brief Convert a QTreeWidgetItem to a CT_MenuLevel (can return NULL)
     */
    CT_MenuLevel* levelFromItem(QTreeWidgetItem *item);

protected:
    void accept();

private slots:
    void displayContextMenu(QPoint p);
    void addRootLevel();
    void addSubLevel();
    void removeLevel();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void on_pushButtonAddRootLevel_clicked();
    void on_pushButtonAddSubLevel_clicked();
    void on_pushButtonRemoveLevel_clicked();
};

#endif // GFAVORITESMENUDIALOG_H
