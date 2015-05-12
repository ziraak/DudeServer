#include "main.h"

int main()
{
    int numberOfFailedTests = 0;
    //numberOfFailedTests += utils_tests();
    numberOfFailedTests += testSuiteCommand();

    return (numberOfFailedTests > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}