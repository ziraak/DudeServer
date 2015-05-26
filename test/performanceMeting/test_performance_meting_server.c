#include "test_performance_meting_server.h"

int SERVER_PORT = 9000;

START_TEST(test_connecting_multiple_clients)
    {
        getServerSocket(SERVER_PORT, "127.0.0.1");
    }
END_TEST

Suite* performance_server_suite(int amountOfCommandLoops)
{
    Suite *suite;
    TCase *tc_util_core;

    suite = suite_create("performanceTest");

    tc_util_core = tcase_create("performance");

    int i = 0;
    for (i = 0; i < amountOfCommandLoops; i++)
    {
        // Specifieke volgorde voor deze scenario.
        tcase_add_test(tc_util_core, test_connecting_multiple_clients);
    }

    suite_add_tcase(suite, tc_util_core);

    return suite;
}

int testServerPerformanceMeting(int amountOfCommandLoops)
{
    struct timeb start_time;
    int durationTest;
    Suite *suite;
    SRunner *sRunner;

    ftime(&start_time);
    suite = performance_server_suite(amountOfCommandLoops);
    sRunner = srunner_create(suite);

    srunner_set_fork_status(sRunner, CK_NORMAL); // CK_NOFORK
    srunner_run_all(sRunner, CK_SILENT);
    srunner_free(sRunner);

    durationTest = getDurationTest(start_time);

    return durationTest;
}