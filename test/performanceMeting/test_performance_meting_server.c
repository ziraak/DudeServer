#include "test_performance_meting_server.h"

performanceInfo performanceInfoServer = {0, 0, 0};
sslConnection connection;

START_TEST(test_connecting_multiple_clients)
    {
        getServerSocket(SERVER_PORT, SERVER_IP);

        SSL_write(connection.ssl_handle, "Test\0", 4);
        char messageFromServer[255];
        bzero(messageFromServer, 255);
        sslRead(messageFromServer, sizeof(messageFromServer));
        printf("Message from server: %s", messageFromServer);
        messageFromServer[strlen(messageFromServer) - 2] = '\0'; // remove carriage return
        if (strcmp(messageFromServer, "100") == 0)
        {
            printf("Received correct accept message! \n");
            performanceInfoServer.AMOUNT_OF_CLIENTS_RECEIVED_CORRECT_DATA++;
        }
        performanceInfoServer.AMOUNT_OF_CLIENTS_RECEIVED_DATA++;
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

performanceInfo testServerPerformanceMeting(int amountOfCommandLoops)
{
    performanceInfoServer.durationTest = 0;
    performanceInfoServer.AMOUNT_OF_CLIENTS_RECEIVED_DATA = 0;
    performanceInfoServer.AMOUNT_OF_CLIENTS_RECEIVED_CORRECT_DATA = 0;
    struct timeb start_time;
    Suite *suite;
    SRunner *sRunner;

    ftime(&start_time);
    suite = performance_server_suite(amountOfCommandLoops);
    sRunner = srunner_create(suite);

    srunner_set_fork_status(sRunner, CK_NORMAL); // CK_NOFORK
    srunner_run_all(sRunner, CK_SILENT);
    srunner_free(sRunner);

    //printf("AMOUNT_OF_CLIENTS_RECEIVED_DATA: %i\n", performanceInfoServer.AMOUNT_OF_CLIENTS_RECEIVED_DATA);
    //printf("AMOUNT_OF_CLIENTS_RECEIVED_CORRECT_DATA: %i\n", performanceInfoServer.AMOUNT_OF_CLIENTS_RECEIVED_CORRECT_DATA);

    performanceInfoServer.durationTest = getDurationTest(start_time);
    return performanceInfoServer;
}