#include "main.h"

int main(int argc, char **argv)
{
    lastTimestamp = 0;

    /*
    int fork = BOOL_TRUE;
    int port = SERVER_PORT;
    int i;
    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "FORK") == 0 || strcmp(argv[i], "fork") == 0 || strcmp(argv[i], "-f") == 0)
        {
            fork = BOOL_TRUE;
        }
        if(strcmp(argv[i], "PORT") == 0 || strcmp(argv[i], "port") == 0 || strcmp(argv[i], "-p") == 0)
        {
            if(argc >= i + 1)
            {
                port = atoi(argv[i + 1]);
                i++;
            }
        }
    }
     */

    runServer(BOOL_TRUE, SERVER_PORT);
    stopDatabase();

    return EXIT_SUCCESS;
}