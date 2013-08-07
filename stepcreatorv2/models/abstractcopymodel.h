#ifndef ABTRACTCOPYMODEL_H
#define ABTRACTCOPYMODEL_H

#include "qstandarditemmodel.h"
#include "widgets/abstractcopywidget.h"
#include "qset.h"

class AbstractCopyModel : public QStandardItem
{
public:

    enum ModelType
    {
        M_Result_COPY,
        M_Group_COPY,
        M_Item_COPY
    };

    enum Status
    {
        S_Copy,
        S_DeletedCopy,
        S_Added
    };

    AbstractCopyModel();
    ~AbstractCopyModel();

    virtual AbstractCopyModel::ModelType getModelType() = 0;

    AbstractCopyWidget* getWidget();
    virtual QString getName() = 0;
    virtual QString getModelName() = 0;
    QString getDef();

    virtual QString getAlias();
    virtual bool isValid();
    inline AbstractCopyModel::Status getStatus() {return _status;}

    void setDeleted();
    void setNotDeleted();

    virtual QString getCopyModelsDefines();

    static QString getQStringListConcat(QSet<QString> &list);
    virtual void getCopyModelsIncludesList(QSet<QString> &list) = 0;

    virtual void getCopyItemsTypesIncludesList(QSet<QString> &list) = 0;

    virtual QString getCopyModelsDefinition() = 0;
    void getChildrenCopyModelsDefinitions(QString &result);

    virtual QString getCopyModelsHierachy() = 0;
    void getChildrenCopyModelsHierachy(QString &result);
    virtual QString getCopyModelAddingCommand() = 0;

    virtual QString getCopyComputeBeginning(int rank = 0, QString resultName = "") = 0;
    void getChildrenCopyComputeBeginning(QString &result, QString resultName = "");

    virtual QString getCopyComputeItemsCreations(QString resultName = "") = 0;

    void onAliasChange();

protected:
    AbstractCopyWidget*  _widget;
    AbstractCopyModel::Status   _status;

};

#endif // ABTRACTCOPYMODEL_H
