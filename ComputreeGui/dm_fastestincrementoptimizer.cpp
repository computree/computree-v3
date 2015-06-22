#include "dm_fastestincrementoptimizer.h"

DM_FastestIncrementOptimizer::DM_FastestIncrementOptimizer()
{
    m_desiredFps = 5;
    m_currentFps = 1000;
    m_accumulatedFps = 0;
    m_fi = 1;
    m_nMeasure = 0;
    m_maxNbMeasure = 6;
}

void DM_FastestIncrementOptimizer::setMinFPS(double fps)
{
    m_desiredFps = fps;
}

double DM_FastestIncrementOptimizer::minFPS() const
{
    return m_desiredFps;
}

double DM_FastestIncrementOptimizer::currentFPS() const
{
    return m_currentFps;
}

void DM_FastestIncrementOptimizer::preDraw()
{
    m_fpsTimer.restart();
}

void DM_FastestIncrementOptimizer::postDraw()
{
    ++m_nMeasure;

    double elapsed = m_fpsTimer.elapsed();

    if(elapsed == 0)
        elapsed = 1;

    m_accumulatedFps += (1000.0/elapsed);

    if(m_nMeasure >= m_maxNbMeasure) {
        m_currentFps = m_accumulatedFps/(double)m_nMeasure;
        m_accumulatedFps = 0;
        m_nMeasure = 0;

        if(((int)m_currentFps) > m_desiredFps) {
            --m_fi;

            if(m_fi <= 0)
                m_fi = 1;
        }
        else if((((int)m_currentFps)+5) < m_desiredFps){
            //m_fi += (m_desiredFps - m_currentFps);
            ++m_fi;
        }
    }
}

size_t DM_FastestIncrementOptimizer::fastestIncrement() const
{
    return m_fi;
}
