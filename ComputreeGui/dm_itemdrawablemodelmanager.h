#ifndef DM_ITEMDRAWABLEMODELMANAGER_H
#define DM_ITEMDRAWABLEMODELMANAGER_H

class CT_AbstractResult;

class DM_ItemDrawableModelManager
{
public:
    DM_ItemDrawableModelManager();
    virtual ~DM_ItemDrawableModelManager();

    virtual void setResult(const CT_AbstractResult *res) = 0;
};

#endif // DM_ITEMDRAWABLEMODELMANAGER_H
