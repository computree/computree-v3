#include <QString>
#include <QtTest>

class ModelTest : public QObject
{
    Q_OBJECT

public:
    ModelTest();

private Q_SLOTS:
    //void initTestCase();
    void testCaseInSingularItemModelCreation();
    void testCaseInGroupModelCreation();
    void testCaseInZeroOrMoreGroupModelCreation();
    void testCaseInResultGroupModelCreation();
    void testCaseInTreeCreation();
    void testCaseOutSingularItemModelCreation();
    void testCaseOutGroupModelCreation();
    void testCaseOutOPFNodeGroupModelCreation();
    void testCaseOutResultGroupModelCreation();
    void testCaseOutTreeCreation();
    void testCaseTreeSearchSimple1();
    void testCaseTreeSearchSimple2();
    void testCaseTreeSearchComplex();
    void testCaseTreeSearchOne();
    void testCaseTreeSearchVeryComplex();
    //void cleanupTestCase();
};
