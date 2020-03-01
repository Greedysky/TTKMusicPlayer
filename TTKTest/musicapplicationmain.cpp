#include "musicautotest.h"

#if 1
// This is all you need to run all the tests
TEST_MAIN
#else
// Or supply your own main function
int main(int argc, char *argv[])
{
    const int failures = AutoTest::run(argc, argv);
    if(failures == 0)
    {
       TTK_LOGGER_INFO("ALL TESTS PASSED");
    }
    else
    {
        TTK_LOGGER_INFO(failures << " TESTS FAILED!");
    }
    return app.exec();
}
#endif
