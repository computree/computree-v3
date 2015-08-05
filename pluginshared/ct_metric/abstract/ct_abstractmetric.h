#ifndef CT_ABSTRACTMETRIC_H
#define CT_ABSTRACTMETRIC_H

#include "ct_filter/abstract/ct_abstractconfigurableelement.h"
#include "ct_attributes/abstract/ct_abstractitemattribute.h"
#include "ct_tools/model/ct_autorenamemodels.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"


class PLUGINSHAREDSHARED_EXPORT CT_AbstractMetric : public CT_AbstractConfigurableElement
{
    Q_OBJECT
public:

    enum AttributeType
    {
        AT_bool,
        AT_double,
        AT_float,
        AT_int,
        AT_size_t,
        AT_QString
    };

    CT_AbstractMetric();
    CT_AbstractMetric(const CT_AbstractMetric* other);

    ~CT_AbstractMetric();

    void initAttributesModels(CT_OutResultModelGroupToCopyPossibilities* resPoss, const CT_AutoRenameModels &parentModel);

    void computeMetric(CT_AbstractSingularItemDrawable *item);

protected:
    struct AttributeObject {

        AttributeObject() {}
        AttributeObject(QString name,
                        CT_AbstractMetric::AttributeType type,
                        QString category,
                        CT_AutoRenameModels* autoRenameModel)
        {
            _name = name;
            _type = type;
            _category = category;
            _autoRenameModel = autoRenameModel;
            _instance = NULL;
        }

        AttributeObject(const AttributeObject &other)
        {
            _name = other._name;
            _type = other._type;
            _category = other._category;
            _autoRenameModel = other._autoRenameModel;
            _instance = NULL;
        }

        QString                                 _name;
        CT_AbstractMetric::AttributeType        _type;
        QString                                 _category;
        CT_AutoRenameModels*                    _autoRenameModel;
        CT_AbstractItemAttribute*               _instance;
    };

    CT_ResultGroup*                   _result;
    QMap<QString, AttributeObject*> _attributes;


    virtual void createAttributes() = 0;
    virtual void computeMetric() = 0;


    void addAttribute(QString name, CT_AbstractMetric::AttributeType type, QString category);

    void setAttributeValue(QString attributeName, bool &value);
    void setAttributeValue(QString attributeName, double &value);
    void setAttributeValue(QString attributeName, float &value);
    void setAttributeValue(QString attributeName, int &value);
    void setAttributeValue(QString attributeName, size_t &value);
    void setAttributeValue(QString attributeName, QString &value);

private:
    AttributeObject *getAttributeObject(QString attributeName, CT_AbstractMetric::AttributeType type);
};

#endif // CT_AbstractMetric_H
