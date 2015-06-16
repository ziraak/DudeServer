#include "main.h"

int main(int argc, char **argv)
{
    lastTimestamp = 0;

    uint16_t port = SERVER_PORT;
    int i;
    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "PORT") == 0 || strcmp(argv[i], "port") == 0 || strcmp(argv[i], "-p") == 0)
        {
            if(argc >= i + 1)
            {
                port = (uint16_t)atoi(argv[i + 1]);
                printf("ACCEPTED PORT %i FROM ARGUMENTS\n", port);
                i++;
            }
        }
    }

    runServer(port);

    return EXIT_SUCCESS;
}