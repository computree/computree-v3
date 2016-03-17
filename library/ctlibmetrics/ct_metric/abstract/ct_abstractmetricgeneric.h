#ifndef CT_ABSTRACTMETRICGENERIC_H
#define CT_ABSTRACTMETRICGENERIC_H

#include "ctlibmetrics/ctlibmetrics_global.h"
#include "ctlibmetrics/tools/ct_valueandbool.h"

// from pluginshared
#include "ct_metric/abstract/ct_abstractmetric.h"
#include "ct_view/ct_genericconfigurablewidget.h"

class CTLIBMETRICS_EXPORT CT_AbstractMetricGeneric : public CT_AbstractMetric
{
    Q_OBJECT

public:
    CT_AbstractMetricGeneric() : CT_AbstractMetric() {}
    CT_AbstractMetricGeneric(const CT_AbstractMetricGeneric &other)  : CT_AbstractMetric(other) {}
    ~CT_AbstractMetricGeneric();

    /**
     * @brief Declare a attribute that you will set in your compute method. Call this method in your constructor.
     * @example : registerAttributeVaB(m_classMemberThatWillBeComputedInComputeMetrics, CT_AbstractCategory::DATA_DIMENSION, tr("my attribute that was nice"));
     * @param classMember : a variable member of your class that will be used in "computeMetrics" method.
     * @param categoryName : the name of the category of the attribute (example : CT_AbstractCategory::DATA_AREA)
     * @param displayableName : a displayable name for this attribute (displayed in gui), must not be empty !
     */
    template<typename VType>
    void registerAttributeVaB(const VaB<VType> &classMember, const QString &categoryName, const QString &displayableName) {
        if(!m_vab.contains((CT_AbstractVaB*)&classMember)) {
            m_vab.append((CT_AbstractVaB*)&classMember);
            m_attributesVaB.insert((void*)&classMember, new CT_AbstractMetric::AttributeObject<VType>(displayableName, categoryName, NULL));
        }
    }

    /**
     * @brief Call setAttribute(....) in your "compute" method to set results of attributes declared with "addAttribute" method
     * @param classMemberAndValue : the variable member of your class that was filled with the good value and that you used in method "addAttribute"
     */
    template<typename VType>
    void setAttributeValueVaB(VaB<VType> &classMemberAndValue) {
        if(classMemberAndValue.used)
            setAttributeValue<VType>(classMemberAndValue.value);
    }

    /**
     * @brief Returns default settings if you have used "addAttributeVaB" method
     */
    virtual SettingsNodeGroup* getAllSettings() const;

    /**
     * @brief Set default settings if you have used "addAttributeVaB" method
     */
    virtual bool setAllSettings(const SettingsNodeGroup *settings);

    /**
     * @brief Returns a default widget to choose attributes to compute if you have used "addAttributeVaB" method
     */
    virtual CT_AbstractConfigurableWidget* createConfigurationWidget();

private:
    QList<CT_AbstractVaB*>  m_vab;
    AttributesContainer     m_attributesVaB;

protected:
    /**
     * @brief Inherit this method to add your unselectable attributes
     * @warning Don't forget to call this method ! CT_AbstractMetricGeneric::createAttributes()
     */
    void createAttributes();
};

#endif // CT_ABSTRACTMETRICGENERIC_H
