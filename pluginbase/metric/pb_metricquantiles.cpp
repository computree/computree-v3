#include "pb_metricquantiles.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_iterator/ct_pointiterator.h"

PB_MetricQuantiles::PB_MetricQuantiles() : CT_AbstractMetric_XYZ()
{
    _typeAsString = "Hmax";
    updateName();
}

PB_MetricQuantiles::PB_MetricQuantiles(const PB_MetricQuantiles *other) : CT_AbstractMetric_XYZ(other)
{
    updateName();
}

void PB_MetricQuantiles::createConfigurationDialog()
{
    CT_StepConfigurableDialog* configDialog = addConfigurationDialog();

    configDialog->addString(tr("Hmax"), "", _typeAsString);
}

void PB_MetricQuantiles::updateParamtersAfterConfiguration()
{
    updateName();
}

void PB_MetricQuantiles::updateName()
{
    _name = QString("PBM_%1").arg(_typeAsString);
}

QString PB_MetricQuantiles::getParametersAsString() const
{
    QString result = "";
    result.append(_typeAsString);

    return result;
}

bool PB_MetricQuantiles::setParametersFromString(QString parameters)
{
    _typeAsString = parameters;
    return true;
}

void PB_MetricQuantiles::createAttributes()
{
    addAttribute("Hmax", CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_Z);
}

void PB_MetricQuantiles::computeMetric()
{
    double hmax = -std::numeric_limits<double>::max();
    CT_PointIterator itP(_inCloud);
    while(itP.hasNext())
    {
        const CT_Point& point = itP.next().currentPoint();

        if (point(2) > hmax) {hmax = point(2);}
    }
    setAttributeValue("Hmax", hmax);
}

QString PB_MetricQuantiles::getShortDescription() const
{
    return tr("Compute quantiles on Z coordinate");
}

QString PB_MetricQuantiles::getDetailledDescription() const
{
    return tr("");
}

CT_AbstractConfigurableElement *PB_MetricQuantiles::copy() const
{
    PB_MetricQuantiles* metric = new PB_MetricQuantiles(this);
    metric->_typeAsString = _typeAsString;

    return metric;
}

