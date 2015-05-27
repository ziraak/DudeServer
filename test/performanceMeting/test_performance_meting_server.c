#include "test_performance_meting_server.h"

int AMOUNT_OF_CLIENTS_RECEIVED_DATA = 0;

int SERVER_PORT = 9000;

START_TEST(test_connecting_multiple_clients)
    {
        int sock = getServerSocket(SERVER_PORT, "127.0.0.1");
        send(sock, "Test\0", 4, 0);
        char messageFromServer[255];
        bzero(messageFromServer, 255);
        read(sock, &messageFromServer, sizeof(messageFromServer));
        printf("Message from server: %s", messageFromServer);
        AMOUNT_OF_CLIENTS_RECEIVED_DATA++;
    }
END_TEST

Suite* performance_server_suite(int amountOfCommandLoops)
{
    Suite *suite;
    TCase *tc_util_core;

    suite = suite_create("performanceTest");

    tc_util_core = tcase_create("performance");

    int i = 0;
    for (i = 0; i < amountOfCommandLoops; i++)
    {
        // Specifieke volgorde voor deze scenario.
        tcase_add_test(tc_util_core, test_connecting_multiple_clients);
    }

    suite_add_tcase(suite, tc_util_core);

    return suite;
}

int testServerPerformanceMeting(int amountOfCommandLoops)
{
    struct timeb start_time;
    int durationTest;
    Suite *suite;
    SRunner *sRunner;

    ftime(&start_time);
    suite = performance_server_suite(amountOfCommandLoops);
    sRunner = srunner_create(suite);

    srunner_set_fork_status(sRunner, CK_NORMAL); // CK_NOFORK
    srunner_run_all(sRunner, CK_SILENT);
    srunner_free(sRunner);

    durationTest = getDurationTest(start_time);

    printf("AMOUNT_OF_CLIENTS_RECEIVED_DATA: %i\n", AMOUNT_OF_CLIENTS_RECEIVED_DATA);

    return durationTest;
}