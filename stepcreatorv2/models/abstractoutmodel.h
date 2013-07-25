#ifndef ABTRACTOUTMODEL_H
#define ABTRACTOUTMODEL_H

#include "qstandarditemmodel.h"
#include "widgets/abstractoutwidget.h"
#include "qset.h"

class AbstractOutModel : public QStandardItem
{
public:

    enum ModelType
    {
        M_Result_OUT,
        M_Group_OUT,
        M_Item_OUT
    };

    AbstractOutModel();
    ~AbstractOutModel();

    virtual AbstractOutModel::ModelType getModelType() = 0;

    virtual AbstractOutWidget* getWidget();
    virtual QString getName() = 0;
    virtual QString getModelName() = 0;
    QString getDef();

    virtual QString getAlias();
    virtual bool isValid();

    virtual QString getOutModelsDefines();

    static QString getQStringListConcat(QSet<QString> &list);
    virtual void getOutModelsIncludesList(QSet<QString> &list) = 0;

    virtual void getOutItemsTypesIncludesList(QSet<QString> &list) = 0;

    virtual QString getOutModelsDefinition() = 0;
    void getChildrenOutModelsDefinitions(QString &result);

    virtual QString getOutModelsHierachy() = 0;
    void getChildrenOutModelsHierachy(QString &result);
    virtual QString getOutModelAddingCommand() = 0;

    virtual QString getOutComputeBeginning(QString resultDef = "", QString useCopy = "") = 0;
    void getChildrenOutComputeBeginning(QString &result, QString resultDef = "", QString useCopy = "");

    virtual QString getOutComputeLoops(int nbIndent = 1) = 0;

    void onAliasChange();

protected:
    AbstractOutWidget*  _widget;

};

#endif // ABTRACTOUTMODEL_H
