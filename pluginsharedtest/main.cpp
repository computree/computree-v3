#include "tst_modeltest.h"
#include "cloudtest.h"
#include "itemdrawabletest.h"

int main( int argc, char *argv[])
{
    ModelTest mt;
    QTest::qExec (&mt, argc, argv );
    CloudTest ct;
    QTest::qExec (&ct, argc, argv );
    ItemDrawableTest idt ;
    QTest::qExec (&idt, argc, argv );

    CT_Context::deleteStaticInstance();

    return 0;
}
