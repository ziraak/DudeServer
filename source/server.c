#include <pthread.h>
#include "server.h"

int authenticated = BOOL_FALSE;
int listenPipe[2];
int aantalChilderen = 0;
int *childPipeFd[2];
int sock;
#define THREAD(tid, function) pthread_create(&tid, NULL, (void *) &function, NULL);
#define EXIT_THREAD pthread_exit(NULL);
#define JOIN_THREAD(tid) pthread_join(tid, NULL);
int communicateWithClientThreadRunning = BOOL_TRUE;

void acceptIncomingConnections()
{
    sslInitialize();

    printf("ACCEPTING MULTIPLE CLIENTS ON PORT\n");
    for (; ;)
    {
        childPipeFd[aantalChilderen] = malloc(10);
        pipe(childPipeFd[aantalChilderen]);

        struct sockaddr_in clientAddress;
        socklen_t client_address_size = sizeof(clientAddress);
        int clientSocket;
        if((clientSocket = accept(sock, (struct sockaddr *)&clientAddress, &client_address_size)) > -1)
        {
            processConnectedClientWithFork(clientSocket, clientAddress);
        }
        else
        {
            exit(EXIT_FAILURE);
        }
        aantalChilderen++;

        sslClose();

        size_t pipeBufferMaxLength = 1024;
        char pipeBuffer[pipeBufferMaxLength];
        bzero(pipeBuffer, pipeBufferMaxLength);
    }

    sslDestroy();
    EXIT_THREAD;
}

void runServer(int USE_FORK, int port)
{
    pthread_t acceptThread;

    flushStdout();
    sock = getListeningSocket(SERVER_IP, port);
    exitIfError(sock, "Couldn't create a socket to listen to.");

    pipe(listenPipe);

    THREAD(acceptThread, acceptIncomingConnections);

    if(setupDatabaseConnection() != DB_RETURN_SUCCES)
    {
        exit(EXIT_FAILURE);
    }

    size_t pipeBufferMaxLength = 1024;
    char pipeBuffer[pipeBufferMaxLength];
    bzero(pipeBuffer, pipeBufferMaxLength);
    for (;;)
    {
        if (read(listenPipe[0], pipeBuffer, pipeBufferMaxLength) > 0)
        {
            printf("Final boss received a message: %s\n", pipeBuffer);

            if (strncmp(pipeBuffer, "KILL_CHILD_PIPE", 15) == 0)
            {
                free(childPipeFd[pipeBuffer[17]]);
                childPipeFd[pipeBuffer[17]] = NULL;
                printf("Parent: free'd child %i\n", pipeBuffer[17]);
            }

            int i;
            for (i = 0; i < aantalChilderen + 1; i++)
            {
                if (childPipeFd[i] != NULL)
                {
                    write(childPipeFd[i][1], "Broadcast!!", 11);
                }
            }

            bzero(pipeBuffer, pipeBufferMaxLength);
        }
    }
    JOIN_THREAD(acceptThread);
}

int authenticateClient(commandStruct cmd)
{
    authenticated = BOOL_FALSE;
    if (commandEquals(cmd, "LOGIN"))
    {
        int result = handleLoginCommand(cmd);
        if (result == RPL_LOGIN)
        {
            authenticated = BOOL_TRUE;
        }
        sslSendInteger(result);
    }
    else
    {
        sslSendInteger(ERR_NOLOGIN);
    }
    return authenticated;
}

void readMessageFromParent()
{
    int bufferLength = 1024;
    char buffer[bufferLength];
    bzero(buffer, bufferLength);

    while (communicateWithClientThreadRunning == BOOL_TRUE)
    {
        if(read(childPipeFd[aantalChilderen][0], buffer, bufferLength) > 0)
        {
            printf("Child %i received message: %s\n", aantalChilderen, buffer);
            bzero(buffer, bufferLength);
        }
    }
    printf("Exit thread in child.\n");
    EXIT_THREAD;
}

void processConnectedClientWithFork(int clientSocket, struct sockaddr_in clientAddress)
{
    int childpid = fork();
    if (childpid == 0)
    {
        sslAcceptConnection(clientSocket, clientAddress);
        close(listenPipe[0]);
        sslSendInteger(RPL_CONNECTED);
        processConnectedClient();
        free(childPipeFd[aantalChilderen]);

        printf("CLOSED FORKED CLIENT\n");
        sslClose();
        exit(0);
    }
    exitIfError(childpid, "Error forking child");
}

