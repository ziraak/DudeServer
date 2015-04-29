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

    for (; ;)
    {
        clientlen = sizeof(adres_client);
        if ((sockfd = accept(sock, (struct sockaddr *) &adres_client, &clientlen)) > -1)
        {
            printf("Connection accepted with client: IP %s client port %i\n", inet_ntoa(adres_client.sin_addr),
                   ntohs(adres_client.sin_port));

            int childpid = fork();
            if (childpid == 0)
            {
                acknowledgeConnection(sockfd);
                processConnectedClient(sockfd);
                close(sockfd);
                printf("Connection closed with client: IP %s\n", inet_ntoa(adres_client.sin_addr));
                // exit child
                exit(0);
            }
            else if (childpid < 0)
            {
                perror("Fork error");
                exit(1);
            }
        }
        else
        {
            printf("Server can' t accept a client.\n");
        }
    }

    return EXIT_SUCCESS;
}

void processConnectedClient(int sockfd)
{
    ssize_t receive;
    char buffer[200];
    bzero(buffer, sizeof(buffer));

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
                    printf("LOGGED IN: %s\n", currentUser.username);
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
                exit(1);
            }

            // getAllUnreadMessagesByName(); TODO: Username meegeven
            int result = parseMessage(buffer);
            sendIntegerMessageToClient(sockfd, result);
        }

        bzero(buffer, sizeof(buffer));
    }

    close(sockfd);
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
        exit(1);
    }

    if (bind(sock, (struct sockaddr *) &adres_server, sizeof(adres_server)) < 0)
    {
        perror("Bind error...");
        exit(1);
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
        exit(1);
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