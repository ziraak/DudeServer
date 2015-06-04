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
        ck_assert_int_eq(checkIfUserExists(NULL), DB_RETURN_NULLPOINTER);
        ck_assert_int_eq(checkIfUserExists(""), DB_RETURN_NULLPOINTER);
    }
END_TEST

START_TEST(checkUser_test_false)
    {
        ck_assert_int_eq(checkIfUserExists("retard"), BOOL_FALSE);
    }
END_TEST

START_TEST(checkUser_test_true)
    {
        int result = checkIfUserExists("fatih");
        ck_assert_int_eq(result, BOOL_TRUE);
    }
END_TEST

//create user tests------------------------------------------------------------------------------------

START_TEST(createUser_test_alreadyExists)
    {
        ck_assert_int_eq(createNewUser(uName, "a"), DB_RETURN_ALREADYEXISTS);
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
        ck_assert_int_eq(userJoinChannel("ferdi", "eigendunk", USER_ROLE_USER), DB_RETURN_SUCCES);
        userInfo user;
        channelInfo channel;
        ck_assert_int_eq(getUser("ferdi", &user), DB_RETURN_SUCCES);
//        ck_assert_str_eq(user.channels[0], cName);
//        ck_assert_int_eq(getChannelByName(cName, &channel), DB_RETURN_SUCCES);
//        ck_assert_str_eq(channel.users[0], uName);
        userLeaveChannel("ferdie","eigendunk");
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

        deleteUser(newUname);
        ck_assert_int_eq(checkIfUserExists(newUname),BOOL_FALSE);

    }
END_TEST

START_TEST(test_getUsrNickname)
    {
        createNewUser("alibaba","open sesame");
        changeNickname("alibaba","ali-b");
        char* nickname = getUserNickname("alibaba");
        ck_assert_str_eq(nickname,"ali-b");
        free(nickname);
    }
END_TEST

START_TEST(test_all)
    {
        timeStart;
        createNewUser("awesome","awesome");
        checkIfUserExists("awesome");
        userInfo result;
        getUser("awesome",&result);
        userInfo_free(&result);
        userJoinChannel("awesome", "eigendunk","Fuhrer");

        changeNickname("awesome", "not awesome");

        changePassword("awesome","P@$$w0rd");

        char* awesome = getUserNickname("awesome");
        ck_assert_str_eq("not awesome",awesome);
        free(awesome);

        userLeaveChannel("awesome","eigendunk");
        deleteUser("awesome");
        timeStop("test_All");
    }
END_TEST




//----------------___---------------------------------------_____--------------___----__----__----__----__---___---_-----_----
Suite *user_suite(void)
{
    Suite *s;
    TCase *tc_user_core;

    s = suite_create("user");

    tc_user_core = tcase_create("core");

//    tcase_add_test(tc_user_core, checkUser_test_NULL);
//    tcase_add_test(tc_user_core, checkUser_test_false);
//    tcase_add_test(tc_user_core, checkUser_test_true);

//    tcase_add_test(tc_user_core, createUser_test_alreadyExists);
//    tcase_add_test(tc_user_core, createUser_test_NULL);
//    tcase_add_test(tc_user_core, createUser_test_CORRECT);

//    tcase_add_test(tc_user_core, userJoinChannel_test_correct);
//    tcase_add_test(tc_user_core, userJoinChannel_test_wrongChannel);
//    tcase_add_test(tc_user_core, userJoinChannel_test_wrongUser);

//    tcase_add_test(tc_user_core, test_deleteUser_correct);

    tcase_add_loop_test(tc_user_core, test_all,0,100);

//    tcase_add_test(tc_user_core, test_getUsrNickname);

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

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed;
}