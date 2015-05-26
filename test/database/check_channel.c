//
// Created by osboxes on 18/05/15.
//

#include "check_channel.h"

//getUserlist tests
START_TEST(test_getUserlistFromChannel)
    {
        channelUser *users;
        users = getUsersFromChannel("fatihs head");
        ck_assert_str_eq(users[0].username,"fatih");
        ck_assert_str_eq(users[0].nickname,"ik moet jou in mijn team");
        ck_assert_str_eq(users[0].role,USER_ROLE_OPERATOR);
    }
END_TEST

START_TEST(test_getUserRole)
    {
        ck_assert_str_eq(getUserRole("fatihs head","fatih"),USER_ROLE_OPERATOR);
    }
END_TEST



//get channel tests
START_TEST(test_getChannel)
    {
        channelInfo channelInfo1;
        getChannel("batcave",&channelInfo1);
        ck_assert_str_eq(channelInfo1.topic,"everything is batman");
        ck_assert_str_eq(channelInfo1.password,"open sesame");
    }
END_TEST

START_TEST(test_getChannel_noPass)
    {
        channelInfo channelInfo1;
        getChannel("eigendunk",&channelInfo1);
        ck_assert(channelInfo1.password == NULL);
    }
END_TEST


//getMessages tests

START_TEST(test_getMessages)
    {
        char* body = "test123";
        char* body2 = "test321";
        messageInfo* messageInfo1;
        messageInfo1= getMessages("testChannel");
        ck_assert_str_eq(messageInfo1[0].body,body);
        ck_assert_str_eq(messageInfo1[1].body,body2);
    }
END_TEST

START_TEST(test_getMessagesOnTime)
    {
        char* body = "test321";
        messageInfo* messageInfo1;
        messageInfo1= getMessagesOnTime("testChannel",10);
        ck_assert_str_eq(messageInfo1[0].body,body);
    }
END_TEST

//authenticate password

START_TEST(test_authenticatePassword)
    {
        ck_assert_int_eq(checkIfChannelHasPassword("batcave"),BOOL_TRUE);
        ck_assert_int_eq(authenticateChannelPassword("batcave","open sesame"),BOOL_TRUE);
        ck_assert_int_eq(checkIfChannelHasPassword("eigendunk"),BOOL_FALSE);
        ck_assert_int_eq(authenticateChannelPassword("eigendunk","open sesame"),BOOL_FALSE);
    }
END_TEST


START_TEST(test_addToList)
    {
//        addChannelToList("eigendunk",BOOL_TRUE);
//        addChannelToList("batcave",BOOL_FALSE);
//        addChannelToList("fatihs head",BOOL_TRUE);
//        addChannelToList("rovers",BOOL_FALSE);
//        addChannelToList("testChannel",BOOL_TRUE);
        char **list = getVisibleChannels();
        int i = 0;
        while(list[i]!= NULL)
        {
            printf("channel: %s\n",list[i]);
            i++;
        }
        ck_assert_int_eq(checkIfChannelVisible("eigendunk"),BOOL_TRUE);
        ck_assert_int_eq(checkIfChannelVisible("batcave"),BOOL_FALSE);
    }
END_TEST

//----------------___---------------------------------------_____--------------___----__----__----__----__---___---_-----_----
Suite *channel_suite(void)
{
    Suite *s;
    TCase *tc_channel_core;
    s = suite_create("channel");
    tc_channel_core = tcase_create("core");

//    tcase_add_test(tc_channel_core, test_getMessages);
//    tcase_add_test(tc_channel_core, test_getMessagesOnTime);

    tcase_add_test(tc_channel_core,test_getChannel);
    tcase_add_test(tc_channel_core,test_getChannel_noPass);

    tcase_add_test(tc_channel_core,test_authenticatePassword);

    tcase_add_test(tc_channel_core,test_addToList);

    tcase_add_test(tc_channel_core,test_getUserlistFromChannel);
    tcase_add_test(tc_channel_core,test_getUserRole);

    suite_add_tcase(s, tc_channel_core);
    return s;
}


int channel_tests()
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = channel_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return number_failed;
}