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
    void testCaseTreeSearchSimple3();
    void testCaseTreeSearchSimple4();
    void testCaseTreeSearchSimple1Copy();
    void testCaseTreeSearchSimple1ItemAttribute();
    void testCaseTreeSearchSimple2ItemAttribute();
    void testCaseTreeSearchSimple3ItemAttribute();
    void testCaseTreeSearchSimple4ItemAttribute();
    void testCaseTreeSearchComplex();
    void testCaseTreeSearchOne();
    void testCaseTreeSearchVeryComplex();
    void testCaseTreeSearchSimple1Optional();
    void testCaseTreeSearchComplexOptional();
    //void cleanupTestCase();
};
