#ifndef CT_INSTDRESULTMODELPOSSIBILITY_H
#define CT_INSTDRESULTMODELPOSSIBILITY_H

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

class CT_InAbstractModel;

/**
 * @brief Represent a possibility for a INPUT model that represent a result.
 *
 * Each possibility of a model that represent a result contains an INPUT model that was a copy of the root model
 * of the result model. This root model (CT_InAbstractModel) contains all possibilities found in childrens
 * of the OUPUT model (result) of this possibility.
 */
class PLUGINSHAREDSHARED_EXPORT CT_InStdResultModelPossibility : public CT_InStdModelPossibility
{
public:
    CT_InStdResultModelPossibility();
    ~CT_InStdResultModelPossibility();

    /**
     * @brief Set the INPUT model. This model will be deleted from memory by this class.
     */
    void setInModel(CT_InAbstractModel *model);

    /**
     * @brief Returns the INPUT model
     */
    CT_InAbstractModel* inModel() const;

private:
    CT_InAbstractModel  *m_inModel;
};

#endif // CT_INSTDRESULTMODELPOSSIBILITY_H
