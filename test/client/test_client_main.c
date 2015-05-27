#include "test_client_main.h"

int main(int argc, char **argv)
{
    char *cmd;
    char *port;
    char *ip;

    int sock;

    if (argv[1] == NULL)
    {
        for (; ;)
        {
            int cmdSize = prompt(CMD_SIZE, &cmd, "CONNECT NAAR '<IP> <POORT>': ");

            while (*cmd == ' ')
            {
                cmd++;
            }

            int i = substringCharacter(cmd, &ip);

            if (i == 1)
            {
                printf("NEED IP AND PORT\n");
                continue;
            }

            if ((i - 1) == cmdSize)
            {
                printf("NEED PORT\n");
                continue;
            }

            int j = substringCharacter(cmd + i, &port);

            if (j < 2)
            {
                printf("NEED PORT\n");
                continue;
            }

            sock = getServerSocket(atoi(port), ip);
            if (sock < 0)
            {
                printf("INVALID CONFIGURATION (%s:%s)\n", ip, port);
                continue;
            }

            printf("CONNECTED TO %s:%s (ID: %i)\n", ip, port, sock);
            break;
        }
    }
    else
    {
        sock = getServerSocket(atoi(argv[2]), argv[1]);
    }

    int childpid = fork();
    if (childpid == 0)
    {
        clientBusinessSend(sock);
    }
    else
    {
        clientBusinessReceive(sock);
    }

    return EXIT_SUCCESS;
}