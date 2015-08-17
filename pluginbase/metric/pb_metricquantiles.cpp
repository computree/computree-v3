#include "pb_metricquantiles.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_iterator/ct_pointiterator.h"
#include "ct_math/ct_mathstatistics.h"

PB_MetricQuantiles::PB_MetricQuantiles() : CT_AbstractMetric_XYZ()
{
    _quantMin = 0.05;
    _quantMax = 0.95;
    _quantStep = 0.05;
    _prefix = "H";
    _hmin = true;
    _hmed = true;
    _h99 = true;
    _hmax = true;
}

PB_MetricQuantiles::PB_MetricQuantiles(const PB_MetricQuantiles *other) : CT_AbstractMetric_XYZ(other)
{
}

QString PB_MetricQuantiles::getName()
{
    return QString("PB_Quantiles");
}

void PB_MetricQuantiles::createConfigurationDialog()
{
    CT_StepConfigurableDialog* configDialog = addConfigurationDialog();

    configDialog->addTitle(tr("Paramétrage des quantiles à calculer :"));
    configDialog->addDouble(tr("- A partir de"), "%", 0, 100, 0, _quantMin, 100);
    configDialog->addDouble(tr("- Jusqu'à"), "%", 0, 100, 0, _quantMax, 100);
    configDialog->addDouble(tr("- Avec un pas de"), "%", 0, 100, 0, _quantStep, 100);
    configDialog->addString(tr("Préfixe à utiliser (ex : P99, Q99, H99, Z99,...)"), "", _prefix);
    configDialog->addEmpty();
    configDialog->addTitle(tr("Métriques complémentaires :"));
    configDialog->addBool(tr("Minimum (Hmin ~ H00)"), "", "", _hmin);
    configDialog->addBool(tr("Médiane (Hmed ~ H50)"), "", "", _hmed);
    configDialog->addBool(tr("Percentile 99  (H99)"), "", "",  _h99);
    configDialog->addBool(tr("Maximum (Hmax ~ H100)"), "", "", _hmax);
}

void PB_MetricQuantiles::updateParamtersAfterConfiguration()
{
}

QString PB_MetricQuantiles::getParametersAsString() const
{
    QString result = "";
    result.append(QString("%4;%1;%2;%3;%5;%6;%7;%8")).arg(_quantMin).arg(_quantMax).arg(_quantStep).arg(_prefix).arg((int)_hmin).arg((int)_hmed).arg((int)_h99).arg((int)_hmax);
    return result;
}

bool PB_MetricQuantiles::setParametersFromString(QString parameters)
{
    QStringList values = parameters.split(";");
    if (values.size() < 8) {return false;}
    bool ok1, ok2, ok3, ok5, ok6, ok7, ok8;

    _quantMin  = values.at(0).toDouble(&ok1);
    _quantMax  = values.at(1).toDouble(&ok2);
    _quantStep = values.at(2).toDouble(&ok3);
    _prefix = values.at(3); if (_prefix.isEmpty()) {_prefix = "H";}
    _hmin = (bool) values.at(4).toInt(&ok5);
    _hmed = (bool) values.at(5).toInt(&ok6);
    _h99  = (bool) values.at(6).toInt(&ok7);
    _hmax = (bool) values.at(7).toInt(&ok8);

    return (ok1 && ok2 && ok3 && ok5 && ok6 && ok7 && ok8);
}

void PB_MetricQuantiles::createAttributes()
{    
    int nb = (_quantMax - _quantMin) / _quantStep + 1;
    for (int i = 0 ; i <= nb ; i++)
    {
        addAttribute(getQuantileString(_quantMin + i*_quantStep), CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_NUMBER);
    }

    for (double i = _quantMin ; i <= _quantMax ; i += _quantStep)
    {
        addAttribute(getQuantileString(i), CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_NUMBER);
    }

    if (_hmin) {addAttribute("Hmin", CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_NUMBER);}
    if (_hmed) {addAttribute("Hmed", CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_NUMBER);}
    if (_h99 ) {addAttribute("H99",  CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_NUMBER);}
    if (_hmax) {addAttribute("Hmax", CT_AbstractMetric::AT_double, CT_AbstractCategory::DATA_NUMBER);}
}

void PB_MetricQuantiles::computeMetric()
{
    QList<double> values;

    CT_PointIterator itP(_inCloud);
    while(itP.hasNext())
    {
        const CT_Point& point = itP.next().currentPoint();

        if (_plotArea->contains(point(0), point(1))) {values.append(point(2));}
    }

    qSort(values.begin(), values.end());

    double val;
    int nb = (_quantMax - _quantMin) / _quantStep + 1;
    for (int i = 0 ; i <= nb ; i++)
    {
        double quant = _quantMin + i*_quantStep;
        val = CT_MathStatistics::computeQuantile(values, quant, false);
        setAttributeValue(getQuantileString(quant), val);
    }

    if (_hmin) {val = values.first();                                           setAttributeValue("Hmin", val);}
    if (_hmed) {val = CT_MathStatistics::computeQuantile(values, 0.50, false);  setAttributeValue("Hmed", val);}
    if (_h99)  {val = CT_MathStatistics::computeQuantile(values, 0.99, false);  setAttributeValue("H99",  val);}
    if (_hmax) {val = values.last();                                            setAttributeValue("Hmax", val);}
}

QString PB_MetricQuantiles::getShortDescription() const
{
    return tr("Calcul de quantiles sur les coordonnées Z des points");
}

QString PB_MetricQuantiles::getDetailledDescription() const
{
    return tr("");
}

CT_AbstractConfigurableElement *PB_MetricQuantiles::copy() const
{
    PB_MetricQuantiles* metric = new PB_MetricQuantiles(this);

    metric->_quantMin  = _quantMin;
    metric->_quantMax  = _quantMax;
    metric->_quantStep = _quantStep;
    metric->_prefix    = _prefix;
    metric->_hmin      = _hmin;
    metric->_hmed      = _hmed;
    metric->_h99       = _h99;
    metric->_hmax      = _hmax;

    return metric;
}

QString PB_MetricQuantiles::getQuantileString(double quantile)
{
    QString number = QString::number(100.0*quantile, 'f', 0);
    if (number.size() <2) {number.prepend("0");}
    return QString("%1%2").arg(_prefix).arg(number);
}
