#include "test_performance_meting_server.h"

START_TEST(test_opzetten_server)
    {
        commandStruct cmdStruct = commandStruct_initialize("CREATE_USER fatihTestUser testPassword");
        int resultCreateUser = handleCreateUserCommand(cmdStruct);
        commandStruct_free(&cmdStruct);
        ck_assert_int_eq(RPL_SUCCESS, resultCreateUser);
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
        tcase_add_test(tc_util_core, test_opzetten_server);
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

    srunner_set_fork_status(sRunner, CK_NOFORK);
    srunner_run_all(sRunner, CK_SILENT);
    srunner_free(sRunner);

    durationTest = getDurationTest(start_time);

    return durationTest;
}