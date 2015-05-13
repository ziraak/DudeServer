#include "test_suite_commands.h"

START_TEST(test_create_user_command)
    {
        commandStruct cmdStruct;
        parseCommand("CREATE_USER fatihTestUser testPassword", &cmdStruct);
        int resultCreateUser = handleCreateUserCommand(cmdStruct);
        commandStruct_free(&cmdStruct);
        ck_assert_int_eq(RPL_SUCCESS, resultCreateUser);
    }
END_TEST

START_TEST(test_login_command)
    {
        commandStruct cmdStruct;
        parseCommand("LOGIN fatihTestUser testPassword testNickname", &cmdStruct);
        int resultLogin = handleLoginCommand(cmdStruct);
        commandStruct_free(&cmdStruct);
        ck_assert_int_eq(RPL_LOGIN, resultLogin);
    }
END_TEST

START_TEST(test_login_command_wrong_password)
    {
        commandStruct cmdStruct;
        parseCommand("LOGIN fatihTestUser testPassword2 testNickname", &cmdStruct);
        int resultLogin = handleLoginCommand(cmdStruct);
        commandStruct_free(&cmdStruct);
        ck_assert_int_eq(ERR_NOLOGIN, resultLogin);
    }
END_TEST

START_TEST(test_join_channel)
    {
        commandStruct cmdStruct;
        parseCommand("JOIN batcaveTestChannel", &cmdStruct);
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
        commandStruct cmdStruct;
        parseCommand("PART batcaveTestChannel", &cmdStruct);
        int resultPartChannel = handlePartCommand(cmdStruct);
        commandStruct_free(&cmdStruct);
        ck_assert_int_eq(RPL_SUCCESS, resultPartChannel);
    }
END_TEST

Suite* commands_suite()
{
    Suite *suite;
    TCase *tc_util_core;

    suite = suite_create("commandsTest");

    tc_util_core = tcase_create("core");

    // Specifieke volgorde voor deze scenario.
    tcase_add_test(tc_util_core, test_create_user_command);
    tcase_add_test(tc_util_core, test_login_command_wrong_password);
    tcase_add_test(tc_util_core, test_login_command);
    tcase_add_test(tc_util_core, test_join_channel);
    tcase_add_test(tc_util_core, test_delete_user);
    tcase_add_test(tc_util_core, test_part_channel_command);


    suite_add_tcase(suite, tc_util_core);

    return suite;
}

int testSuiteCommand()
{
    int number_tests_failed = 0;
    Suite *suite;
    SRunner *sRunner;

    suite = commands_suite();
    sRunner = srunner_create(suite);

    srunner_set_fork_status(sRunner, CK_NOFORK);
    srunner_run_all(sRunner, CK_NORMAL);
    number_tests_failed = srunner_ntests_failed(sRunner);
    srunner_free(sRunner);

    return number_tests_failed;
}