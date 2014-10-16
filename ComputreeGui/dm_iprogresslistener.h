#ifndef DM_IPROGRESSLISTENER_H
#define DM_IPROGRESSLISTENER_H

class DM_IProgressListener
{
public:
    virtual ~DM_IProgressListener() {}

    /**
     * @brief Set the range of progress
     */
    virtual void setProgressRange(const int &min, const int &max) = 0;

    /**
     * @brief Set the progress
     */
    virtual void setProgress(const int &progress) = 0;
};

#endif // DM_IPROGRESSLISTENER_H
