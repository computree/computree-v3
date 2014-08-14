#ifndef CT_TOPFNODEGROUP_H
#define CT_TOPFNODEGROUP_H

#include "ct_itemdrawable/ct_tnodegroup.h"

class CT_OutOPFNodeGroupModel;

/**
 * @brief A class that you can use with a CT_TTreeGroup to create a topology.
 *
 * This class has other method to save elements from a opf file
 */
class PLUGINSHAREDSHARED_EXPORT CT_TOPFNodeGroup : public CT_TNodeGroup
{
public:
    CT_TOPFNodeGroup();

    /**
     * @brief Create a group with a model defined in your step and the result that will contains your ItemDrawable
     *
     * @warning The model and/or the result can be NULL but you must set them with method "setModel()" and "changeResult()" before finish
     *          your step computing !!!
     */
    CT_TOPFNodeGroup(const CT_OutOPFNodeGroupModel *model,
                     const CT_AbstractResult *result);

    /**
     * @brief Create a group with a name of model defined in your step (typically a DEF_...)
     *        and the result that will contains your ItemDrawable
     *
     * @warning The modelName can not be empty and the result can not be NULL to use this constructor
     */
    CT_TOPFNodeGroup(const QString &modelName,
                     const CT_AbstractResult *result);

    /**
      * WARNING : don't forget to copy and inherit this methods if you create a subclass
      */
    QString getType() const;
    static QString staticGetType();

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);

    /**
     * @brief Set ID readed in OPF File
     */
    void setOPFID(const size_t &id);

    /**
     * @brief Set the matrix readed in OPF File
     */
    void setOPFMatrix(const QMatrix4x4 &matrix);

    /**
     * @brief Returns the specific model for CT_TOPFNodeGroup
     */
    CT_OutOPFNodeGroupModel* opfModel() const;
};

#endif // CT_TOPFNODEGROUP_H
