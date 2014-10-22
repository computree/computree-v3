#ifndef DM_FASTESTINCREMENTOPTIMIZER_H
#define DM_FASTESTINCREMENTOPTIMIZER_H

#include <QTime>

/**
 * @brief Optimize the faster increment when draw a point cloud per example
 *        to get the desired fps
 */
class DM_FastestIncrementOptimizer
{
public:
    DM_FastestIncrementOptimizer();

    /**
     * @brief Set the minimum fps to obtain. 15 fps by default.
     */
    void setMinFPS(double fps);

    /**
     * @brief Returns the minimum fps desired
     */
    double minFPS() const;

    /**
     * @brief Returns current FPS calculated
     */
    double currentFPS() const;

    /**
     * @brief Call this method in preDraw method of graphics view
     */
    void preDraw();

    /**
     * @brief Call this method in postDraw method of graphics view
     */
    void postDraw();

    /**
     * @brief Returns the fastest increment optimized
     */
    size_t fastestIncrement() const;

private:
    QTime   m_fpsTimer;
    double  m_desiredFps;
    double  m_currentFps;
    size_t  m_fi;
};

#endif // DM_FASTESTINCREMENTOPTIMIZER_H
