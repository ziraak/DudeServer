//
// Created by Sjuul Wijnia on 20/04/15.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ssl/communicationStructs.h>
#include "../../source/utils/utils.h"

#define CMD_SIZE 256

void clientBusinessSend();
void clientBusinessReceive();

int getServerSocket(int port, char *ip);

int input(size_t s, char **result);
int prompt(size_t s, char **result, char *message);

int main(int argc, char **argv)
{
    int success = getServerSocket(SERVER_PORT, SERVER_IP);

    if(success != SSL_OK)
    {
        perror("No connection!\n");
        return success;
    }

    printf("VERBONDEN MET %s:%i\n", SERVER_IP, SERVER_PORT);

    int childpid = fork();
    if (childpid == 0)
    {
        clientBusinessSend();
    }
    else
    {
        clientBusinessReceive();
    }

    return EXIT_SUCCESS;
}

void clientBusinessSend()
{
    char *snd = NULL;

    for (; ;)
    {
        int len = prompt(CMD_SIZE, &snd, "COMMAND: ");

        if (strcmp(snd, "QUIT\n") == 0)
        {
            printf("CLOSING CONNECTION, BYE BYE\n");
            return;
        }
        else if (strcmp(snd, "") != 0)
        {
            if (strcmp(snd, "LOGIN\n") == 0)
            {
                SSL_write(connection.ssl_handle, "LOGIN fatih nub", 15);
            }
            else if (strcmp(snd, "JOIN\n") == 0)
            {
                SSL_write(connection.ssl_handle, "JOIN batcave", 12);
            }
            else if (SSL_write(connection.ssl_handle, snd, len) < 0)
            {
                perror("SEND ERROR, CLOSING CLIENT");
                exit(EXIT_FAILURE);
            }
        }

        free(snd);
    }
}

void clientBusinessReceive()
{
    char *rcv = NULL;

    rcv = malloc(CMD_SIZE + 1);
    bzero(rcv, CMD_SIZE + 1);

    for (; ;)
    {
        int read = sslRead(rcv, CMD_SIZE);
        if(read != SSL_OK)
        {
            perror("ERROR RECEIVING.");
            return;
        }

        printf("RECEIVED: '%s'\n", rcv);

        bzero(rcv, CMD_SIZE + 1);
    }
}

int getServerSocket(int port, char *ip)
{
    struct sockaddr_in serverAddress;
    int serverSocket;

    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons((uint16_t)port);
    serverAddress.sin_addr.s_addr = inet_addr(ip);

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("KON GEEN SOCKET AANMAKEN\n");
        return -1;
    }

    if (connect(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("KON SOCKET NIET VERBINDEN\n");
        return -1;
    }

    SSL_load_error_strings();
    SSL_library_init();

    SSL_CTX *sslContext = SSL_CTX_new(SSLv23_client_method());
    if(sslContext == NULL)
    {
        SSL_ERROR_RETURN(SSL_NO_SSL_CONTEXT);
    }

    int sslCertificateLoad = sslLoadCertificate(sslContext, SSL_CERTIFICATE_LOCATION);
    if(sslCertificateLoad != SSL_OK)
    {
        return sslCertificateLoad;
    }

    SSL *sslHandle = SSL_new(sslContext);
    if(sslHandle == NULL)
    {
        SSL_ERROR_RETURN(SSL_NO_SSL_HANDLE);
    }

    if(SSL_set_fd(sslHandle, serverSocket) == 0)
    {
        SSL_ERROR_RETURN(SSL_NO_SSL_HANDLE_SOCKET_LINK);
    }

    if(SSL_connect(sslHandle) != 1)
    {
        SSL_ERROR_RETURN(1);
    }

    sslConnection conn = {
            .socket = serverSocket,
            .ssl_context = sslContext,
            .ssl_handle = sslHandle,
            .address = serverAddress
    };
    connection = conn;

    return SSL_OK;
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
