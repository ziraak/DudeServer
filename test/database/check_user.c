//
// Created by osboxes on 12/05/15.
//

#include "check_user.h"

START_TEST(checkUser_test_NULL)
    {
        ck_assert_int_eq(checkUser(NULL),DB_RETURN_NULLPOINTER);
    }
END_TEST

START_TEST(checkUser_test_false)
    {
        ck_assert_int_eq(checkUser("retard"),BOOL_FALSE);
    }
END_TEST

START_TEST(checkUser_test_true)
    {
        ck_assert_int_eq(checkUser("dummy"),BOOL_TRUE);
    }
END_TEST

START_TEST(createUser_test_alreadyExists)
    {
        ck_assert_int_eq(createNewUser("dummy","aksf"),DB_RETURN_ALREADYEXISTS);
    }
END_TEST

START_TEST(createUser_test_alreadyExistsButBanned)
    {
        ck_assert_int_eq(createNewUser("banned","aksf"),DB_RETURN_ALREADYEXISTS);
    }
END_TEST

START_TEST(userJoinChannel_test_correct)
    {
        ck_assert_int_eq(userJoinChannel("fatih","rovers"),DB_RETURN_SUCCES);

    }
END_TEST

START_TEST(userJoinChannel_test_wrongChannel)
    {
        ck_assert_int_eq(userJoinChannel("dummy","rover"),DB_RETURN_DOESNOTEXIST);
    }
END_TEST

START_TEST(userJoinChannel_test_wrongUser)
    {
        ck_assert_int_eq(userJoinChannel("retard","batcave"),DB_RETURN_DOESNOTEXIST);
    }
END_TEST


Suite* user_suite(void)
{
    Suite *s;
    TCase *tc_user_core;

    s = suite_create("user");


    tc_user_core = tcase_create("core");

//    tcase_add_test(tc_user_core, checkUser_test_NULL);
//    tcase_add_test(tc_user_core, checkUser_test_false);
//    tcase_add_test(tc_user_core, checkUser_test_true);
//    tcase_add_test(tc_user_core, createUser_test_alreadyExists);
//    tcase_add_test(tc_user_core, createUser_test_alreadyExistsButBanned);
    tcase_add_test(tc_user_core, userJoinChannel_test_correct);
    tcase_add_test(tc_user_core, userJoinChannel_test_wrongChannel);
    tcase_add_test(tc_user_core, userJoinChannel_test_wrongUser);

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