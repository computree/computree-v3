#ifndef STEPTEST_H
#define STEPTEST_H

#include <QtGui>
#include <QtTest>

#include "ct_abstractstepplugin.h"

class TextPluginManager : public CT_AbstractStepPlugin
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
