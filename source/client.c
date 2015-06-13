#include "client.h"

int clientWrite;
int clientRead;
int clientNumber;

void exitClient()
{
    sslClose();
    close(clientRead);
}

void sendToServer(char *msg)
{
    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "#%i %s", clientNumber, msg);
    write(clientWrite, buffer, INNER_BUFFER_LENGTH);
    FREE(buffer);
}

void handleServerRead()
{
    if(fork() != 0)
    {
        return;
    }

    while(1)
    {
        char* buffer = MALLOC(INNER_BUFFER_LENGTH);
        ssize_t rd = read(clientRead, buffer, INNER_BUFFER_LENGTH);
        if(rd <= 0 || buffer[0] == '\0' || connection.ssl_handle == NULL)
        {
            sendToServer("CLOSE");
            FREE(buffer);
            break;
        }

        printf("#%i: SERVER -> CLIENT '%s'\n", clientNumber, buffer);
        sslSend(buffer);
    }

    exitClient();
    exit(0);
}

void handleClientRead()
{
    while(1)
    {
        char* buffer = MALLOC(INNER_BUFFER_LENGTH);
        sslRead(buffer, INNER_BUFFER_LENGTH);

        if(buffer[0] == '\0')
        {
            sendToServer("CLOSE");
            FREE(buffer);
            break;
        }

        printf("#%i: CLIENT -> SERVER '%s'\n", clientNumber, buffer);

        sendToServer(buffer);
        FREE(buffer);
    }

    exitClient();
    exit(0);
}

void handleClientProcess(int writeSocket, int readSocket, int number)
{
    if(fork() != 0)
    {
        return;
    }
    printf("#%i: STARTED\n", number);

    clientWrite = writeSocket;
    clientRead = readSocket;
    clientNumber = number;

    handleServerRead();
    handleClientRead();
}
