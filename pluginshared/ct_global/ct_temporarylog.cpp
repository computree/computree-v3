#include "ct_temporarylog.h"

#define CT_IMPLEMENT_NORMAL_MESSAGE void CT_TemporaryLog::addMessage(const int &severity, CT_VirtualAbstractStep *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(severity, LogInterface::step, s, filter); } \
void CT_TemporaryLog::addMessage(const int &severity, CT_AbstractAction *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(severity, LogInterface::action, s, filter); } \
void CT_TemporaryLog::addMessage(const int &severity, PluginEntryInterface *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(severity, LogInterface::plugin, s, filter); } \
void CT_TemporaryLog::addMessage(const int &severity, CT_AbstractStepPlugin *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(severity, LogInterface::plugin, s, filter); } \
void CT_TemporaryLog::addMessage(const int &severity, CT_AbstractResult *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(severity, LogInterface::result, s, filter); } \
void CT_TemporaryLog::addMessage(const int &severity, CT_AbstractItemDrawable *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(severity, LogInterface::itemdrawable, s, filter); }

#define CT_IMPLEMENT_SEVERITY_MESSAGE(_M_TYPE_, SEVTYPE) void CT_TemporaryLog::_M_TYPE_(const int &type, const QString &s, const QString &filter) { addMessage(SEVTYPE, type, s, filter); } \
void CT_TemporaryLog::_M_TYPE_(CT_VirtualAbstractStep *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(SEVTYPE, LogInterface::step, s, filter); } \
void CT_TemporaryLog::_M_TYPE_(CT_AbstractAction *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(SEVTYPE, LogInterface::action, s, filter); } \
void CT_TemporaryLog::_M_TYPE_(PluginEntryInterface *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(SEVTYPE, LogInterface::plugin, s, filter); } \
void CT_TemporaryLog::_M_TYPE_(CT_AbstractStepPlugin *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(SEVTYPE, LogInterface::plugin, s, filter); } \
void CT_TemporaryLog::_M_TYPE_(CT_AbstractResult *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(SEVTYPE, LogInterface::result, s, filter); } \
void CT_TemporaryLog::_M_TYPE_(CT_AbstractItemDrawable *type, const QString &s, const QString &filter) { Q_UNUSED(type) addMessage(SEVTYPE, LogInterface::itemdrawable, s, filter); }

CT_TemporaryLog::CT_TemporaryLog()
{
}

void CT_TemporaryLog::addMessage(const int &severity, const int &type, const QString &s, const QString &filter)
{
    CT_TemporaryLogMessage m;
    m.m_severity = severity;
    m.m_type = type;
    m.m_message = s;
    m.m_filter = filter;

    m_messages.append(m);
}

CT_IMPLEMENT_NORMAL_MESSAGE

CT_IMPLEMENT_SEVERITY_MESSAGE(addTraceMessage, LogInterface::trace)
CT_IMPLEMENT_SEVERITY_MESSAGE(addDebugMessage, LogInterface::debug)
CT_IMPLEMENT_SEVERITY_MESSAGE(addInfoMessage, LogInterface::info)
CT_IMPLEMENT_SEVERITY_MESSAGE(addWarningMessage, LogInterface::warning)
CT_IMPLEMENT_SEVERITY_MESSAGE(addErrorMessage, LogInterface::error)
CT_IMPLEMENT_SEVERITY_MESSAGE(addFatalMessage, LogInterface::fatal)

const QList<CT_TemporaryLogMessage> &CT_TemporaryLog::messages() const
{
    return m_messages;
}
