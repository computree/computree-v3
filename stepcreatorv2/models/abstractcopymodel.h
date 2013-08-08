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
    virtual QString getDisplayableName();
    QString getDef();

    virtual QString getAlias();
    virtual QString getAutoRenameName();
    virtual bool isValid();
    inline AbstractCopyModel::Status getStatus() {return _status;}

    bool copyIncludesNeeded();
    virtual void getActionsIncludes(QSet<QString> &list) = 0;
    QString getAutoRenamesDeclarations();
    virtual QString getCopyModelsDefinitions(QString actionName = "") = 0;
    virtual QString getCopyComputeLoops(int nbIndent = 1, QString resultName = "") = 0;
    virtual QString getCopyModelDoc(int nbIndent = 0) = 0;


    void setDeleted();
    void setNotDeleted();

    void onAliasChange();

protected:
    AbstractCopyWidget*  _widget;
    AbstractCopyModel::Status   _status;

};

#endif // ABTRACTCOPYMODEL_H
