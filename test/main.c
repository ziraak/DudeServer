#include "main.h"

int main()
{
    int numberOfFailedTests = 0;
    //numberOfFailedTests += unitTests();
    performanceMetingCommandsTests();


    return (numberOfFailedTests > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int unitTests()
{
    int numberOfFailedTests = 0;
    numberOfFailedTests += utils_tests();
    numberOfFailedTests += user_tests();
    numberOfFailedTests += channel_tests();
    numberOfFailedTests += testSuiteCommand(); // achteraan want forked niet
    return numberOfFailedTests;
}

int performanceMetingCommandsTests()
{
    int durationTestsInMsec = 0;
    int i;
    int averageDurationTestsInMsec = 0;
    int amountOfCommandLoops = 10;
    int numberOfCommandTestSuiteRun = 10;
    for (i = 0; i < numberOfCommandTestSuiteRun; i++)
    {
        durationTestsInMsec += testCommandsPerformanceMeting(amountOfCommandLoops);
    }
    averageDurationTestsInMsec = durationTestsInMsec / (numberOfCommandTestSuiteRun * amountOfCommandLoops);

    printf("Number of command loops in a test suite: %i\n", amountOfCommandLoops);
    printf("Number of test suites run: %i\n", numberOfCommandTestSuiteRun);
    printf("Every suite ran all commands %i times. Average duration each set of commands of %i commands suite(s) in msec: %i\n" , amountOfCommandLoops, numberOfCommandTestSuiteRun,
           averageDurationTestsInMsec);

    return averageDurationTestsInMsec;
}