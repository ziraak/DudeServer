//
// Created by osboxes on 18/05/15.
//

#include "check_channel.h"

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



//----------------___---------------------------------------_____--------------___----__----__----__----__---___---_-----_----
Suite *channel_suite(void)
{
    Suite *s;
    TCase *tc_channel_core;
    s = suite_create("channel");
    tc_channel_core = tcase_create("core");

    tcase_add_test(tc_channel_core, test_getMessages);
    tcase_add_test(tc_channel_core, test_getMessagesOnTime);

    tcase_add_test(tc_channel_core,test_getChannel);
    tcase_add_test(tc_channel_core,test_getChannel_noPass);

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