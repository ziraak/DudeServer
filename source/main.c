#include "main.h"

#define PRINT_CHANNEL(c) printf("%s: %s | %s | %i\n", c.name, c.password, c.topic, c.visible)

int main(int argc, char **argv)
{
//
//    int fork = BOOL_FALSE;
//    if(argc > 1)
//    {
//        if(strcmp(argv[1], "FORK") == 0 || strcmp(argv[1], "fork") == 0 || strcmp(argv[1], "-f") == 0)
//        {
//            fork = BOOL_TRUE;
//        }
//    }
//
//    runServer(fork, SERVER_PORT);

    if(setupDatabaseConnection() != DB_RETURN_SUCCES)
    {
        return EXIT_FAILURE;
    }

    int i;
    channelInfo *cis = getChannels(ALL_COLUMNS, &i);
    if(i != BOOL_FALSE)
    {
        int j;
        for(j = 0; j < i; j++)
        {
            PRINT_CHANNEL(cis[j]);
        }

        channelInfos_free(cis, i);
    }

    messageInfo messageInfoStruct = {
        .writer = "fatih",
        .timestamp = "0",
        .body = "Test message"
    };

    int result  = insertMessage(messageInfoStruct, "batcave");
    printf("Result: %i", result);

    stopDatabase();

    return EXIT_SUCCESS;
}