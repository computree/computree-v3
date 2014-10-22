#include "dm_fastestincrementoptimizer.h"

DM_FastestIncrementOptimizer::DM_FastestIncrementOptimizer()
{
    m_desiredFps = 5;
    m_currentFps = 1000;
    m_fi = 1;
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
    double elapsed = m_fpsTimer.elapsed();

    if(elapsed == 0)
        elapsed = 1;

    m_currentFps = 1000.0/elapsed;

    if(((int)m_currentFps) > m_desiredFps) {
        if(m_fi != 1)
            --m_fi;
    }
    else if(((int)m_currentFps) < m_desiredFps){
        ++m_fi;
    }
}

size_t DM_FastestIncrementOptimizer::fastestIncrement() const
{
    return m_fi;
}
