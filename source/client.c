#include "client.h"

int clientWrite;
int clientRead;
int clientNumber;

void exitClient(int otherPid)
{
    printf("#%i: CLOSED (KILLED %i)\n", clientNumber, otherPid);
    kill(otherPid, SIGKILL);
    sslClose();
    close(clientRead);
}

void sendToServer(char *msg)
{
    printf("#%i -> S: '%s'\n", clientNumber, msg);
    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "#%i %s", clientNumber, msg);
    write(clientWrite, buffer, INNER_BUFFER_LENGTH);
    FREE(buffer);
}

int handleServerRead()
{
    int i = fork();
    if(i != 0)
    {
        return i;
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

    exitClient(getppid());
    exit(0);
}

void handleClientRead(int otherPid)
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

    exitClient(otherPid);
    exit(0);
}

int clientAcceptProcedure()
{
    printf("#%i: READING ON: %i\n", clientNumber, clientRead);

    sendToServer("ACCEPT");

    char* clientName = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(clientName, CLIENT_MKFIFO_LOCATION, clientNumber);
    clientRead = open(clientName, O_RDONLY);

    if(clientRead < 0)
    {
        perror("OPEN:");
        exit(-6);
    }

    FREE(clientName);

    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    if(read(clientRead, buffer, INNER_BUFFER_LENGTH) < 0)
    {
        perror("READ:");
        exit(-2);
    }
    printf("#%i: RECEIVED '%s'\n", clientNumber, buffer);

    if(strcmp(buffer, "ACCEPT") != 0)
    {
        printf("#%i: ACCEPT DENIED [%s]\n", clientNumber, buffer);
        FREE(buffer);
        return BOOL_FALSE;
    }

    printf("#%i: ACCEPTED\n", clientNumber);

    sendToServer("ACTIVE");
    FREE(buffer);

    return BOOL_TRUE;
}

void handleClientProcess(int writeSocket, int number)
{
    if(fork() == 0)
    {
        return;
    }

    clientWrite = writeSocket;
    clientNumber = number;

    if(clientAcceptProcedure() == BOOL_FALSE)
    {
        exit(0);
    }

    printf("#%i: STARTED\n", number);

    handleClientRead(handleServerRead());
}
