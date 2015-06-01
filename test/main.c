#include "main.h"

int main()
{
    setupDatabaseConnection();
    int numberOfFailedTests = 0;
    numberOfFailedTests += unitTests();
//    performanceMetingCommandsTests();
//    performanceMetingServer();

//    numberOfFailedTests += knownBugsSuiteCommand();

    stopDatabase();
    return (numberOfFailedTests > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int unitTests()
{
    int numberOfFailedTests = 0;
//    numberOfFailedTests += utils_tests();
    numberOfFailedTests += user_tests();
//    numberOfFailedTests += channel_tests();
//    numberOfFailedTests += testSuiteCommand(); // achteraan want forked niet
    return numberOfFailedTests;
}

int performanceMetingCommandsTests()
{
    int durationTestsInMsec = 0;
    int i;
    int averageDurationTestsInMsec = 0;
    int amountOfCommandLoops = PERFORMANCE_METING_AMOUNT_OF_COMMAND_LOOPS;
    int numberOfCommandTestSuiteRun = PERFORMANCE_METING_AMOUNT_OF_TEST_SUITES;
    for (i = 0; i < numberOfCommandTestSuiteRun; i++)
    {
        durationTestsInMsec += testCommandsPerformanceMeting(amountOfCommandLoops);
    }
    averageDurationTestsInMsec = durationTestsInMsec / (numberOfCommandTestSuiteRun * amountOfCommandLoops);

    printf("Number of command loops in a test suite: %i\n", amountOfCommandLoops);
    printf("Number of test suites run: %i\n", numberOfCommandTestSuiteRun);
    printf("Every suite ran all commands %i times. Average duration each set of commands of %i commands suite(s) (in msec): %i\n" , amountOfCommandLoops, numberOfCommandTestSuiteRun,
           averageDurationTestsInMsec);

    return averageDurationTestsInMsec;
}

int performanceMetingServer()
{
    performanceInfo performanceInfoServer = { 0, 0, 0};
    int i;
    int averageDurationTestsInMsec = 0;
    int amountOfTestLoops = 10;
    int numberOfTestSuiteRun = 10;

    for (i = 0; i < numberOfTestSuiteRun; i++)
    {
        performanceInfo performanceInfoPerformanceMeting;
        performanceInfoPerformanceMeting = testServerPerformanceMeting(amountOfTestLoops);
        performanceInfoServer.durationTest += performanceInfoPerformanceMeting.durationTest;
        performanceInfoServer.AMOUNT_OF_CLIENTS_RECEIVED_CORRECT_DATA += performanceInfoPerformanceMeting.AMOUNT_OF_CLIENTS_RECEIVED_CORRECT_DATA;
        performanceInfoServer.AMOUNT_OF_CLIENTS_RECEIVED_DATA += performanceInfoPerformanceMeting.AMOUNT_OF_CLIENTS_RECEIVED_DATA;
    }
    averageDurationTestsInMsec = performanceInfoServer.durationTest / (numberOfTestSuiteRun * amountOfTestLoops);

    printf("Number of test loops in a test suite: %i\n", amountOfTestLoops);
    printf("Number of server test suites run: %i\n", numberOfTestSuiteRun);
    printf("Average of CORRECT data received: (%i/%i)\n", performanceInfoServer.AMOUNT_OF_CLIENTS_RECEIVED_CORRECT_DATA / numberOfTestSuiteRun, performanceInfoServer.AMOUNT_OF_CLIENTS_RECEIVED_DATA / numberOfTestSuiteRun);
    printf("Every suite ran all tests %i times. Average duration each set of tests of %i server suite(s) (in msec): %i\n" ,
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