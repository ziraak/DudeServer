#include "main.h"

int main(int argc, char **argv)
{
    /*
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
    */

    if(setupDatabaseConnection() != BOOL_TRUE)
    {
        return EXIT_FAILURE;
    }



    stopDatabase();

    return EXIT_SUCCESS;
}