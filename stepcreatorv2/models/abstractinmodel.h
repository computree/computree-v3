#ifndef ABTRACTINMODEL_H
#define ABTRACTINMODEL_H

#include "qstandarditemmodel.h"
#include "widgets/abstractinwidget.h"
#include "qset.h"

class AbstractInModel : public QStandardItem
{
public:

    enum ModelType
    {
        M_Result_IN,
        M_Group_IN,
        M_Item_IN
    };

    AbstractInModel();
    ~AbstractInModel();

    virtual AbstractInModel::ModelType getModelType() = 0;

    virtual AbstractInWidget* getWidget();
    virtual QString getName() = 0;
    virtual QString getModelName() = 0;
    QString getDef();

    virtual QString getAlias();
    virtual bool isValid();

    virtual QString getInModelsDefines();

    virtual void getInModelsIncludesList(QSet<QString> &list) = 0;

    virtual void getInItemsTypesIncludesList(QSet<QString> &list) = 0;

    virtual QString getInModelsDefinition() = 0;
    void getChildrenInModelsDefinitions(QString &result);

    virtual QString getInModelsHierachy() = 0;
    void getChildrenInModelsHierachy(QString &result);
    virtual QString getInModelAddingCommand() = 0;

    virtual QString getInComputeBeginning(QString resultDef = "", QString useCopy = "") = 0;
    void getChildrenInComputeBeginning(QString &result, QString resultDef = "", QString useCopy = "");

    virtual QString getInComputeLoops(int nbIndent = 1) = 0;

    void onAliasChange();

protected:
    AbstractInWidget*  _widget;

};

#endif // ABTRACTINMODEL_H
