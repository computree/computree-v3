#include "ct_cloudmetrics.h"

#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_iterator/ct_pointiterator.h"

// percentiles must be in ascending order (25 and 75 must be kept to compute the interquartileDistance !)
int CT_CloudMetrics::PERCENTILE_COEFF[CT_CloudMetrics::PERCENTILE_ARRAY_SIZE] = {1, 5, 10, 20, 25, 30, 40, 50, 60, 70, 75, 80, 90, 95, 99};

CT_CloudMetrics::CT_CloudMetrics() : CT_AbstractMetric_XYZ()
{
    declareAttributes();
    m_configAndResults.minZ = -1;
}

CT_CloudMetrics::CT_CloudMetrics(const CT_CloudMetrics &other)  : CT_AbstractMetric_XYZ(other)
{
    declareAttributes();
    m_configAndResults = other.m_configAndResults;
}

CT_CloudMetrics::Config CT_CloudMetrics::metricConfiguration() const
{
    return m_configAndResults;
}

void CT_CloudMetrics::setMetricConfiguration(const CT_CloudMetrics::Config &conf)
{
    m_configAndResults = conf;
}

SettingsNodeGroup *CT_CloudMetrics::getAllSettings() const
{
    SettingsNodeGroup *root = CT_AbstractMetricGeneric::getAllSettings();

    SettingsNodeGroup *v = new SettingsNodeGroup("MinZ");
    v->addValue(new SettingsNodeValue("value", m_configAndResults.minZ));
    root->addGroup(v);

    return root;
}

bool CT_CloudMetrics::setAllSettings(const SettingsNodeGroup *settings)
{
    if(settings == NULL)
        return false;

    if(settings->name() == "CT_AbstractMetricGeneric")
    {
        QList<SettingsNodeGroup*> groups = settings->groupsByTagName("MinZ");
        SettingsNodeGroup *v = groups.first();

        SettingsNodeValue *usedV = v->firstValueByTagName("value");
        if(usedV == NULL) {return false;}
        m_configAndResults.minZ = usedV->value().toDouble();

        return CT_AbstractMetricGeneric::setAllSettings(settings);
    }

    return false;
}

QString CT_CloudMetrics::getShortDescription() const
{
    return tr("Calcul des indicateurs statistiques standards (FUSION)");
}

