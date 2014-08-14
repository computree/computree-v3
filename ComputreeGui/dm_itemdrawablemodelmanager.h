#ifndef DM_ITEMDRAWABLEMODELMANAGER_H
#define DM_ITEMDRAWABLEMODELMANAGER_H

class Result;

class DM_ItemDrawableModelManager
{
public:
    DM_ItemDrawableModelManager();
    virtual ~DM_ItemDrawableModelManager();

    virtual void setResult(const Result *res) = 0;
};

#endif // DM_ITEMDRAWABLEMODELMANAGER_H
