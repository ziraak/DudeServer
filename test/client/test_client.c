#include "test_client.h"

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

void clientBusinessSend(int sock)
{
    char *snd = NULL;

    for (; ;)
    {
        int len = prompt(CMD_SIZE, &snd, "COMMAND: ");

        if (strcmp(snd, "QUIT\n") == 0)
        {
            printf("CLOSING CONNECTION, BYE BYE\n");
            close(sock);
            return;
        }
        else if (strcmp(snd, "") != 0)
        {
            if (strcmp(snd, "LOGIN\n") == 0)
            {
                char *lgn = "LOGIN fatih nub";
                send(sock, lgn, strlen(lgn), 0);
            }
            else if (strcmp(snd, "JOIN\n") == 0)
            {
                char *lgn = "JOIN batcave";
                send(sock, lgn, strlen(lgn), 0);
            }
            else if (send(sock, snd, (size_t)len, 0) < 0)
            {
                perror("SEND ERROR, CLOSING CLIENT");
                close(sock);
                exit(EXIT_FAILURE);
            }
        }

        free(snd);
    }
}

void clientBusinessReceive(int sock)
{
    char *rcv = NULL;

    rcv = malloc(CMD_SIZE + 1);
    bzero(rcv, CMD_SIZE + 1);

    for (; ;)
    {
        ssize_t received = recv(sock, rcv, CMD_SIZE, 0);
        if (received < 0)
        {
            perror("RECEIVE ERROR, CLOSING CLIENT");
            close(sock);
            exit(2);
        }
        else if (received == 0)
        {
            printf("CONNECTION CLOSED BY SERVER, BYE BYE\n");
            close(sock);
            return;
        }
        else
        {
            printf("RECEIVED: '%s'\n", rcv);
        }

        bzero(rcv, CMD_SIZE + 1);
    }
}

int getServerSocket(int port, char *ip)
{
    struct sockaddr_in addr;
    int sock;

    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("KON GEEN SOCKET AANMAKEN\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        printf("KON SOCKET NIET VERBINDEN\n");
        return -1;
    }

    return sock;
}

int input(size_t s, char **result)
{
    char *line = malloc(s + 1), *linePointer = line;
    char c;

    int count = 0;

    if (line == NULL)
    {
        return -1;
    }

    bzero(line, s);

    for (; ;)
    {
        if (count == (s - 1))
        {
            break;
        }

        c = (char) fgetc(stdin);

        if (c == EOF)
        {
            break;
        }
        count++;

        if ((*line++ = c) == '\n')
        {
            break;
        }
    }

    *line = '\0';
    *result = linePointer;

    return --count;
}

int prompt(size_t s, char **result, char *message)
{
    printf("%s", message);

    return input(s, result);
}