void processConnectedClient()
{
    printf("Connection opened with client (%s:%i)\n", inet_ntoa(connection.address.sin_addr), connection.address.sin_port);
    sslSendInteger(RPL_CONNECTED);

    pthread_t tid;
    THREAD(tid, readMessageFromParent);

    int bufferLength = 1024;
    char buffer[bufferLength];
    bzero(buffer, (size_t)bufferLength);
    char newBuffer[bufferLength];
    bzero(newBuffer, bufferLength);

    while(sslRead(buffer, bufferLength) == SSL_OK && buffer[0] != '\0')
    {
        if (currentUser.username != NULL)
        {
            bzero(newBuffer, bufferLength);
            strcat(newBuffer, "#");
            strcat(newBuffer, currentUser.username);
            strcat(newBuffer, " ");
            strcat(newBuffer, buffer);

            write(listenPipe[1], newBuffer, bufferLength);
        }
        else
        {
            write(listenPipe[1], buffer, bufferLength);
        }
        bzero(buffer, sizeof(buffer));
    }

    communicateWithClientThreadRunning = BOOL_FALSE;
    write(childPipeFd[aantalChilderen][1], "CLEAN_THREAD_UP", 15);
    printf("Connection closed with client (%s:%i)\n", inet_ntoa(connection.address.sin_addr), connection.address.sin_port);
    authenticated = BOOL_FALSE;

    char *msgToParent = malloc(20);
    sprintf(msgToParent, "KILL_CHILD_PIPE %i", aantalChilderen);
    write(listenPipe[1], msgToParent, strlen(msgToParent));
    free(msgToParent);

    //freeCurrentUser();
    JOIN_THREAD(tid);

        /*
        if (authenticated == BOOL_FALSE)
        {
            commandStruct cmd = commandStruct_initialize(buffer);
            if (commandEquals(cmd, "CREATE_USER"))
            {
                result = handleCreateUserCommand(cmd);
                sslSendInteger(result);
            }
            else
            {
                authenticated = authenticateClient(cmd);
                if(authenticated == BOOL_TRUE)
                {
                    commandStruct pollCmd = commandStruct_initialize("POLL");
                    handlePollCommand(pollCmd, 100);
                    commandStruct_free(&pollCmd);
                }
            }
            commandStruct_free(&cmd);
        }
        else
        {
            int msg = parseMessage(buffer);

            if(msg != RPL_NOREPLY)
            {
                sslSendInteger(msg);
            }
        }
         */
}

void freeCurrentUser()
{
    userInfo_free(&currentUser);
}

int parseMessage(char *message)
{
    commandStruct cmd = commandStruct_initialize(message);
    int result = ERR_UNKNOWNCOMMAND;
    if (commandEquals(cmd, "LOGIN"))
    {
        result = ERR_ALREADY_LOGGED_IN;
    }
    else if (commandEquals(cmd, "JOIN"))
    {
        result = handleJoinCommand(cmd);
    }
    else if (commandEquals(cmd, "PRIVMSG"))
    {
        result = handlePrivateMessageCommand(cmd);
    }
    else if (commandEquals(cmd, "PART"))
    {
        result = handlePartCommand(cmd);
    }
    else if (commandEquals(cmd, "DELETE_USER"))
    {
        result = handleDeleteUserCommand();
        authenticated = BOOL_FALSE;
    }
    else if (commandEquals(cmd, "UPDATE_NICKNAME"))
    {
        result = handleUpdateNicknameCommand(cmd);
    }
    else if (commandEquals(cmd, "UPDATE_PASSWORD"))
    {
        result = handleUpdatePasswordCommand(cmd);
    }
    else if (commandEquals(cmd, "TOPIC"))
    {
        result = handleTopicCommand(cmd);
    }
    else if (commandEquals(cmd, "MODE"))
    {
        result = handleModeCommand(cmd);
    }
    else if (commandEquals(cmd, "POLL"))
    {
        result = handlePollCommand(cmd, 9999);
    }
    else if (commandEquals(cmd, "INVITE"))
    {
        result = handleInviteCommand(cmd);
    }
    else if (commandEquals(cmd, "NAMES"))
    {
        result = handleNamesCommand(cmd);
    }
    else if (commandEquals(cmd, "KICK"))
    {
        result = handleKickCommand(cmd);
    }
    else if (commandEquals(cmd, "UPDATE_CHANNEL_PASSWORD"))
    {
        result = handleUpdateChannelPasswordCommand(cmd);
    }
    commandStruct_free(&cmd);

    return result;
}
void flushStdout()
{
    setvbuf(stdout, NULL, _IONBF, 0);
}
int commandEquals(commandStruct cmd, char *check)
{
    return strcmp(cmd.command, check) == 0;
}
void exitIfError(ssize_t variableToCheckForError, char *errorMessage)
{
    if (variableToCheckForError < 0)
    {
        perror(errorMessage);
        exit(EXIT_FAILURE);
    }
}