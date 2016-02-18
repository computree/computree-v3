#ifndef DM_CONTEXTMENUCOLOURISTADDER_H
#define DM_CONTEXTMENUCOLOURISTADDER_H

#include <QMenu>
#include <QList>

class CT_AbstractItemDrawable;
class CT_AbstractModel;
class CT_OutAbstractItemAttributeModel;

class DocumentManagerInterface;

class QtColorPicker;
class DM_ItemDrawableManagerOptions;

/**
 * @brief Inherit from this class to get itemdrawable to colorize to the class DM_ContextMenuColouristAdder
 */
class IColouristContextMenuAccess {
public:
    virtual ~IColouristContextMenuAccess() {}

private:
    friend class DM_ContextMenuColouristAdder;

    virtual QList<CT_AbstractItemDrawable*> getItemDrawableToColorize() const = 0;
    virtual QList<CT_AbstractModel*> getSelectedModelsToUseInColorizerMenu() const = 0;
};

/**
 * @brief This tool class can be used to add menu in a context menu to colorize itemdrawable in documents by multiple way
 */
class DM_ContextMenuColouristAdder : public QObject
{
    Q_OBJECT

public:
    struct ActionSetColorByAttribute {
        ActionSetColorByAttribute(): m_model(NULL), m_docIndex(-1) {}
        ActionSetColorByAttribute(CT_OutAbstractItemAttributeModel *m, int docI) : m_model(m), m_docIndex(docI) {}

        CT_OutAbstractItemAttributeModel    *m_model;
        int                                 m_docIndex;
    };

    /**
     * @brief You must inherit from the class IColouristContextMenuAccess to give at the menu the possibility to access itemdrawable to colorize
     */
    DM_ContextMenuColouristAdder(IColouristContextMenuAccess &access, QObject *parent = NULL);
    ~DM_ContextMenuColouristAdder();

    /**
     * @brief Set the document manager to use
     */
    void setDocumentManager(DocumentManagerInterface *docManager);

    /**
     * @brief Set the object that produce automatic color
     */
    void setAutomaticColorProducer(DM_ItemDrawableManagerOptions *autoColorProducer);

public slots:
    /**
     * @brief Set the gradient to use when the user will color itemdrawable by attributes or points by coordinates
     */
    void setGradientToUseForColorization(const QGradient &gradient);
    void setLinearGradientToUseForColorization(const QLinearGradient &gradient);

public:
    /**
     * @brief Add multiple menu to the menu passed in parameter to help user to colorize itemdrawable
     * @param menu : the menu to fill
     */
    void initContextMenu(QMenu *contextMenu);

private:
    IColouristContextMenuAccess     &m_itemAccess;
    DocumentManagerInterface        *m_docManager;
    QtColorPicker                   *m_colorPicker;
    DM_ItemDrawableManagerOptions   *m_autoColorProducer;
    QGradient                       m_gradient;

private slots:
    /**
     * @brief Called when user want to color items by a unique color
     */
    void colorByUniqueColor();

    /**
     * @brief Called when user want to color items by automatic color
     */
    void colorByAutomaticColor();

    /**
     * @brief Called when user want to color items by attributes (use a gradient)
     */
    void colorByAttributeAndGradient();

    /**
     * @brief Called when user want to color points of items by a coordinate
     */
    void colorByPointsCoordinate();
};

Q_DECLARE_METATYPE(DM_ContextMenuColouristAdder::ActionSetColorByAttribute)

#endif // DM_CONTEXTMENUCOLOURISTADDER_H
