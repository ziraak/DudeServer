#include "server.h"

int authenticated = BOOL_FALSE;

typedef struct _record
{
    fd_set master;
    fd_set read;

    int high;
    int listen;

    int client_listen;
    int client_write;

    int* clients;
    int client_number;
    int client_active_number;
} Record;

Record record;

void runServer(int USE_FORK, int port)
{
    if(setupDatabaseConnection() != DB_RETURN_SUCCES)
    {
        return;
    }

    flushStdout();

    int exitServer = BOOL_FALSE;
    int piped[2];
    pipe(piped);

    bzero(&record, sizeof(Record));
    record.clients = MALLOC(sizeof(int));
    record.client_listen = piped[0];
    record.client_write = piped[1];
    record.client_number = 0;
    record.listen = getListeningSocket(SERVER_IP, port);
    exitIfError(record.listen, "Couldn't create a socket to listen to.");

    FD_SET(record.listen, &record.master);
    FD_SET(record.client_listen, &record.master);

    printf("ACCEPT: %i | LISTEN: %i\n", record.listen, record.client_listen);

    record.high = record.listen > record.client_listen ? record.listen : record.client_listen;

    while(exitServer == BOOL_FALSE)
    {
        record.read = record.master;

        select(record.high + 1, &record.read, NULL, NULL, NULL);
        int i = 0;

        for(i = 0; i <= record.high; i++)
        {
            if(FD_ISSET(i, &record.read))
            {
                if(i == record.listen)
                {
                    if(sslAcceptConnection(record.listen) == SSL_OK)
                    {
                        int clientfd[2];
                        pipe(clientfd);
                        record.clients[record.client_number] = clientfd[1];
                        record.client_number++;
                        record.client_active_number++;

                        handleClientProcess(record.client_write, clientfd[0], record.client_number - 1);
                    }
                }
                else if(i == record.client_listen)
                {
                    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
                    read(record.client_listen, buffer, INNER_BUFFER_LENGTH);
                    commandStruct cmd = commandStruct_initialize(buffer);

                    if(strcmp(cmd.command, "CLOSE") == 0)
                    {
                        int c_number = atoi(cmd.parameters[0]);
                        printf("#%i: CLOSED\n", c_number);
                        close(record.clients[c_number]);
                        record.clients[c_number] = -1;
                        record.client_active_number--;

                        if(record.client_active_number == 0)
                        {
                            exitServer = BOOL_TRUE;
                            printf("EXITING SERVER!\n");
                        }
                    }
                    else
                    {
                        int j;
                        for(j = 0; j < record.client_number; j++)
                        {
                            if(record.clients[j] >= 1)
                            {
                                write(record.clients[j], buffer, INNER_BUFFER_LENGTH);
                            }
                        }
                    }

                    commandStruct_free(&cmd);
                    FREE(buffer);
                }
            }
        }
    }

    sslDestroy();
    stopDatabase();
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
void processConnectedClient()
{
    printf("Connection opened with client (%s:%i)\n", inet_ntoa(connection.address.sin_addr), connection.address.sin_port);
    sslSendInteger(RPL_CONNECTED);
    int bufferLength = 1024, result;
    char buffer[bufferLength];
    bzero(buffer, (size_t)bufferLength);
    char newBuffer[bufferLength];
    bzero(newBuffer, bufferLength);
    while(sslRead(buffer, bufferLength) == SSL_OK && buffer[0] != '\0')
    {
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
        bzero(buffer, sizeof(buffer));
    }
    printf("Connection closed with client (%s:%i)\n", inet_ntoa(connection.address.sin_addr), connection.address.sin_port);
    authenticated = BOOL_FALSE;
    freeCurrentUser();
    sslClose();
}

void freeCurrentUser()
{
    userInfo_free(&currentUser);
}

void processConnectedClientWithFork()
{
    int childpid = fork();
    if (childpid == 0)
    {
        processConnectedClient();
        printf("CLOSED FORKED CLIENT\n");
        exit(0);
    }
    exitIfError(childpid, "Error forking child");
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