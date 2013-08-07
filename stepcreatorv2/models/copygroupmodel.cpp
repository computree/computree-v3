#include "copygroupmodel.h"
#include "widgets/copygroupwidget.h"
#include "models/copyitemmodel.h"
#include "tools.h"

COPYGroupModel::COPYGroupModel() : AbstractCopyModel()
{
    _widget = new COPYGroupWidget(this);
    _status = AbstractCopyModel::S_Added;
    setData(QVariant(QColor(Qt::blue)),Qt::ForegroundRole);
    setText(getName());
}

void COPYGroupModel::init(QString alias, QString name, QString desc)
{
    ((COPYGroupWidget*)_widget)->init(alias, name, desc);
    _status = AbstractCopyModel::S_Copy;
    setData(QVariant(QColor(Qt::black)),Qt::ForegroundRole);
    setText(getName() + " (copie)");
}

void COPYGroupModel::init(INGroupModel *inModel)
{
    init(inModel->getAlias(), inModel->getDisplayableName(), inModel->getDescription());
}


QString COPYGroupModel::getName()
{
    if (_status == AbstractCopyModel::S_Added)
    {
        return QString("groupOut_%1").arg(getAlias());
    } else {
        return QString("groupIn_%1").arg(getAlias());
    }
}

QString COPYGroupModel::getModelName()
{
    return QString("groupCopyModel_%1").arg(getAlias());
}


void COPYGroupModel::getActionsIncludes(QSet<QString> &list)
{
    if (_status == AbstractCopyModel::S_Added)
    {
        list.insert("#include \"ct_tools/model/ct_outmodelcopyactionaddmodelgroupingroup.h\"");
    } else if (_status == AbstractCopyModel::S_DeletedCopy)
    {
        list.insert("#include \"ct_tools/model/ct_outmodelcopyactionremovemodelgroupingroup.h\"");
    }

    int size = rowCount();
    for (int i = 0 ; i < size ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        item->getActionsIncludes(list);
    }
}

QString COPYGroupModel::getCopyModelsDefinitions(QString actionName)
{
    QString result = "";


    if (getStatus() == AbstractCopyModel::S_Added)
    {

    } else if (getStatus() == AbstractCopyModel::S_DeletedCopy)
    {

    }

    int count = rowCount();
    for (int i = 0 ; i < count ; i++)
    {
        AbstractCopyModel* item = (AbstractCopyModel*) child(i);
        result += item->getCopyModelsDefinitions(actionName);
    }

    return result;
}

#ifdef TOTO
void toto()
{
    // on récupère le résultat modèle d'entrée à copier défini dans "createInResultModelListProtected()"
    CT_InResultModelGroupToCopy *intResModelToCopy = (CT_InResultModelGroupToCopy*)getInResultModel(DEF_SearchInResult);

    // On créée une liste d'action à executer sur la copie du modèle
    QList<CT_AbstractOutModelCopyAction*> actions;



    // On créée le modèle d'item (CT_ReferencePoint) à ajouter
    CT_OutStandardItemDrawableModel *refPointModel = new CT_OutStandardItemDrawableModel("", new CT_ReferencePoint(), tr("Barycentre"));

    // On ajoute une action permettant d'ajouter le modèle d'item créé
    // Cette action prend en paramètre à générateur de nom automatique : _outRefPointModelName
    actions << new CT_OutModelCopyActionAddModelItemInGroup(DEF_SearchInGroup,
                                                            refPointModel,
                                                            _outRefPointModelName);


    // On ajoute le modèle de sortie modifier
    // En réalité cette méthode va faire effectivement la copie du résultat d'entrée
    addOutResultModelCopy(intResModelToCopy, actions);
}
#endif
