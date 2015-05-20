#include "main.h"

int main()
{
    int numberOfFailedTests = 0;
//    numberOfFailedTests += utils_tests();
//    numberOfFailedTests += testSuiteCommand();
    numberOfFailedTests += user_tests();
//    numberOfFailedTests += channel_tests();

    return (numberOfFailedTests > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}