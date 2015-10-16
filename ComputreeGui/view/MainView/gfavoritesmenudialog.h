#ifndef GFAVORITESMENUDIALOG_H
#define GFAVORITESMENUDIALOG_H

#include <QDialog>
#include "interfaces.h"

namespace Ui {
class GFavoritesMenuDialog;
}

/**
 * @brief Dialogue to add a step to a level in favorites menu
 */
class GFavoritesMenuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GFavoritesMenuDialog(QWidget *parent = 0);
    ~GFavoritesMenuDialog();

    void init();

    void setStepToAdd(CT_VirtualAbstractStep *step);

private:
    Ui::GFavoritesMenuDialog *ui;
    CT_VirtualAbstractStep   *m_step;
protected:
    void accept();

private slots:

    void on_pushButtonAddLevel_clicked();
};

#endif // GFAVORITESMENUDIALOG_H
