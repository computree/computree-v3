#include "metric/pb_metriccomputestats.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_iterator/ct_pointiterator.h"

PB_MetricComputeStats::PB_MetricComputeStats() : CT_AbstractMetric_XYZ()
{
    _computeHmean = true;
    _computeHsd = true;
    _computeHskew = true;
    _computeHkurt = true;
    _computeHcv  = true;
}

PB_MetricComputeStats::PB_MetricComputeStats(const PB_MetricComputeStats *other) : CT_AbstractMetric_XYZ(other)
{
}

QString PB_MetricComputeStats::getName()
{
    return QString("PB_ComputeStats");
}

void PB_MetricComputeStats::createConfigurationDialog()
{
    CT_StepConfigurableDialog* configDialog = addConfigurationDialog();

    configDialog->addBool("Hmean", "", "", _computeHmean);
    configDialog->addBool("Hsd"  , "", "", _computeHsd);
    configDialog->addBool("Hskew", "", "", _computeHskew);
    configDialog->addBool("Hkurt", "", "", _computeHkurt);
    configDialog->addBool("HCV"  , "", "", _computeHcv);
}

void PB_MetricComputeStats::updateParamtersAfterConfiguration()
{
}

QString PB_MetricComputeStats::getParametersAsString() const
{
    QString result = "";
    return result;
}

bool PB_MetricComputeStats::setParametersFromString(QString parameters)
{
    return true;
}

void PB_MetricComputeStats::createAttributes()
{
    addAttribute("N", CT_AbstractMetric::AT_size_t, CT_AbstractCategory::DATA_NUMBER);
    if (_computeHmean) {addAttribute("Hmean", CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_Z);}
    if (_computeHsd)   {addAttribute("Hsd", CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_Z);}
    if (_computeHskew) {addAttribute("Hskew", CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_Z);}
    if (_computeHkurt) {addAttribute("Hkurt", CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_Z);}
    if (_computeHcv)   {addAttribute("Hcv", CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_NUMBER);}
}

void PB_MetricComputeStats::computeMetric()
{    
    double valHmean = 0;
    double valHsd = 0;
    double valHskew = 0;
    double valHkurt = 0;
    double valHcv = 0;
    double m2 = 0;
    double m3 = 0;
    double m4 = 0;
    size_t n = 0;

    CT_PointIterator itP(_inCloud);
    while(itP.hasNext())
    {
        const CT_Point& point = itP.next().currentPoint();

        if (_plotArea->contains(point(0), point(1)))
        {
            n++;
            if (_computeHmean || _computeHsd   || _computeHskew || _computeHkurt || _computeHcv) {valHmean += point(2);}
        }
    }

    if (n > 1)
    {
        double nd = (double)n;
        if (_computeHmean || _computeHsd   || _computeHskew || _computeHkurt || _computeHcv)
        {
            valHmean /= nd;
        }


        if (_computeHskew || _computeHkurt)
        {
            CT_PointIterator itP2(_inCloud);
            while(itP2.hasNext())
            {
                const CT_Point& point = itP2.next().currentPoint();

                if (_plotArea->contains(point(0), point(1)))
                {
                    double val = point(2) - valHmean;
                    if (_computeHsd || _computeHskew || _computeHkurt) {m2 += pow(val, 2.0);}
                    if (_computeHskew)                                 {m3 += pow(val, 3.0);}
                    if (_computeHkurt)                                 {m4 += pow(val, 4.0);}
                }
            }
            m2 /= nd;
            m3 /= nd;
            m4 /= nd;

            if (_computeHsd || _computeHcv)
            {
                valHsd = sqrt((nd/(nd-1))*m2);
            }

            if (_computeHcv)
            {
                valHcv = valHsd / valHmean;
            }

            if (_computeHskew)
            {
                valHskew = m3 / pow(m2, 3.0/2.0);
                valHskew *= sqrt(nd*(nd-1.0))/(nd-2.0); // SAS & SPSS
                //valHskew *= pow((nd-1.0)/nd, 3.0/2.0); // R package e1071

            }

            if (_computeHkurt)
            {
                valHkurt = m4 / pow(m2, 2.0) - 3.0;
                valHkurt = ((nd+1.0)*valHkurt + 6.0)*(nd-1.0)/((nd-2.0)*(nd-3.0)); // SAS & SPSS
                //valHkurt = (valHkurt + 3)*pow(1.0-1.0/nd, 2.0) - 3.0; // R package e1071
            }
        }
    } else {
        valHsd = 0;
        valHskew = 0;
        valHkurt = 0;
        valHcv = 0;
    }


    setAttributeValue("N", n);
    if (_computeHmean) {setAttributeValue("Hmean", valHmean);}
    if (_computeHsd)   {setAttributeValue("Hsd", valHsd);}
    if (_computeHskew) {setAttributeValue("Hskew", valHskew);}
    if (_computeHkurt) {setAttributeValue("Hkurt", valHkurt);}
    if (_computeHcv)   {setAttributeValue("Hcv", valHcv);}
}

QString PB_MetricComputeStats::getShortDescription() const
{
    return tr("Calcul des indicateurs statistiques standard");
}

QString PB_MetricComputeStats::getDetailledDescription() const
{
    return tr("");
}

CT_AbstractConfigurableElement *PB_MetricComputeStats::copy() const
{
    PB_MetricComputeStats* metric = new PB_MetricComputeStats(this);
    metric->_computeHmean = _computeHmean;
    metric->_computeHsd   = _computeHsd;
    metric->_computeHskew = _computeHskew;
    metric->_computeHkurt = _computeHkurt;
    metric->_computeHcv   = _computeHcv;

    return metric;
}
