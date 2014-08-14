#include "ct_generateoutresultmodelname.h"

#include "ct_outresultmodelmanager.h"
#include "ct_outabstractitemmodel.h"
#include "ct_outabstractresultmodel.h"

CT_GenerateOutResultModelName::CT_GenerateOutResultModelName()
{
}

QString CT_GenerateOutResultModelName::getNewResultModelNameThatDontExistIn(CT_OutResultModelManager &outResultModelManager,
                                                                            const QList<QString> &resultModelsNameAlreadyGenerated,
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

        if(!resultModelsNameAlreadyGenerated.contains(modelName))
            found = existInList(outResultModelManager.models(), modelName);

        if(!found)
            return modelName;

        modelName = generateNextName(index, letter, letters);
    }while(1);

    return "";
}

// PRIVATE //

bool CT_GenerateOutResultModelName::existInList(const QList<CT_OutAbstractResultModel*> &list, const QString &modelName) const
{
    QListIterator<CT_OutAbstractResultModel*> itM(list);

    while(itM.hasNext())
    {
        if(itM.next()->uniqueName() == modelName)
            return true;
    }

    return false;
}

QString CT_GenerateOutResultModelName::generateNextName(int &index, char &letter, QString &letters) const
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
