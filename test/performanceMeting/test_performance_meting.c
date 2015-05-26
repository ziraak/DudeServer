#include "test_performance_meting.h"

START_TEST(test_create_user_command)
{
    commandStruct cmdStruct = commandStruct_initialize("CREATE_USER fatihTestUser testPassword");
    int resultCreateUser = handleCreateUserCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(RPL_SUCCESS, resultCreateUser);
}
END_TEST

START_TEST(test_login_command)
{
    commandStruct cmdStruct = commandStruct_initialize("LOGIN fatihTestUser testPassword testNickname");
    int resultLogin = handleLoginCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(RPL_LOGIN, resultLogin);
}
END_TEST

START_TEST(test_login_command_wrong_password)
{
    commandStruct cmdStruct = commandStruct_initialize("LOGIN fatihTestUser testPassword2 testNickname");
    int resultLogin = handleLoginCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(ERR_NOLOGIN, resultLogin);
}
END_TEST

START_TEST(test_join_channel)
{
    commandStruct cmdStruct = commandStruct_initialize("JOIN batcaveTestChannel");
    int resultJoinChannel = handleJoinCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(RPL_TOPIC, resultJoinChannel);
}
END_TEST

START_TEST(test_delete_user)
{
    int resultDeleteUser = handleDeleteUserCommand();
    ck_assert_int_eq(RPL_SUCCESS, resultDeleteUser);
}
END_TEST

START_TEST(test_part_channel_command)
{
    commandStruct cmdStruct = commandStruct_initialize("PART batcaveTestChannel");
    int resultPartChannel = handlePartCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(RPL_SUCCESS, resultPartChannel);
}
END_TEST

START_TEST(test_update_nickname_command)
{
    commandStruct cmdStruct = commandStruct_initialize("UPDATE_NICKNAME changedNick");
    currentUser.username = "fatihTestUser";
    int resultPartChannel = handleUpdateNicknameCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(RPL_SUCCESS, resultPartChannel);
}
END_TEST

START_TEST(test_update_password_command)
{
    commandStruct cmdStruct = commandStruct_initialize("UPDATE_PASSWORD changedPass");
    currentUser.username = "fatihTestUser";
    int resultPartChannel = handleUpdatePasswordCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(RPL_SUCCESS, resultPartChannel);
}
END_TEST

Suite* performance_commands_suite(int amountOfCommandLoops)
{
    Suite *suite;
    TCase *tc_util_core;

    suite = suite_create("performanceTest");

    tc_util_core = tcase_create("performance");

    int i = 0;
    for (i = 0; i < amountOfCommandLoops; i++)
    {
        // Specifieke volgorde voor deze scenario.
        tcase_add_test(tc_util_core, test_create_user_command);
        tcase_add_test(tc_util_core, test_login_command_wrong_password);
        tcase_add_test(tc_util_core, test_login_command);
        tcase_add_test(tc_util_core, test_join_channel);
        tcase_add_test(tc_util_core, test_update_nickname_command);
        tcase_add_test(tc_util_core, test_update_password_command);
        tcase_add_test(tc_util_core, test_delete_user);
        tcase_add_test(tc_util_core, test_part_channel_command);
    }

    suite_add_tcase(suite, tc_util_core);

    return suite;
}

int testCommandsPerformanceMeting(int amountOfCommandLoops)
{
    struct timeb start_time;
    int durationTest;
    Suite *suite;
    SRunner *sRunner;

    ftime(&start_time);
    suite = performance_commands_suite(amountOfCommandLoops);
    sRunner = srunner_create(suite);

    srunner_set_fork_status(sRunner, CK_NOFORK);
    srunner_run_all(sRunner, CK_SILENT);
    srunner_free(sRunner);

    durationTest = getDurationTest(start_time);
    //printf("Duration of %ix commands tests in msecs: %i\n", amountOfCommandTestRuns, durationTest);

    return durationTest;
}

int getDurationTest(struct timeb start_time)
{
    struct timeb end_time;
    ftime(&end_time);
    return (int) ((end_time.time - start_time.time) * 1000) + (end_time.millitm - start_time.millitm);
}