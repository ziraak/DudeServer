//
// Created by osboxes on 12/05/15.
//

#include "check_user.h"

char *uName = "dummy";
char *cName = "testChannel";
char *bannedName = "banned";
char *newUname = "dummy2";




//check user tests------------------------------------------------------------------------------------------------

START_TEST(checkUser_test_NULL)
    {
        ck_assert_int_eq(checkUser(NULL), DB_RETURN_NULLPOINTER);
    }
END_TEST

START_TEST(checkUser_test_false)
    {
        ck_assert_int_eq(checkUser("retard"), BOOL_FALSE);
    }
END_TEST

START_TEST(checkUser_test_true)
    {
        ck_assert_int_eq(checkUser(uName), BOOL_TRUE);
    }
END_TEST

//create user tests------------------------------------------------------------------------------------

START_TEST(createUser_test_alreadyExists)
    {
        ck_assert_int_eq(createNewUser(uName, "a"), DB_RETURN_ALREADYEXISTS);
    }
END_TEST

START_TEST(createUser_test_alreadyExistsButBanned)
    {
        ck_assert_int_eq(createNewUser(bannedName, "a"), DB_RETURN_ALREADYEXISTS);
    }
END_TEST

START_TEST(createUser_test_NULL)
    {
        ck_assert_int_eq(createNewUser(NULL, "a"), DB_RETURN_NULLPOINTER);
        ck_assert_int_eq(createNewUser(newUname, NULL), DB_RETURN_NULLPOINTER);
    }
END_TEST

START_TEST(createUser_test_CORRECT)
    {
        userInfo user;
        ck_assert_int_eq(createNewUser(newUname, "a"), DB_RETURN_SUCCES);
        ck_assert_int_eq(getUser(newUname,&user),DB_RETURN_SUCCES);
        ck_assert_str_eq(user.nickname,newUname);
        ck_assert_str_eq(user.password,"a");
        ck_assert_str_eq(user.username,newUname);
    }
END_TEST


// Join channel tests -------------------------------------------------------------------------------------

START_TEST(userJoinChannel_test_correct)
    {
        ck_assert_int_eq(userJoinChannel(uName, cName, USER_ROLE_USER), DB_RETURN_SUCCES);
        userInfo user;
        channelInfo channel;
        ck_assert_int_eq(getUser(uName, &user), DB_RETURN_SUCCES);
        ck_assert_str_eq(user.channels[0], cName);
        ck_assert_int_eq(getChannel(cName, &channel), DB_RETURN_SUCCES);
        ck_assert_str_eq(channel.users[0], uName);
    }
END_TEST

START_TEST(userJoinChannel_test_wrongChannel)
    {
        ck_assert_int_eq(userJoinChannel(uName, "a", NULL), DB_RETURN_DOESNOTEXIST);
    }
END_TEST

START_TEST(userJoinChannel_test_wrongUser)
    {
        ck_assert_int_eq(userJoinChannel("a", cName, NULL), DB_RETURN_DOESNOTEXIST);
    }
END_TEST

//delete users test-----------------------------------------------------------------------------------
START_TEST(test_deleteUser_correct)
    {
        char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
        sprintf(docname, "%s%s.xml", DB_USERLOCATION, newUname);
        deleteUser(newUname);
        ck_assert_int_eq(checkUser(newUname),BOOL_FALSE);
        ck_assert_int_eq(openDoc(docname),NULL);

        free(docname);
    }
END_TEST

START_TEST(test_deleteUser_banned)//TODO: doesn't support banned users
    {
        char *docname = (char *) malloc(DB_DOCNAMEMEMORYSPACE);
        sprintf(docname, "%s%s.xml", DB_USERLOCATION, bannedName);
        deleteUser(bannedName);
        ck_assert_int_eq(checkUser(bannedName),BOOL_FALSE);
        ck_assert_int_eq(openDoc(docname),!NULL);
        free(docname);
    }
END_TEST

START_TEST(test_getUsrNickname)
    {
        ck_assert_str_eq(getUserNickname("alibaba"),"ali-b");
    }
END_TEST



//----------------___---------------------------------------_____--------------___----__----__----__----__---___---_-----_----
Suite *user_suite(void)
{
    Suite *s;
    TCase *tc_user_core;

    s = suite_create("user");

    tc_user_core = tcase_create("core");

    tcase_add_test(tc_user_core, checkUser_test_NULL);
    tcase_add_test(tc_user_core, checkUser_test_false);
    tcase_add_test(tc_user_core, checkUser_test_true);

    tcase_add_test(tc_user_core, createUser_test_alreadyExists);
    tcase_add_test(tc_user_core, createUser_test_alreadyExistsButBanned);
    tcase_add_test(tc_user_core, createUser_test_NULL);
    tcase_add_test(tc_user_core, createUser_test_CORRECT);

    tcase_add_test(tc_user_core, userJoinChannel_test_correct);
    tcase_add_test(tc_user_core, userJoinChannel_test_wrongChannel);
    tcase_add_test(tc_user_core, userJoinChannel_test_wrongUser);

    tcase_add_test(tc_user_core, test_deleteUser_correct);


    tcase_add_test(tc_user_core, test_getUsrNickname);

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