#include "main.h"

userInfo currentUser;
int authenticated = BOOL_FALSE;

int main(int argc, char **argv)
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
        if ((sockfd = accept(sock, (struct sockaddr *) &adres_client, &clientlen)) > -1)
        {
            procesConnectedClientWithFork(sockfd, adres_client);
        }
        else
        {
            printf("Server can' t accept a client.\n");
        }
    }
#pragma clang diagnostic pop

    return EXIT_SUCCESS;
}

void procesConnectedClientWithFork(int sockfd, struct sockaddr_in adres_client)
{
    int childpid = fork();
    if (childpid == 0)
    {
        processConnectedClient(sockfd, adres_client);
    }
    else if (childpid < 0)
    {
        perror("Fork error");
        exit(EXIT_FAILURE);
    }
}

void processConnectedClient(int sockfd, struct sockaddr_in adres_client)
{
    ssize_t receive;
    char buffer[200];
    bzero(buffer, sizeof(buffer));

    printf("Connection accepted with client: IP %s client port %i\n", inet_ntoa(adres_client.sin_addr),
           ntohs(adres_client.sin_port));
    acknowledgeConnection(sockfd);

    while ((receive = recv(sockfd, buffer, sizeof(buffer), 0)) != EOF && buffer[0] != '\0')
    {
        if (authenticated == BOOL_FALSE)
        {
            char *command;
            int offset = substringCharacter(buffer, &command);

            if (commandEquals(command, "LOGIN"))
            {
                int result = handleLoginCommand(buffer + offset);

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
        }
        else
        {
            if (receive < 0)
            {
                perror("Error recv");
                exit(EXIT_FAILURE);
            }

            // getAllUnreadMessagesByName(); TODO: Username meegeven
            int result = parseMessage(buffer);
            sendIntegerMessageToClient(sockfd, result);
        }

        bzero(buffer, sizeof(buffer));
    }

    close(sockfd);
    printf("Connection closed with client: IP %s\n", inet_ntoa(adres_client.sin_addr));
    exit(EXIT_SUCCESS);
}

int parseMessage(char *message)
{
    char *command;

    int offset = substringCharacter(message, &command);

    if (commandEquals(command, "JOIN"))
    {
        return handleJoinCommand(message + offset);
    }
    else if (commandEquals(command, "PRIVMSG"))
    {
        return handlePrivateMessageCommand(message + offset);
    }
    else if (commandEquals(command, "PART"))
    {
        return handlePartCommand(message + offset);
    }

    return ERR_UNKNOWNCOMMAND;
}

int setupServer()
{
    char *server_ip = "127.0.0.1";
    uint16_t listenPort = 9090;
    struct sockaddr_in adres_server;
    int sock;
    adres_server.sin_family = AF_INET; // ip protocol
    adres_server.sin_port = htons(listenPort); // change to network byte order
    adres_server.sin_addr.s_addr = inet_addr(server_ip);


    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror("Socket error...");
        exit(EXIT_FAILURE);
    }

    if (bind(sock, (struct sockaddr *) &adres_server, sizeof(adres_server)) < 0)
    {
        perror("Bind error...");
        exit(EXIT_FAILURE);
    }

    return sock;
}

void flushStdout()
{
    setvbuf(stdout, NULL, _IONBF, 0);
}

void sendIntegerMessageToClient(int sockfd, int msg)
{
    char *dest = malloc(3);
    sprintf(dest, "%i", msg);
    sendMessageToClient(sockfd, dest, sizeof(dest));
    free(dest);
}

void sendMessageToClient(int sockfd, char *buffer, size_t bufferLength)
{
    if (send(sockfd, buffer, bufferLength, 0) < 0)
    {
        perror("Error send.. ");
        exit(EXIT_FAILURE);
    }
}

void acknowledgeConnection(int sockfd)
{
    int buffer = RPL_CONNECTED;
    sendIntegerMessageToClient(sockfd, buffer);
}

int commandEquals(char *command, char *check)
{
    return strcmp(command, check) == 0;
}

int writeMessageToDB(char *recipient, char *msgToSend)
{
    // TODO: Write message to the file of given user
    return 0;
}

char **getAllUnreadMessagesByName(char *username)
{
    // TODO: Get all unread messages
    char **allUnreadMessages;
    return allUnreadMessages;
}

int generateToken()
{
    return rand();
}