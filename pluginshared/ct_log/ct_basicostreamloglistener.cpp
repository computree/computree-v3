#include "ct_basicostreamloglistener.h"

CT_BasicOStreamLogListener::CT_BasicOStreamLogListener() : CT_AbstractLogListener()
{
    m_strm = &std::cout;
}

void CT_BasicOStreamLogListener::setBasicOStream(std::basic_ostream<char> &strm)
{
    m_strm = &strm;
}

void CT_BasicOStreamLogListener::addMessage(const int &severity, const int &type, const QString &s, const QString &filter)
{
    if(acceptMessage(severity, type, filter))
        (*m_strm) << s.toLatin1().data();
}
