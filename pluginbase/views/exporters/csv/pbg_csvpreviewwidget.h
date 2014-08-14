#ifndef PBG_CSVPREVIEWWIDGET_H
#define PBG_CSVPREVIEWWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMenu>

class PB_CSVExporterConfiguration;
class IItemDataRefList;
class IItemDataRef;

namespace Ui {
class PBG_CSVPreviewWidget;
}

class PBG_CSVPreviewWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentDropPreviewColumnWidth READ currentDropPreviewColumnWidth WRITE setCurrentDropPreviewColumnWidth)

public:
    explicit PBG_CSVPreviewWidget(QWidget *parent = 0);
    ~PBG_CSVPreviewWidget();

    void setConfiguration(const PB_CSVExporterConfiguration *configuration);
    void setListOfDataRefList(const QList<const IItemDataRefList*> *list);

    bool updateConfiguration();

protected:

    /*!
     *  \brief Vérifie si l'élément à dropper est valide et accepte ou non le drop en fonction
     */
    void dragEnterEvent(QDragEnterEvent *event);

    /*!
     *  \brief Affiche l'aperçu de l'action du drop
     */
    void dragMoveEvent(QDragMoveEvent *event);

    /*!
     *  \brief Supprime l'aperçu
     */
    void dragLeaveEvent(QDragLeaveEvent *event);

    /*!
     *  \brief Traite le drop accepté
     */
    void dropEvent(QDropEvent *event);

    void keyPressEvent(QKeyEvent *e);

private:
    Ui::PBG_CSVPreviewWidget *ui;

    PB_CSVExporterConfiguration     *_configuration;
    QList<const IItemDataRefList*>  _list;
    QStandardItemModel              _model;
    QMenu                           *_headerContextMenu;
    int                             _currentDropPreviewColumnIndex;

    void createColumn(const IItemDataRefList *refList,
                      IItemDataRef *ref,
                      const QPoint &pos,
                      bool preview);

    void createColumn(const IItemDataRefList *refList,
                      IItemDataRef *ref,
                      int columnIndex,
                      bool preview);

    int currentDropPreviewColumnWidth() const;

    void updateViewFromConfiguration();

private slots:

    bool acceptItemAttribute(const QString &mimeData);
    void setCurrentDropPreviewColumnWidth(int size);
    void showHorizontalHeaderViewContextMenu(const QPoint &pos);
    void removeSelectedColumn();

public slots:
    void setItemAttribute(const QString &mimeData, QDropEvent *event = NULL);
};

#endif // PBG_CSVPREVIEWWIDGET_H
