#include "tst_modeltest.h"
#include "cloudtest.h"

int main( int argc, char *argv[])
{
    ModelTest mt;
    QTest::qExec (&mt, argc, argv );
    CloudTest ct;
    QTest::qExec (&ct, argc, argv );
    /*Modele2Tests tc2 ;
    QTest::qExec (&tc2, argc, argv );*/

    CT_Context::deleteStaticInstance();

    return 0;
}
