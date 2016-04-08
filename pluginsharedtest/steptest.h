#ifndef STEPTEST_H
#define STEPTEST_H

#include <QtGui>
#include <QtTest>

#include "ct_abstractstepplugin.h"
#include "ct_global/ct_temporarylog.h"

class TestPluginManager : public CT_AbstractStepPlugin
{
public:
    QString getPluginURL() const { return ""; }

protected:
    bool loadGenericsStep() { return true; }
    bool loadOpenFileStep() { return true; }
    bool loadCanBeAddedFirstStep()  { return true; }
    bool loadActions()  { return true; }
    bool loadExporters()  { return true; }
    bool loadReaders()  { return true; }
};

class TestCore : public CoreInterface
{
public:

    /**
     * @brief get the log
     */
    virtual LogInterface* log() const { return (LogInterface*)&m_log; }

    /**
     * @brief get the plugin manager
     */
    virtual PluginManagerInterface* pluginManager() const { return m_pluginManager; }

    /**
     * @brief get the menu to populate with steps
     */
    virtual CT_StepsMenu* stepsMenu() const { return (CT_StepsMenu*)&m_menu; }

    CT_TemporaryLog         m_log;
    PluginManagerInterface  *m_pluginManager;
    CT_StepsMenu            m_menu;
};

class StepTest : public QObject
{
    Q_OBJECT

public:
    StepTest();

private Q_SLOTS:
    //void initTestCase();
    void testStep();
    //void cleanupTestCase();
};

#endif // STEPTEST_H
