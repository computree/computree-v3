#include "tst_modeltest.h"

int main( int argc, char *argv[])
{
    ModelTest tc;
    QTest::qExec (&tc, argc, argv );
    /*Modele2Tests tc2 ;
    QTest::qExec (&tc2, argc, argv );*/
    return 0;
}
