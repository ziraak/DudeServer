#include "main.h"

int main()
{
    int numberOfFailedTests = 0;
    numberOfFailedTests += utils_tests();
    numberOfFailedTests += user_tests();
    numberOfFailedTests += testSuiteCommand(); // achteraan want forked niet

    return (numberOfFailedTests > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}