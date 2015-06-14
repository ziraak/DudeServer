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

    int client_number;
    int client_active_number;
    User* clients;
} Record;

Record record;

void handleAccept()
{
    if(sslAcceptConnection(record.listen) == SSL_OK)
    {
        int clientfd[2];
        pipe(clientfd);

        User u = {
                .active = BOOL_TRUE,
                .authorized = BOOL_FALSE,
                .write = clientfd[1]
        };
        REALLOC(record.clients, sizeof(User) * (record.client_number + 1));
        record.clients[record.client_number] = u;
        record.client_number++;
        record.client_active_number++;

        printf("#%i: ACCEPTED\n", record.client_number - 1);

        handleClientProcess(record.client_write, clientfd[0], record.client_number - 1);
    }
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
    else if (commandEquals(cmd, "INVITE"))
    {
        result = handleInviteCommand(cmd);
    }
    else if (commandEquals(cmd, "KICK"))
    {
        result = handleKickCommand(cmd);
    }
    else if (commandEquals(cmd, "UPDATE_CHANNEL_PASSWORD"))
    {
        result = handleUpdateChannelPasswordCommand(cmd);
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

void handleUnauthorizedClient(commandStruct cmd)
{
    if(commandEquals(cmd, "LOGIN") == BOOL_TRUE)
    {
        char* buffer = MALLOC(INNER_BUFFER_LENGTH);

        int result = handleLoginCommand(cmd);
        if (result == RPL_LOGIN)
        {
            record.clients[cmd.sender].authorized = BOOL_TRUE;
            record.clients[cmd.sender].user = currentUser;

            sprintf(buffer, "%i %s", result, record.clients[cmd.sender].user.username);
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

int handleClient()
{
    int result = BOOL_FALSE;

    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    read(record.client_listen, buffer, INNER_BUFFER_LENGTH);
    commandStruct cmd = commandStruct_initialize(buffer);

    if(strcmp(cmd.command, "CLOSE") == 0)
    {
        close(record.clients[cmd.sender].write);
        record.clients[cmd.sender].write = -1;
        record.clients[cmd.sender].authorized = BOOL_FALSE;
        record.clients[cmd.sender].active = BOOL_FALSE;
        userInfo_free(&record.clients[cmd.sender].user);
        record.client_active_number--;

        if(record.client_active_number == 0)
        {
            result = BOOL_TRUE;
            printf("EXITING SERVER!\n");
        }
    }
    else
    {
        if(record.clients[cmd.sender].authorized == BOOL_TRUE)
        {
            handleAuthorizedClient(cmd);
        }
        else
        {
            handleUnauthorizedClient(cmd);
        }
    }

    commandStruct_free(&cmd);
    FREE(buffer);

    return result;
}

void runServer(int USE_FORK, int port)
{
    if(setupDatabaseConnection() != DB_RETURN_SUCCES)
    {
        return;
    }

    setvbuf(stdout, NULL, _IONBF, 0);

    int exitServer = BOOL_FALSE;
    int piped[2];
    pipe(piped);

    bzero(&record, sizeof(Record));
    record.clients = MALLOC(sizeof(User));
    record.client_listen = piped[0];
    record.client_write = piped[1];
    record.client_number = 0;
    record.listen = getListeningSocket(SERVER_IP, port);

    if(record.listen < 0)
    {
        perror("Nope!");
        return;
    }

    FD_SET(record.listen, &record.master);
    FD_SET(record.client_listen, &record.master);

    record.high = record.listen > record.client_listen ? record.listen : record.client_listen;

    printf("SERVER IS LISTENING...");

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
                    handleAccept();
                }
                else if(i == record.client_listen)
                {
                    exitServer = handleClient();
                }
            }
        }
    }

    close(record.listen);
    sslDestroy();
    stopDatabase();
}

void sendToClient(int client, char *message)
{
    if(client >= record.client_number || record.clients[client].active == BOOL_FALSE)
    {
        return;
    }

    char *msg = MALLOC(strlen(message) + 3);
    sprintf(msg, "%s\r\n", message);

    write(record.clients[client].write, msg, strlen(msg));

    FREE(msg);
}

void sendToAllClients(char *message)
{
    int i;
    for(i = 0; i < record.client_number; i++)
    {
        sendToClient(i, message);
    }
}

User* getClient(int client)
{
    if(client >= record.client_number)
    {
        return NULL;
    }

    return &record.clients[client];
}

int commandEquals(commandStruct cmd, char *check)
{
    return strcmp(cmd.command, check) == 0;
}