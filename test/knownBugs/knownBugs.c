#include "knownBugs.h"

START_TEST(bug_xml_update_channel_with_3_privmsg)
{
    commandStruct cmdStruct = commandStruct_initialize("LOGIN fatih nub testNickname");
    int resultLogin = handleLoginCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(RPL_LOGIN, resultLogin);

    cmdStruct = commandStruct_initialize("PRIVMSG batcave :Test bericht");
    int resultParseMessage = handlePrivateMessageCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(RPL_AWAY, resultParseMessage);

    resultParseMessage = handlePrivateMessageCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(RPL_AWAY, resultParseMessage);

    resultParseMessage = handlePrivateMessageCommand(cmdStruct);
    commandStruct_free(&cmdStruct);
    ck_assert_int_eq(RPL_AWAY, resultParseMessage);
}
END_TEST

Suite* known_bugs_suite()
{
    Suite *suite;
    TCase *tc_util_core;

    suite = suite_create("knownBugsTest");

    tc_util_core = tcase_create("core");

    tcase_add_test(tc_util_core, bug_xml_update_channel_with_3_privmsg);

    suite_add_tcase(suite, tc_util_core);

    return suite;
}

int knownBugsSuiteCommand()
{
    int number_tests_failed = 0;
    Suite *suite;
    SRunner *sRunner;

    suite = known_bugs_suite();
    sRunner = srunner_create(suite);

    srunner_set_fork_status(sRunner, CK_FORK);
    srunner_run_all(sRunner, CK_NORMAL);
    number_tests_failed = srunner_ntests_failed(sRunner);
    srunner_free(sRunner);

    return number_tests_failed;
}