void CT_CloudMetrics::computeMetric()
{
    if(attributes().isEmpty())
        return;

    initValues();

    size_t nPoints = 0;
    double zValue;
    double tmp;
    const CT_AreaShape2DData *area = plotArea();
    std::vector<double> zValues(pointCloud()->size());

    CT_PointIterator it(pointCloud());

    while(it.hasNext())
    {
        const CT_Point& point = it.next().currentPoint();

        if(((area == NULL) || area->contains(point(CT_Point::X), point(CT_Point::Y))) && (point(CT_Point::Z) >= m_configAndResults.minZ))
        {
            zValue = point(CT_Point::Z);

            m_configAndResults.minimum.value = qMin(m_configAndResults.minimum.value, zValue);
            m_configAndResults.maximum.value = qMax(m_configAndResults.maximum.value, zValue);
            m_configAndResults.mean.value += zValue;
            tmp = zValue*zValue;
            m_configAndResults.elevationQuadraticMean.value += tmp;
            m_configAndResults.elevationCubicMean.value += tmp*zValue;
            zValues[nPoints] = zValue;

            ++nPoints;
        }
    }

    if(nPoints > 0) {
        double nD = nPoints;

        // resize to fit nPoints
        zValues.resize(nPoints);

        // sort Z values
        std::sort(zValues.begin(), zValues.end());

        m_configAndResults.mean.value /= nD;

        size_t percentile50Index = 0;

        for(int i=0; i<CT_CloudMetrics::PERCENTILE_ARRAY_SIZE; ++i) {
            double coeff = CT_CloudMetrics::PERCENTILE_COEFF[i];
            m_configAndResults.percentileValues[i].value = computePercentile(zValues, nPoints, (coeff/100.0));

            // percentile coeff is in ascending order
            if(coeff == 25)
                m_configAndResults.interquartileDistance.value = m_configAndResults.percentileValues[i].value;
            else if(coeff == 75)
                m_configAndResults.interquartileDistance.value = m_configAndResults.percentileValues[i].value - m_configAndResults.interquartileDistance.value;
            else if(coeff == 50)
                percentile50Index = i;
        }

        m_configAndResults.mode.value = computeMode(zValues, nPoints);
        std::vector<double> modeValues(nPoints);
        std::vector<double> medianValues(nPoints);

        // second pass to compute variance, etc...
        double medianValue = m_configAndResults.percentileValues[percentile50Index].value;
        double CL1, CR1, CL2, CL3, CR2, CR3;

        for(size_t index=0; index<nPoints; ++index) {
            zValue = zValues[index];

            double diff = zValue - m_configAndResults.mean.value;

            m_configAndResults.aad.value += std::fabs(diff);

            tmp = diff*diff; // pow(diff, 2)
            m_configAndResults.variance.value += tmp;

            tmp *= diff; // pow(diff, 3)
            m_configAndResults.skewness.value += tmp;

            m_configAndResults.kurtosis.value += (tmp*diff);  // pow(diff, 4)

            medianValues[index] = fabs(zValue - medianValue);
            modeValues[index] = fabs(zValue - m_configAndResults.mode.value);

            CL1 = index;
            CL2 = CL1*((CL1-1.0)/2.0);
            CL3 = CL2*((CL1-2.0)/3.0);
            CR1 = nPoints-(index+1);
            CR2 = CR1*((CR1-1.0)/2.0);
            CR3 = CR2*((CR1-2.0)/3.0);

            m_configAndResults.lMoments[0].value += zValue;
            m_configAndResults.lMoments[1].value += ((CL1 - CR1) * zValue);
            m_configAndResults.lMoments[2].value += ((CL2 - (2.0*CL1*CR1) + CR2) * zValue);
            m_configAndResults.lMoments[3].value += ((CL3 - (3.0*CL2*CR1) + (3.0*CL1*CR2) - CR3) * zValue);
        }

        CL1 = nPoints;
        CL2 = CL1*((CL1-1.0)/2.0);
        CL3 = CL2*((CL1-2.0)/3.0);
        CR1 = CL3*((CL1-3.0)/4.0);

        double doubleInfinity = std::numeric_limits<double>::infinity();

        m_configAndResults.lMoments[0].value /= CL1;

        if(CL2 != 0)
            m_configAndResults.lMoments[1].value /= (CL2*2.0); // same as (m_configAndResults.lMoments[1].value/CL2)/2.0
        else
            m_configAndResults.lMoments[1].value = doubleInfinity;

        if(CL3 != 0)
            m_configAndResults.lMoments[2].value /= (CL3*3.0); // same as (m_configAndResults.lMoments[1].value/CL3)/3.0
        else
            m_configAndResults.lMoments[2].value = doubleInfinity;

        if(CR1 != 0)
            m_configAndResults.lMoments[3].value /= (CR1*4.0); // same as (m_configAndResults.lMoments[1].value/CR1)/4.0
        else
            m_configAndResults.lMoments[3].value = doubleInfinity;

        if((m_configAndResults.lMoments[0].value != 0) && (CL2 != 0))
            m_configAndResults.lMomentsCoeffOfVariation.value = m_configAndResults.lMoments[1].value/m_configAndResults.lMoments[0].value;
        else
            m_configAndResults.lMomentsCoeffOfVariation.value = doubleInfinity;

        if((CL2 != 0) && (CL3 != 0))
            m_configAndResults.lMomentsSkewness.value = m_configAndResults.lMoments[2].value/m_configAndResults.lMoments[1].value;
        else
            m_configAndResults.lMomentsSkewness.value = doubleInfinity;

        if((CL2 != 0) && (CR1 != 0))
            m_configAndResults.lMomentsKurtosis.value = m_configAndResults.lMoments[3].value/m_configAndResults.lMoments[1].value;
        else
            m_configAndResults.lMomentsKurtosis.value = doubleInfinity;

        std::sort(medianValues.begin(), medianValues.end());
        std::sort(modeValues.begin(), modeValues.end());

        m_configAndResults.madMedian.value = computePercentile(medianValues, nPoints, 0.5);
        m_configAndResults.madMode.value = computePercentile(modeValues, nPoints, 0.5);

        if(nPoints > 1) {
            m_configAndResults.variance.value /= (nD-1);
            m_configAndResults.standardDeviation.value = std::sqrt(m_configAndResults.variance.value);
            m_configAndResults.skewness.value /= ( (nD-1) * (std::pow(m_configAndResults.standardDeviation.value, 3)));
            m_configAndResults.kurtosis.value /= ( (nD-1) * (std::pow(m_configAndResults.standardDeviation.value, 4)));

            if(m_configAndResults.mean.value != 0)
                m_configAndResults.coeffOfVariation.value = m_configAndResults.standardDeviation.value / m_configAndResults.mean.value;
            else
                m_configAndResults.coeffOfVariation.value = doubleInfinity;
        } else {
            m_configAndResults.variance.value = doubleInfinity;
            m_configAndResults.standardDeviation.value = doubleInfinity;
            m_configAndResults.skewness.value = doubleInfinity;
            m_configAndResults.kurtosis.value = doubleInfinity;
            m_configAndResults.coeffOfVariation.value = doubleInfinity;
        }

        m_configAndResults.aad.value /= nD;
        m_configAndResults.elevationQuadraticMean.value /= nD;
        m_configAndResults.elevationCubicMean.value /= nD;
        m_configAndResults.elevationQuadraticMean.value = std::sqrt(m_configAndResults.elevationQuadraticMean.value);
        m_configAndResults.elevationCubicMean.value = std::pow(m_configAndResults.elevationCubicMean.value, 1.0/3.0);
        tmp = (m_configAndResults.maximum.value - m_configAndResults.minimum.value);

        if(tmp != 0)
            m_configAndResults.canopyReliefRatio.value = (m_configAndResults.mean.value - m_configAndResults.minimum.value) / tmp;
        else
            m_configAndResults.canopyReliefRatio.value = doubleInfinity;
    }

    setAttributeValueVaB(m_configAndResults.minimum);
    setAttributeValueVaB(m_configAndResults.maximum);
    setAttributeValueVaB(m_configAndResults.mean);
    setAttributeValueVaB(m_configAndResults.mode);
    setAttributeValueVaB(m_configAndResults.standardDeviation);
    setAttributeValueVaB(m_configAndResults.variance);
    setAttributeValueVaB(m_configAndResults.coeffOfVariation);
    setAttributeValueVaB(m_configAndResults.interquartileDistance);
    setAttributeValueVaB(m_configAndResults.skewness);
    setAttributeValueVaB(m_configAndResults.kurtosis);
    setAttributeValueVaB(m_configAndResults.aad);
    setAttributeValueVaB(m_configAndResults.madMedian);
    setAttributeValueVaB(m_configAndResults.madMode);

    for(int i=0; i<4; ++i)
        setAttributeValueVaB(m_configAndResults.lMoments[i]);

    setAttributeValueVaB(m_configAndResults.lMomentsCoeffOfVariation);
    setAttributeValueVaB(m_configAndResults.lMomentsSkewness);
    setAttributeValueVaB(m_configAndResults.lMomentsKurtosis);

    for(int i=0; i<CT_CloudMetrics::PERCENTILE_ARRAY_SIZE; ++i)
        setAttributeValueVaB(m_configAndResults.percentileValues[i]);

    setAttributeValueVaB(m_configAndResults.canopyReliefRatio);
    setAttributeValueVaB(m_configAndResults.elevationQuadraticMean);
    setAttributeValueVaB(m_configAndResults.elevationCubicMean);
}

