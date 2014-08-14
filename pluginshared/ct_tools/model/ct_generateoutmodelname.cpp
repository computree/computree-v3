#include "ct_generateoutmodelname.h"

#include "ct_turn/inTurn/tools/ct_inturnmanager.h"
#include "ct_outabstractitemmodel.h"
#include "ct_outabstractresultmodel.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

CT_GenerateOutModelName::CT_GenerateOutModelName()
{
}

QString CT_GenerateOutModelName::getNewModelNameThatDontExistIn(CT_InTurnManager &turnManager,
                                                                const QList<QString> &modelsNameAlreadyGenerated,
                                                                QString myFavoriteName) const
{
    QString letters;
    int index = 0;
    char letter = 'a';

    QString modelName = myFavoriteName;

    if(modelName.isEmpty())
        modelName = generateNextName(index, letter, letters);

    do
    {
        bool found = true;

        if(!modelsNameAlreadyGenerated.contains(modelName))
        {
            found = false;

            QListIterator<CT_InTurn*> itT(turnManager.turns());

            while(itT.hasNext()
                  && !found)
            {
                found = existInTurn(itT.next(), modelName);
            }
        }

        if(!found)
            return modelName;

        modelName = generateNextName(index, letter, letters);
    }while(1);

    return "";
}

// PRIVATE //

bool CT_GenerateOutModelName::existInTurn(CT_InTurn *turn, const QString &modelName) const
{
    QListIterator<CT_InAbstractResultModel*> itM(turn->models());

    while(itM.hasNext())
    {
        CT_InAbstractResultModel *model = itM.next();

        QList<CT_InStdModelPossibility*> possibilities = model->getPossibilitiesSavedSelected();
        QListIterator<CT_InStdModelPossibility*> itP(possibilities);

        while(itP.hasNext())
        {
            CT_InStdModelPossibility *possibility = itP.next();
            QList<CT_AbstractModel*> models = possibility->outModel()->recursiveGetAllModels();

            QListIterator<CT_AbstractModel*> itM2(models);

            while(itM2.hasNext())
            {
                if(itM2.next()->uniqueName() == modelName)
                    return true;
            }
        }
    }

    return false;
}

QString CT_GenerateOutModelName::generateNextName(int &index, char &letter, QString &letters) const
{
    if(index >= 26)
    {
        index = 0;
        letter = 'a';
        letters.append('a');
    }
    else
    {
        letter = ('a' + index);
    }

    ++index;

    return letters + letter;
}
