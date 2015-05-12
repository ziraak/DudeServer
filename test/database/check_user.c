//
// Created by osboxes on 12/05/15.
//

#include "check_user.h"

START_TEST()







Suite* user_suite(void)
{
    Suite *s;
    TCase *tc_user_core;

    s = suite_create("utils");

    tc_user_core = tcase_create("core");

    //tcase_add_test(tc_util_core, test_utils_substringCharacter);
    //tcase_add_test(tc_util_core, test_utils_parseCommand);

    suite_add_tcase(s, tc_user_core);

    return s;
}



int user_tests()
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = user_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed;
}