void CT_CloudMetrics::initValues()
{
    m_configAndResults.minimum.value = std::numeric_limits<double>::max();
    m_configAndResults.maximum.value = -m_configAndResults.minimum.value;
    m_configAndResults.mean.value = 0;
    m_configAndResults.mode.value = 0;
    m_configAndResults.standardDeviation.value = 0;
    m_configAndResults.variance.value = 0;
    m_configAndResults.coeffOfVariation.value = 0;
    m_configAndResults.interquartileDistance.value = 0;
    m_configAndResults.skewness.value = 0;
    m_configAndResults.kurtosis.value = 0;
    m_configAndResults.aad.value = 0;
    m_configAndResults.madMedian.value = 0;
    m_configAndResults.madMode.value = 0;

    for(int i=0; i<4; ++i)
        m_configAndResults.lMoments[i].value = 0;

    m_configAndResults.lMomentsCoeffOfVariation.value = 0;
    m_configAndResults.lMomentsSkewness.value = 0;
    m_configAndResults.lMomentsKurtosis.value = 0;

    for(int i=0; i<15; ++i)
        m_configAndResults.percentileValues[i].value = 0;

    m_configAndResults.canopyReliefRatio.value = 0;
    m_configAndResults.elevationQuadraticMean.value = 0;
    m_configAndResults.elevationCubicMean.value = 0;
}

