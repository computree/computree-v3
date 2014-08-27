#include "tst_modeltest.h"
#include "cloudtest.h"
#include "steptest.h"

int main( int argc, char *argv[])
{
    ModelTest mt;
    QTest::qExec (&mt, argc, argv );
    CloudTest ct;
    QTest::qExec (&ct, argc, argv );
    StepTest st ;
    QTest::qExec (&st, argc, argv );

    CT_Context::deleteStaticInstance();

    return 0;
}
