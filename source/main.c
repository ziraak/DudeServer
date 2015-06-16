#include "main.h"

int main(int argc, char **argv)
{
    lastTimestamp = 0;

    int port = SERVER_PORT;
    int i;
    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "PORT") == 0 || strcmp(argv[i], "port") == 0 || strcmp(argv[i], "-p") == 0)
        {
            if(argc >= i + 1)
            {
                port = atoi(argv[i + 1]);
                i++;
            }
        }
    }

    runServer(port);

    return EXIT_SUCCESS;
}