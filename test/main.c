#include "main.h"

int main()
{
    int numberOfFailedTests = 0;
    //numberOfFailedTests += unitTests();
    //performanceMetingCommandsTests();
    performanceMetingServer();


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

int performanceMetingServer()
{
    int durationTestsInMsec = 0;
    int i;
    int averageDurationTestsInMsec = 0;
    int amountOfTestLoops = 1;
    int numberOfTestSuiteRun = 1;
    for (i = 0; i < numberOfTestSuiteRun; i++)
    {
        durationTestsInMsec += testServerPerformanceMeting(amountOfTestLoops);
    }
    averageDurationTestsInMsec = durationTestsInMsec / (numberOfTestSuiteRun * amountOfTestLoops);

    printf("Number of test loops in a test suite: %i\n", amountOfTestLoops);
    printf("Number of server test suites run: %i\n", numberOfTestSuiteRun);
    printf("Every suite ran all commands %i times. Average duration each set of tests of %i server suite(s) in msec: %i\n" ,
           amountOfTestLoops,
           numberOfTestSuiteRun,
           averageDurationTestsInMsec);

    return averageDurationTestsInMsec;
}

int getDurationTest(struct timeb start_time)
{
    struct timeb end_time;
    ftime(&end_time);
    return (int) ((end_time.time - start_time.time) * 1000) + (end_time.millitm - start_time.millitm);
}