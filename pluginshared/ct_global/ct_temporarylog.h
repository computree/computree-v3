#ifndef CT_TEMPORARYLOG_H
#define CT_TEMPORARYLOG_H

#include "interfaces.h"
#include "pluginShared_global.h"

#define CT_DECLARE_NORMAL_MESSAGE void addMessage(const int &severity, const int &type, const QString &s, const QString &filter = ""); \
void addMessage(const int &severity, CT_VirtualAbstractStep *type, const QString &s, const QString &filter = ""); \
void addMessage(const int &severity, CT_AbstractAction *type, const QString &s, const QString &filter = ""); \
void addMessage(const int &severity, PluginEntryInterface *type, const QString &s, const QString &filter = ""); \
void addMessage(const int &severity, CT_AbstractStepPlugin *type, const QString &s, const QString &filter = ""); \
void addMessage(const int &severity, CT_AbstractResult *type, const QString &s, const QString &filter = ""); \
void addMessage(const int &severity, CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "");

#define CT_DECLARE_SEVERITY_MESSAGE(_M_TYPE_) void _M_TYPE_(const int &type, const QString &s, const QString &filter = ""); \
void _M_TYPE_(CT_VirtualAbstractStep *type, const QString &s, const QString &filter = ""); \
void _M_TYPE_(CT_AbstractAction *type, const QString &s, const QString &filter = ""); \
void _M_TYPE_(PluginEntryInterface *type, const QString &s, const QString &filter = ""); \
void _M_TYPE_(CT_AbstractStepPlugin *type, const QString &s, const QString &filter = ""); \
void _M_TYPE_(CT_AbstractResult *type, const QString &s, const QString &filter = ""); \
void _M_TYPE_(CT_AbstractItemDrawable *type, const QString &s, const QString &filter = "");

class PLUGINSHAREDSHARED_EXPORT CT_TemporaryLogMessage
{
public:
    int         m_severity;
    int         m_type;
    QString     m_message;
    QString     m_filter;
};

class PLUGINSHAREDSHARED_EXPORT CT_TemporaryLog : public LogInterface
{
    Q_INTERFACES(LogInterface)

public:
    CT_TemporaryLog();

    CT_DECLARE_NORMAL_MESSAGE

    CT_DECLARE_SEVERITY_MESSAGE(addTraceMessage)
    CT_DECLARE_SEVERITY_MESSAGE(addDebugMessage)
    CT_DECLARE_SEVERITY_MESSAGE(addInfoMessage)
    CT_DECLARE_SEVERITY_MESSAGE(addWarningMessage)
    CT_DECLARE_SEVERITY_MESSAGE(addErrorMessage)
    CT_DECLARE_SEVERITY_MESSAGE(addFatalMessage)

    void addPrioritaryLogListener(const CT_AbstractLogListener *logl) { Q_UNUSED(logl) }
    void addNormalLogListener(const CT_AbstractLogListener *logl) { Q_UNUSED(logl) }
    void removeLogListener(CT_AbstractLogListener *logl) { Q_UNUSED(logl) }

    const QList<CT_TemporaryLogMessage>& messages() const;

private:
    QList<CT_TemporaryLogMessage>   m_messages;
};

#endif // CT_TEMPORARYLOG_H
