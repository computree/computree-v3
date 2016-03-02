#ifndef CT_MODELSEARCHHELPER_H
#define CT_MODELSEARCHHELPER_H

#include "ct_model/outModel/abstract/ct_outabstractmodel.h"
#include "ct_result/abstract/ct_abstractresult.h"

class CT_OutAbstractSingularItemModel;
class CT_OutAbstractItemAttributeModel;

/**
 * @brief Use this class to find a model in a step by differents parameters. This class offers the
 *        ability to accelerate the process because it will cache models previously founded to retrieve them
 *        faster.
 */
class PLUGINSHAREDSHARED_EXPORT CT_ModelSearchHelper : public QObject
{
    Q_OBJECT

public:
    typedef QMultiHash<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> SplitHash;

    CT_ModelSearchHelper();
    ~CT_ModelSearchHelper();

    /**
     * @brief Use this method if you want to search a model when you create an ItemDrawable (Group or Item)
     *        or an ItemAttribute
     * @param modelUniqueName : the unique name of the model
     * @param outResult : the result where the model must be
     */
    CT_OutAbstractModel* searchModelForCreation(const QString &outModelUniqueName,
                                                const CT_AbstractResult *outResult);

    /**
     * @brief Search the model corresponding to the uniqueName passed in parameter in step passed in parameter for the
     *        result passed in parameter.
     *
     * @param inOrOutModelUniqueName : unique name of the model that you search (typically DEF_InXXX or Def_OutXXX)
     * @param inOrOutResult : the result where the model must be
     * @param yourStep : your step (typically 'this')
     */
    CT_AbstractModel* searchModel(const QString &inOrOutModelUniqueName,
                                  const CT_AbstractResult *inOrOutResult,
                                  const CT_VirtualAbstractStep *yourStep);

    /**
     * @brief Search the model corresponding to the uniqueName passed in parameter in step passed in parameter for the
     *        result passed in parameter.
     *
     * @param inOrOutModelUniqueName : unique name of the model that you search (typically DEF_InXXX or Def_OutXXX)
     * @param inOrOutResultModel : the model of the result
     * @param yourStep : your step (typically 'this')
     */
    CT_AbstractModel* searchModel(const QString &inOrOutModelUniqueName,
                                  const CT_OutAbstractResultModel *inOrOutResultModel,
                                  const CT_VirtualAbstractStep *yourStep);

    /**
     * @brief Returns a hashmap : the key is a singulat item model and values (multi value) was model of attributes selected by the user for this singular item model.
     * @param inAttributeModelUniqueName : unique name of the attribute model (typically DEF_InXXX) that you want to split by singular item model
     * @param inSingularItemModelUniqueName : unique name of the singular item model (typically DEF_InXXX) that we must use to split attributes model
     * @param inResultModel : the model of the result
     * @param yourStep : your step (typically 'this')
     * @return
     */
    SplitHash splitSelectedAttributesModelBySelectedSingularItemModel(const QString &inAttributeModelUniqueName,
                                                                      const QString &inSingularItemModelUniqueName,
                                                                      const CT_OutAbstractResultModel *inResultModel,
                                                                      const CT_VirtualAbstractStep *yourStep);

    /**
     * @brief Remove cache for the result passed in parameter
     */
    void removeCacheForResult(const CT_AbstractResult *res);

    /**
     * @brief Remove cache for the result model passed in parameter
     */
    void removeCacheForResultModel(const CT_OutAbstractResultModel *resM);

    /**
     * @brief Clear the cache
     */
    void clearCache();

private:
    QHash<CT_OutAbstractResultModel*, QHash<QString, CT_AbstractModel*>* >      m_cache;

    CT_AbstractModel* getInCache(const CT_OutAbstractResultModel *resultModel, const QString &modelUniqueName, QHash<QString, CT_AbstractModel *> **outHash);

private slots:
    void resultModelDeleted(QObject *o);
};

#endif // CT_MODELSEARCHHELPER_H
