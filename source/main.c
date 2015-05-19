#include "main.h"

int authenticated = BOOL_FALSE;

int main(int argc, char **argv)
{
    runServer();

    return EXIT_SUCCESS;
}

void runServer()
{
    flushStdout();
    struct sockaddr_in adres_client;
    int sockfd;
    unsigned int clientlen;
    int sock = setupServer();

    listen(sock, 200);

// Deze regels zorgen ervoor dat de IDE niet inspecteert op de infinite loop hieronder en geen warning geeft. De server moet een infinite loop hebben.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (; ;)
    {
        clientlen = sizeof(adres_client);
        sockfd = accept(sock, (struct sockaddr *) &adres_client, &clientlen);
        if (sockfd > -1)
        {
            processConnectedClient(sockfd, adres_client); // Gebruik geen fork als je wilt debuggen!! Debugger kan je niet attachen aan andere processen behalve de parent.
        }
        else
        {
            perror("Server can' t accept connection with a client.\n");
        }
    }
#pragma clang diagnostic pop
}

int setupServer()
{
    char *server_ip = "127.0.0.1";
    uint16_t listenPort = 9090;
    struct sockaddr_in adres_server;
    int sock, bindResult;
    adres_server.sin_family = AF_INET; // ip protocol
    adres_server.sin_port = htons(listenPort); // change to network byte order
    adres_server.sin_addr.s_addr = inet_addr(server_ip);
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    exitIfError(sock, "Socket failed while trying to start the server.");
    bindResult = bind(sock, (struct sockaddr *) &adres_server, sizeof(adres_server));
    exitIfError(bindResult, "Binding to the socket failed while starting the server.");

    return sock;
}

void processConnectedClientWithFork(int sockfd, struct sockaddr_in adres_client)
{
    int childpid = fork();
    if (childpid == 0)
    {
        processConnectedClient(sockfd, adres_client);
    }
    exitIfError(childpid, "Error forking child");
}

void processConnectedClient(int sockfd, struct sockaddr_in adres_client)
{
    ssize_t receive;
    char buffer[200];
    bzero(buffer, sizeof(buffer));
    int result;

    printf("Connection accepted with client: IP %s client port %i\n", inet_ntoa(adres_client.sin_addr),
           ntohs(adres_client.sin_port));
    acknowledgeConnection(sockfd);

    while ((receive = recv(sockfd, buffer, sizeof(buffer), 0)) != EOF && buffer[0] != '\0')
    {
        exitIfError(receive, "Error receiving message from client.");

        if (authenticated == BOOL_FALSE)
        {
            commandStruct cmd;
            parseCommand(buffer, &cmd);

            if (commandEquals(cmd, "CREATE_USER"))
            {
                result = handleCreateUserCommand(cmd);
                sendIntegerMessageToClient(sockfd, result);
            }
            else
            {
                authenticated = authenticateClient(sockfd, cmd);

                if(authenticated == BOOL_TRUE)
                {
                    commandStruct pollCmd;
                    parseCommand("POLL 1400000000", &pollCmd);
                    handlePollCommand(pollCmd, sockfd); //1431349399
                    commandStruct_free(&pollCmd);
                }
            }
        }
        else
        {
            result = parseMessage(buffer, sockfd);
            sendIntegerMessageToClient(sockfd, result);
        }

        bzero(buffer, sizeof(buffer));
    }

    close(sockfd);
    printf("Connection closed with client: IP %s\n", inet_ntoa(adres_client.sin_addr));
    exit(EXIT_SUCCESS);
}

int authenticateClient(int sockfd, commandStruct cmd)
{
    authenticated = BOOL_FALSE;

    if (commandEquals(cmd, "LOGIN"))
    {
        int result = handleLoginCommand(cmd);

        if (result == RPL_LOGIN)
        {
            authenticated = BOOL_TRUE;
        }
        sendIntegerMessageToClient(sockfd, result);
    }
    else
    {
        sendIntegerMessageToClient(sockfd, ERR_NOLOGIN);
    }
    return authenticated;
}

int parseMessage(char *message, int sockfd)
{
    // TODO: sockfd moet hier weg?? moest even voor POLL
    commandStruct cmd;
    parseCommand(message, &cmd);

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
    else if (commandEquals(cmd, "POLL") && cmd.parameterCount > 0)
    {
        result = handlePollCommand(cmd, sockfd);
    }

    commandStruct_free(&cmd);

    return result;
}

void flushStdout()
{
    setvbuf(stdout, NULL, _IONBF, 0);
}

void sendIntegerMessageToClient(int sockfd, int msg)
{
    char *dest = malloc(3);
    sprintf(dest, "%i", msg);
    sendMessageToClient(sockfd, dest);
    free(dest);
}

void sendMessageToClient(int sockfd, char *buffer)
{
    size_t bufferLength;
    if(sockfd < 0 || buffer == NULL || (bufferLength = strlen(buffer)) == 0)
    {
        return;
    }

    char* snd = malloc(sizeof(char) * bufferLength + 2);
    sprintf(snd, "%s\r\n", buffer);
    ssize_t sendResult = send(sockfd, snd, bufferLength + 2, 0);

    struct timespec remaining;
    remaining.tv_nsec = sendWait.tv_nsec = 500;
    remaining.tv_sec = sendWait.tv_sec;

    nanosleep(&sendWait, &remaining);
    free(snd);

    exitIfError(sendResult, "Error while sending a message to the client.");
}

void acknowledgeConnection(int sockfd)
{
    int buffer = RPL_CONNECTED;
    sendIntegerMessageToClient(sockfd, buffer);
}

int commandEquals(commandStruct cmd, char *check)
{
    return strcmp(cmd.command, check) == 0;
}

int generateToken()
{
    return rand();
}

void exitIfError(ssize_t variableToCheckForError, char *errorMessage)
{
    if (variableToCheckForError < 0)
    {
        perror(errorMessage);
        exit(EXIT_FAILURE);
    }
}