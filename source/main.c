#include "main.h"

int authenticated = BOOL_FALSE;

int main(int argc, char **argv)
{
    int fork = BOOL_FALSE;
    if(argc > 1)
    {
        if(strcmp(argv[1], "FORK") == 0 || strcmp(argv[1], "fork") == 0 || strcmp(argv[1], "-f") == 0)
        {
            fork = BOOL_TRUE;
        }
    }

    runServer(fork);

    return EXIT_SUCCESS;
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
    sslSendInteger(RPL_CONNECTED); // acknownledge connection

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
                    commandStruct pollCmd = commandStruct_initialize("POLL 1431349400");
                    handlePollCommand(pollCmd); // TODO: socket reference weghalen //1431349399
                    commandStruct_free(&pollCmd);
                }
            }
        }
        else
        {
            sslSendInteger(parseMessage(buffer)); // TODO: socket reference weghalen
        }

        bzero(buffer, sizeof(buffer));
    }

    printf("Connection closed with client (%s:%i)\n", inet_ntoa(connection.address.sin_addr), connection.address.sin_port);
    sslClose();
}

void processConnectedClientWithFork()
{
    int childpid = fork();
    if (childpid == 0)
    {
        processConnectedClient();
    }
    exitIfError(childpid, "Error forking child");
}

void runServer(int fork)
{
    flushStdout();
    int sock = getListeningSocket(SERVER_IP, SERVER_PORT);

    exitIfError(sock, "Couldn't create a socket to listen to.");

// Deze regels zorgen ervoor dat de IDE niet inspecteert op de infinite loop hieronder en geen warning geeft. De server moet een infinite loop hebben.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (; ;)
    {
        if(sslAcceptConnection(sock) == SSL_OK)
        {
            if(fork == BOOL_TRUE)
            {
                processConnectedClientWithFork();
            }
            else
            {
                processConnectedClient();
            }
        }
    }
#pragma clang diagnostic pop

    sslDestroy();
}

int parseMessage(char *message)
{
    // TODO: sockfd moet hier weg?? moest even voor POLL
    commandStruct cmd = commandStruct_initialize(message);

    int result = ERR_UNKNOWNCOMMAND;

    if (commandEquals(cmd, "JOIN"))
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
        result = handleDeleteUserCommand(); // TODO: nog steeds authenticated
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
    else if(commandEquals(cmd, "TOPIC"))
    {
        result = handleTopicCommand(cmd);
    }
    else if (commandEquals(cmd, "POLL"))
    {
        result = handlePollCommand(cmd);
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