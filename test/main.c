#include "main.h"

int main()
{
    int numberOfFailedTests = 0;
    //numberOfFailedTests += unitTests();
    performanceMeting();


    return (numberOfFailedTests > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int unitTests()
{
    int numberOfFailedTests = 0;
    //numberOfFailedTests += utils_tests();
    //numberOfFailedTests += user_tests();
    //numberOfFailedTests += channel_tests();
    //numberOfFailedTests += testSuiteCommand(); // achteraan want forked niet
    return numberOfFailedTests;
}

int performanceMeting()
{
    int durationTestsInMsec = 0;
    int i;
    int averageDurationTestsInMsec = 0;
    int amountOfCommandTestRuns = 10;
    int numberOfCommandTestSuiteRun = 10;
    for (i = 0; i < numberOfCommandTestSuiteRun; i++)
    {
        durationTestsInMsec += testCommandsPerformanceMeting(amountOfCommandTestRuns);
    }
    averageDurationTestsInMsec = durationTestsInMsec / numberOfCommandTestSuiteRun;

    printf("Number of commands tests run: %i\n", amountOfCommandTestRuns);
    printf("Number of command test suites run: %i\n", numberOfCommandTestSuiteRun);
    printf("Average duration of %i commands suite run %i times in msec: %i\n" , numberOfCommandTestSuiteRun, amountOfCommandTestRuns, averageDurationTestsInMsec);

    return averageDurationTestsInMsec;
}