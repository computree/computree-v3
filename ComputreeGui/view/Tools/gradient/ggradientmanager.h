#ifndef GGRADIENTMANAGER_H
#define GGRADIENTMANAGER_H

#include <QWidget>

class GColorGradientView;

namespace Ui {
class GGradientManager;
}

/**
 * @brief Display a QToolButton with a menu that contains representation of gradients. The user can select a gradient.
 */
class GGradientManager : public QWidget
{
    Q_OBJECT

public:
    explicit GGradientManager(QWidget *parent = 0);
    ~GGradientManager();

    /**
     * @brief Add a gradient
     */
    void addGradient(const QLinearGradient &gradient);

    /**
     * @brief Set a gradient selected
     */
    void setGradientSelected(int i);

    /**
     * @brief Returns the selected gradient
     */
    QLinearGradient gradientSelected() const;

private:

    struct GradientWidgetInfo {
        GradientWidgetInfo(QWidget *pWid, GColorGradientView *gv) : m_parentWidget(pWid), m_gradientView(gv) {}

        GColorGradientView  *m_gradientView;
        QWidget             *m_parentWidget;
        bool                m_selected;

        void setSelected(bool s);
        bool isSelected() const;
    };

    Ui::GGradientManager *ui;
    QList<GradientWidgetInfo>  m_gradientViews;

    void setGradientWidgetSelected(QWidget *w);

    void updateToolButtonIcon();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void newGradientSelected(QLinearGradient gradient);
};

#endif // GGRADIENTMANAGER_H