double CT_CloudMetrics::computePercentile(const std::vector<double> &array, const size_t &arraySize, const double &p)
{
    // Second Variant, show wikipedia "Percentile"
    double v = ((double)(arraySize-1)) * p;
    int ip1 = (int)v;
    double f = (v-ip1); // (arraySize-1)*p = ip1+f

    int ip2 = ip1 + 1;

    if(ip2 == arraySize)
        return array[ip1];

    if(f == 0)
        return array[ip1];

    return array[ip1] + (f * (array[ip2] - array[ip1]));
}

double CT_CloudMetrics::computeMode(const std::vector<double> &array, const size_t &arraySize)
{
    if(arraySize == 1)
            return array[0];

    double step = (array[arraySize-1] - array[0]) / 64.0;

    std::vector<double> classes(65);
    classes[0] = array[0];

    for(size_t i=1; i<65; ++i)
        classes[i] = classes[i-1]+step;

    std::vector<size_t> res(64, 0);

    size_t j = 0;

    for(size_t i=0; i<arraySize; ++i)
    {
        // if there was rounding problem we can have j > 64
        // or if step == 0
        while((array[i] > classes[j]) && (j<64))
            ++j;

        res[j] += 1;
    }

    size_t maxOccurence = res[0];
    size_t maxOccurenceIndex = 0;

    for(size_t i=1; i<64; ++i) {
        if(res[i] > maxOccurence) {
            maxOccurence = res[i];
            maxOccurenceIndex = i;
        }
    }

    return (classes[maxOccurenceIndex] + classes[maxOccurenceIndex + 1]) / 2.0;

//    if(maxOccurenceIndex > 0) {
//        std::vector<double>::const_iterator it = std::upper_bound(array.begin(), array.end(), classes[maxOccurenceIndex-1]);

//        if(it == array.end())
//            return array[arraySize-1];

//        return (*it);
//    }

//    std::vector<double>::const_iterator it = std::lower_bound(array.begin(), array.end(), classes[maxOccurenceIndex]);

//    if(it == array.end())
//        return array[arraySize-1];

//    return (*it);
}

CT_AbstractConfigurableElement *CT_CloudMetrics::copy() const
{
    return new CT_CloudMetrics(*this);
}

CT_AbstractConfigurableWidget* CT_CloudMetrics::createConfigurationWidget()
{
    CT_GenericConfigurableWidget *wid = new CT_GenericConfigurableWidget();

    wid->addDouble(tr("Ne conserver que les points avec Z >= "), "m", -1e+10, 1e+10, 4, m_configAndResults.minZ);
    wid->addEmpty();

    addAllVaBToWidget(wid);

    return wid;
}


