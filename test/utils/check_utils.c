//
// Created by osboxes on 30/04/15.
//

#include "check_utils.h"

int test_utils_substringCharacter_inner(char* incoming, char* expected_result, int expected_offset)
{
    char* cmd = NULL;
    int offset = 0;

    offset = substringCharacter(incoming, &cmd);

    ck_assert_int_eq(offset, expected_offset);
    ck_assert_str_eq(cmd, expected_result);

    free(cmd);
    return offset;
}

START_TEST(test_utils_substringCharacter)
    {
        char* incoming = "sjuul fatih harmen desmond ferdi";

        incoming += test_utils_substringCharacter_inner(incoming, "sjuul", 6);
        incoming += test_utils_substringCharacter_inner(incoming, "fatih", 6);
        incoming += test_utils_substringCharacter_inner(incoming, "harmen", 7);
        incoming += test_utils_substringCharacter_inner(incoming, "desmond", 8);
        test_utils_substringCharacter_inner(incoming, "ferdi", 6);
    }
END_TEST

Suite* utils_suite(void)
{
    Suite *s;
    TCase *tc_util_core;

    s = suite_create("utils");

    tc_util_core = tcase_create("core");

    tcase_add_test(tc_util_core, test_utils_substringCharacter);

    suite_add_tcase(s, tc_util_core);

    return s;
}

int utils_tests()
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = utils_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed;
}