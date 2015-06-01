#include "main.h"

int main(int argc, char **argv)
{

    mallopt(M_CHECK_ACTION, 1);

    int fork = BOOL_FALSE;
    if(argc > 1)
    {
        if(strcmp(argv[1], "FORK") == 0 || strcmp(argv[1], "fork") == 0 || strcmp(argv[1], "-f") == 0)
        {
            fork = BOOL_TRUE;
        }
    }

    runServer(fork, SERVER_PORT);


    if(setupDatabaseConnection() != DB_RETURN_SUCCES)
    {
        return EXIT_FAILURE;
    }

    channelInfo ci;
    if(getChannelByName("batcave", &ci) == DB_RETURN_SUCCES)
    {
        printf("%s | %s\n", ci.name, ci.password);
        channelInfo_free(&ci);
    }

    stopDatabase();

    return EXIT_SUCCESS;
}