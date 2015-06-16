#include "server.h"

typedef struct _clientRecord
{
    int clientListen;
    int clientNumber;
    int clientActiveNumber;
    User *clients;
} ClientRecord;

ClientRecord clientRecord;

void closeClientConnection(int client)
{
    if(client >= clientRecord.clientNumber)
    {
        return;
    }
    close(clientRecord.clients[client].write);
    userInfo_free(&clientRecord.clients[client].user);

    clientRecord.clients[client].authorized = BOOL_FALSE;
    clientRecord.clients[client].active = BOOL_FALSE;
    clientRecord.clients[client].write = BOOL_FALSE;
    clientRecord.clientActiveNumber--;

    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, CLIENT_MKFIFO_LOCATION, client);
    unlink(buffer);
    FREE(buffer);
}

void handleAuthorizedClient(commandStruct cmd)
{
    int result = ERR_UNKNOWNCOMMAND;
    if (commandEquals(cmd, "PRIVMSG"))
    {
        handlePrivateMessageCommand(cmd);
    }
    else if (commandEquals(cmd, "JOIN"))
    {
        handleJoinCommand(cmd);
    }
    else if (commandEquals(cmd, "PART"))
    {
        handlePartCommand(cmd);
    }
    else if (commandEquals(cmd, "DELETE_USER"))
    {
        handleDeleteUserCommand(cmd);
        closeClientConnection(cmd.sender);
    }
    else if (commandEquals(cmd, "UPDATE_NICKNAME"))
    {
        result = handleUpdateNicknameCommand(cmd); //TODO: server push compliant
    }
    else if (commandEquals(cmd, "UPDATE_PASSWORD"))
    {
        result = handleUpdatePasswordCommand(cmd); //TODO: server push compliant
    }
    else if (commandEquals(cmd, "TOPIC"))
    {
        result = handleTopicCommand(cmd); //TODO: server push compliant
    }
    else if (commandEquals(cmd, "MODE"))
    {
        result = handleModeCommand(cmd); //TODO: server push compliant
    }
    else if (commandEquals(cmd, "INVITE"))
    {
        result = handleInviteCommand(cmd); //TODO: server push compliant
    }
    else if (commandEquals(cmd, "KICK"))
    {
        result = handleKickCommand(cmd); //TODO: server push compliant
    }
    else if (commandEquals(cmd, "UPDATE_CHANNEL_PASSWORD"))
    {
        result = handleUpdateChannelPasswordCommand(cmd); //TODO: server push compliant
    }
    else if (commandEquals(cmd, "LOGIN"))
    {
        char *buffer = MALLOC(INNER_BUFFER_LENGTH);
        sprintf(buffer, "%i", ERR_ALREADY_LOGGED_IN);
        sendToClient(cmd.sender, buffer);
        FREE(buffer);
    }
    else
    {
        char *buffer = MALLOC(INNER_BUFFER_LENGTH);
        sprintf(buffer, "%i '%s'", ERR_ALREADY_LOGGED_IN, cmd.message);
        sendToClient(cmd.sender, buffer);
        FREE(buffer);
    }
}

/*
 * Login procedure.
 */
void handleUnauthorizedClient(commandStruct cmd)
{
    //TODO: CREATE_USER toevoegen
    if(commandEquals(cmd, "LOGIN") == BOOL_TRUE)
    {
        char* buffer = MALLOC(INNER_BUFFER_LENGTH);

        int result = handleLoginCommand(cmd);
        if (result == RPL_LOGIN)
        {
            sprintf(buffer, "%i %s", result, getClient(cmd.sender)->user.username);
        }
        else
        {
            sprintf(buffer, "%i", result);
        }
        sendToAllClients(buffer);
        FREE(buffer);

        handlePoll(cmd.sender, 100);
    }
    else
    {
        sendToClient(cmd.sender, "444");
    }
}

