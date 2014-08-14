#ifndef CT_BASICOSTREAMLOGLISTENER_H
#define CT_BASICOSTREAMLOGLISTENER_H

#include "ct_log/abstract/ct_abstractloglistener.h"

#include <iostream>

/**
 * @brief A log listener that can redirect message to a std::basic_ostream like std::cout
 */
class PLUGINSHAREDSHARED_EXPORT CT_BasicOStreamLogListener : public CT_AbstractLogListener
{
public:
    CT_BasicOStreamLogListener();

    /**
     * @brief Set the stream
     * @param strm : a reference to the stream
     * @example setBasicOStream(std::cout)
     */
    void setBasicOStream(std::basic_ostream<char> &strm);

    /**
     * @brief Called when a message is added to the log manager.
     */
    void addMessage(const int &severity, const int &type, const QString &s, const QString &filter);

private:
    std::basic_ostream<char> *m_strm;
};

#endif // CT_BASICOSTREAMLOGLISTENER_H
