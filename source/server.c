#include "server.h"

int authenticated = BOOL_FALSE;

void runServer(int USE_FORK, int port)
{
    flushStdout();
    int sock = getListeningSocket(SERVER_IP, SERVER_PORT);
    exitIfError(sock, "Couldn't create a socket to listen to.");

    if(USE_FORK == BOOL_TRUE)
    {
// Deze regels zorgen ervoor dat de IDE niet inspecteert op de infinite loop hieronder en geen warning geeft. De server moet een infinite loop hebben.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
        printf("ACCEPTING MULTIPLE CLIENTS\n");
        for (; ;)
        {
            if(sslAcceptConnection(sock) == SSL_OK)
            {
                processConnectedClientWithFork();
            }
        }
#pragma clang diagnostic pop
    }
    else
    {
        printf("ACCEPTING A SINGLE CLIENT\n");
        if(sslAcceptConnection(sock) == SSL_OK)
        {
            processConnectedClient();
        }
    }
    sslDestroy();
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
    int bufferLength = 256, result;
    char buffer[bufferLength];
    bzero(buffer, (size_t)bufferLength);
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
                    commandStruct pollCmd = commandStruct_initialize("POLL 0");
                    handlePollCommand(pollCmd);
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
    free(currentUser.username);
    free(currentUser.nickname);
    free(currentUser.password);
}

void processConnectedClientWithFork()
{
    int childpid = fork();
    if (childpid == 0)
    {
        processConnectedClient();
        exit(0);
    }
    exitIfError(childpid, "Error forking child");
}

int parseMessage(char *message)
{
    commandStruct cmd = commandStruct_initialize(message);
    int result = ERR_UNKNOWNCOMMAND;
    if (commandEquals(cmd, "LOGIM"))
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
        result = handlePollCommand(cmd);
    }
    else if (commandEquals(cmd, "INVITE"))
    {
        result = handleInviteCommand(cmd);
    }
    else if (commandEquals(cmd, "NAMES"))
    {
        result = handleNamesCommand(cmd);
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