void handleClient(int acceptPid)
{
    if(setupDatabaseConnection() != DB_RETURN_SUCCES)
    {
        return;
    }

    char* buffer = MALLOC(INNER_BUFFER_LENGTH);

    int exitServer = BOOL_FALSE;
    while(exitServer == BOOL_FALSE)
    {
        if(read(clientRecord.clientListen, buffer, INNER_BUFFER_LENGTH) < 0)
        {
            perror("SERVER READ");
            continue;
        }

        commandStruct cmd = commandStruct_initialize(buffer);

        if(cmd.sender < 0)
        {
            continue;
        }

        if(strcmp(cmd.command, "ACCEPT") == 0)
        {
            // accept procedure: open the right MKFIFO to start communicating
            if(getClient(cmd.sender) == NULL)
            {
                int s = cmd.sender;

                if(s >= clientRecord.clientNumber)
                {
                    clientRecord.clientNumber = s + 1;
                    REALLOC(clientRecord.clients, sizeof(User) * clientRecord.clientNumber);
                }

                char* clientName = MALLOC(INNER_BUFFER_LENGTH);
                sprintf(clientName, CLIENT_MKFIFO_LOCATION, cmd.sender);

                User user = {
                    .authorized = BOOL_FALSE,
                    .active = BOOL_FALSE,
                    .write = open(clientName, O_WRONLY)
                };

                FREE(clientName);
                clientRecord.clients[s] = user;
                if(write(user.write, "ACCEPT", 6) < 0)
                {
                    perror("WRITE:");
                    exit(-4);
                }
            }
        }
        else if(strcmp(cmd.command, "ACTIVE") == 0)
        {
            // accept procedure: mark client as active
            User *user = getClient(cmd.sender);
            if(user != NULL)
            {
                user->active = BOOL_TRUE;
            }
        }
        else if(strcmp(cmd.command, "CLOSE") == 0)
        {
            // close client connection
            closeClientConnection(cmd.sender);

            if(clientRecord.clientActiveNumber == 0) // TODO: remove when live environment
            {
                exitServer = BOOL_TRUE;
                printf("EXITING SERVER!\n");
            }
        }
        else
        {
            if(cmd.sender < clientRecord.clientNumber && clientRecord.clients[cmd.sender].active)
            {
                if(clientRecord.clients[cmd.sender].authorized == BOOL_TRUE)
                {
                    if(getClient(cmd.sender) != NULL)
                    {
                        handleAuthorizedClient(cmd);
                    }
                }
                else
                {
                    handleUnauthorizedClient(cmd);
                }
            }
        }

        commandStruct_free(&cmd);
        bzero(buffer, INNER_BUFFER_LENGTH);
    }

    FREE(buffer);
    kill(acceptPid, SIGKILL);
    exit(0);
}

int handleAccept(int clientWrite, int port)
{
    int pid = fork();
    if(pid == 0)
    {
        return pid;
    }

    int listenSocket = getListeningSocket(SERVER_IP, port);
    int clientNumber = 0;

    if(listenSocket < 0)
    {
        perror("BINDING FAILED!");
        exit(-1);
    }

    printf("SERVER ACCEPTING CLIENTS ON PORT %i\n", port);

    while(1)
    {
        if(sslAcceptConnection(listenSocket) == SSL_OK)
        {
            if(PRINT_ALL == BOOL_TRUE)
            {
                printf("#%i: ACCEPTED\n", clientNumber);
            }

            char* clientName = MALLOC(INNER_BUFFER_LENGTH);
            sprintf(clientName, CLIENT_MKFIFO_LOCATION, clientNumber);
            if(mkfifo(clientName, 0666) < 0)
            {
                printf("S: ERROR CREATING NAMED PIPE %s:\n", clientName);
                perror("");
            }
            FREE(clientName);

            handleClientProcess(clientWrite, clientNumber);

            clientNumber++;
        }
    }

    exit(0);
}

void runServer(int port)
{
    setvbuf(stdout, NULL, _IONBF, 0);

    int piped[2];
    if(pipe(piped) < 0)
    {
        return;
    }

    printf("SERVER IS ACTIVE!\n");

    bzero(&clientRecord, sizeof(ClientRecord));

    int pid = handleAccept(piped[1], port);

    clientRecord.clientListen = piped[0];

    handleClient(pid);

    sslDestroy();
    stopDatabase();
}

/*
 * Send MESSAGE to CLIENT if CLIENT exists.
 */
void sendToClient(int client, char *message)
{
    if(client >= clientRecord.clientNumber || clientRecord.clients[client].active == BOOL_FALSE)
    {
        return;
    }

    char *msg = MALLOC(strlen(message) + 3);
    sprintf(msg, "%s\r\n", message);

    if(write(clientRecord.clients[client].write, msg, strlen(msg)) < 0)
    {
        perror("SERVER -> CLIENT WRITE");
    }

    if(PRINT_ALL == BOOL_TRUE)
    {
        printf("S -> #%i: '%s'\n", client, msg);
    }

    FREE(msg);
}

/*
 * Send MESSAGE to all currently existing clients.
 */
void sendToAllClients(char *message)
{
    int i;
    for(i = 0; i < clientRecord.clientNumber; i++)
    {
        sendToClient(i, message);
    }
}

/*
 * Get CLIENT if he exists, else NULL. DON'T FREE THE RESULT.
 */
User* getClient(int client)
{
    if(client >= clientRecord.clientNumber)
    {
        return NULL;
    }

    return &clientRecord.clients[client];
}

int commandEquals(commandStruct cmd, char *check)
{
    return strcmp(cmd.command, check) == 0;
}