void CT_CloudMetrics::declareAttributes()
{
    /*registerAttributeVaB(m_configAndResults.totalNumberOfReturns, CT_AbstractCategory::DATA_NUMBER, tr("Total return"));

    for(int i=0; i<9; ++i)
        registerAttributeVaB(m_configAndResults.countOfReturnsByReturnNumber[i], CT_AbstractCategory::DATA_NUMBER, tr("Return %1 count").arg(i+1));
*/
    registerAttributeVaB(m_configAndResults.minimum, CT_AbstractCategory::DATA_Z, tr("Elev minimum"));
    registerAttributeVaB(m_configAndResults.maximum, CT_AbstractCategory::DATA_Z, tr("Elev maximum"));
    registerAttributeVaB(m_configAndResults.mean, CT_AbstractCategory::DATA_Z, tr("Elev mean"));
    registerAttributeVaB(m_configAndResults.mode, CT_AbstractCategory::DATA_Z, tr("Elev mode"));
    registerAttributeVaB(m_configAndResults.standardDeviation, CT_AbstractCategory::DATA_Z, tr("Elev stddev"));
    registerAttributeVaB(m_configAndResults.variance, CT_AbstractCategory::DATA_Z, tr("Elev variance"));
    registerAttributeVaB(m_configAndResults.coeffOfVariation, CT_AbstractCategory::DATA_Z, tr("Elev CV"));
    registerAttributeVaB(m_configAndResults.interquartileDistance, CT_AbstractCategory::DATA_Z, tr("Elev IQ"));
    registerAttributeVaB(m_configAndResults.skewness, CT_AbstractCategory::DATA_Z, tr("Elev skewness"));
    registerAttributeVaB(m_configAndResults.kurtosis, CT_AbstractCategory::DATA_Z, tr("Elev kurtosis"));
    registerAttributeVaB(m_configAndResults.aad, CT_AbstractCategory::DATA_Z, tr("Elev AAD"));
    registerAttributeVaB(m_configAndResults.madMedian, CT_AbstractCategory::DATA_Z, tr("Elev MAD median"));
    registerAttributeVaB(m_configAndResults.madMode, CT_AbstractCategory::DATA_Z, tr("Elev MAD mode"));

    for(int i=0; i<CT_CloudMetrics::LMOMENTS_ARRAY_SIZE; ++i)
        registerAttributeVaB(m_configAndResults.lMoments[i], CT_AbstractCategory::DATA_Z, tr("Elev L%1").arg(i+1));

    registerAttributeVaB(m_configAndResults.lMomentsCoeffOfVariation, CT_AbstractCategory::DATA_Z, tr("Elev L CV"));
    registerAttributeVaB(m_configAndResults.lMomentsSkewness, CT_AbstractCategory::DATA_Z, tr("Elev L skewness"));
    registerAttributeVaB(m_configAndResults.lMomentsKurtosis, CT_AbstractCategory::DATA_Z, tr("Elev L kurtosis"));

    for(int i=0; i<CT_CloudMetrics::PERCENTILE_ARRAY_SIZE; ++i)
        registerAttributeVaB(m_configAndResults.percentileValues[i], CT_AbstractCategory::DATA_Z, tr("Elev P%1%2").arg((PERCENTILE_COEFF[i] < 10 ? "0" : "")).arg(PERCENTILE_COEFF[i]));

    registerAttributeVaB(m_configAndResults.canopyReliefRatio, CT_AbstractCategory::DATA_Z, tr("Canopy relief ratio"));
    registerAttributeVaB(m_configAndResults.elevationQuadraticMean, CT_AbstractCategory::DATA_Z, tr("Elev SQRT mean SQ"));
    registerAttributeVaB(m_configAndResults.elevationCubicMean, CT_AbstractCategory::DATA_Z, tr("Elev CURT mean CUBE"));
}
