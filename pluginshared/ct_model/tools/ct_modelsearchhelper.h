#ifndef CT_MODELSEARCHHELPER_H
#define CT_MODELSEARCHHELPER_H

#include "ct_model/outModel/abstract/ct_outabstractmodel.h"
#include "ct_result/abstract/ct_abstractresult.h"

/**
 * @brief Use this class to find a model in a step by differents parameters. This class offers the
 *        ability to accelerate the process because it will cache models previously founded to retrieve them
 *        faster.
 */
class PLUGINSHAREDSHARED_EXPORT CT_ModelSearchHelper : public QObject
{
    Q_OBJECT

public:
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
     * @brief Remove cache for the result passed in parameter
     */
    void removeCacheForResult(const CT_AbstractResult *res);

    /**
     * @brief Clear the cache
     */
    void clearCache();

private:
    QHash<CT_AbstractResult*, QHash<QString, CT_AbstractModel*>* >   m_cache;

    CT_AbstractModel* getInCache(const CT_AbstractResult *result, const QString &modelUniqueName, QHash<QString, CT_AbstractModel *> **outHash);

private slots:
    void resultDeleted(QObject *o);
};

#endif // CT_MODELSEARCHHELPER_H
