#include "ct_outstdsingularitemmodel.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

CT_OutStdSingularItemModel::CT_OutStdSingularItemModel(const QString &uniqueName,
                                                       CT_AbstractSingularItemDrawable *item,
                                                       const QString &displayableName,
                                                       const QString &description) : DEF_CT_AbstractItemDrawableModelOut(uniqueName,
                                                                                                                         description,
                                                                                                                         displayableName.isEmpty() ? item->name() : displayableName)
{
    setItem(item);
}

CT_OutStdSingularItemModel::~CT_OutStdSingularItemModel()
{
    qDeleteAll(m_autoRenameOfDefaultIA.begin(), m_autoRenameOfDefaultIA.end());
}

QString CT_OutStdSingularItemModel::modelTypeDisplayable() const
{
    return QString("CT_OutStdSingularItemModel");
}

CT_OutAbstractModel* CT_OutStdSingularItemModel::copy() const
{
    CT_OutStdSingularItemModel *cpy = new CT_OutStdSingularItemModel(uniqueName(),
                                                                     NULL,
                                                                     displayableName(),
                                                                     description());

    QListIterator<CT_OutAbstractItemAttributeModel*> itI(itemAttributes());

    while(itI.hasNext())
    {
        CT_OutAbstractItemAttributeModel *iaModel = itI.next();
        cpy->internalAddItemAttribute((CT_OutAbstractItemAttributeModel*)iaModel->copy());
    }

    if(itemDrawable() != NULL)
        cpy->setItem(itemDrawable()->copy(cpy, result(), CT_ResultCopyModeList()));

    cpy->setOriginalModel(this);

    return cpy;
}

bool CT_OutStdSingularItemModel::setComplete()
{
    if((item() != NULL) && m_autoRenameOfDefaultIA.isEmpty())
    {
        // get default item attributes of this item drawable
        QList<CT_AbstractItemAttribute*> l = PS_DIAM->itemAttributes(itemDrawable()->getType());
        QListIterator<CT_OutAbstractItemAttributeModel*> itI(itemAttributes());

        while(itI.hasNext()) {
            CT_OutAbstractItemAttributeModel *iaModel = itI.next();

            QMutableListIterator<CT_AbstractItemAttribute*> it(l);

            while(it.hasNext())
            {
                CT_AbstractItemAttribute *att = it.next();

                if(iaModel->originalModel() == att->model()->originalModel())
                    it.remove();
            }
        }

        QListIterator<CT_AbstractItemAttribute*> it(l);

        while(it.hasNext())
        {
            CT_AbstractItemAttribute *att = it.next();

            // get the model of this attribute
            CT_OutAbstractItemAttributeModel *attModel = att->model();

            // copy the model
            CT_OutAbstractItemAttributeModel *cpy = (CT_OutAbstractItemAttributeModel*)attModel->copy();

            CT_AutoRenameModels *autoRename = new CT_AutoRenameModels();

            // add it to this model with the auto rename object of the original model
            if(!addItemAttribute(cpy, *autoRename))
            {
                delete autoRename;
                return false;
            }

            m_autoRenameOfDefaultIA.append(autoRename);
        }
    }

    return DEF_CT_AbstractItemDrawableModelOut::setComplete();
}
