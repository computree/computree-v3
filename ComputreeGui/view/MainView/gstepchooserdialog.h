#ifndef GSTEPCHOOSERDIALOG_H
#define GSTEPCHOOSERDIALOG_H

#include "interfaces.h"
#include "ct_step/tools/menu/ct_menulevel.h"

#include <QDialog>

namespace Ui {
class GStepChooserDialog;
}

class GStepManager;
class GStepViewDefault;

class GStepChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GStepChooserDialog(QWidget *parent = 0);
    ~GStepChooserDialog();

    void setStepManager(GStepManager *stepManager);

    void init();

    GStepViewDefault* stepsChooserWidget() const;

private:
    Ui::GStepChooserDialog *ui;
    GStepManager            *m_stepManager;
    QAction                 *m_actionAddStepToFavorites;
    QAction                 *m_actionRemoveStepFromFavorites;
    QAction                 *m_actionRemoveLevelFromFavorites;

    bool existInFavorites(CT_VirtualAbstractStep *step);

    bool existInLevelRecursively(CT_MenuLevel *level, CT_VirtualAbstractStep *step);

protected:
    bool event(QEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    void closeEvent(QCloseEvent *e);

private slots:
    void addStepFromStepChooserToFavoritesMenu();
    void removeStepFromFavoritesMenu();
    void showSelectedStepInformation();
    void removeLevelFromFavoritesMenu();
    void refreshContextMenuOfStep(CT_VirtualAbstractStep *step);
    void refreshContextMenuOfLevel(CT_MenuLevel *level);

public slots:
    void toggleVisibility();

signals:
    void visibilityChanged(bool visible);
    void maximizedChanged(bool maximized);
};

#endif // GSTEPCHOOSERDIALOG_H
