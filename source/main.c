#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils/utils.h"
#include "main.h"


int main(int argc, char **argv)
{
    flushStdout();
    char *server_ip = "127.0.0.1";
    struct sockaddr_in adres_server, adres_client;
    int sockfd;
    uint16_t listenPort = 9099;
    unsigned int clientlen;
    int sock = setupServer(&adres_server, listenPort, server_ip);

    listen(sock, 200);

    while(1)
    {
        clientlen = sizeof(adres_client);
        if ((sockfd = accept(sock, (struct sockaddr *) &adres_client, &clientlen)) > -1)
        {
            acknowledgeConnection(sockfd);
            processConnectedClient(sockfd);
        }
        else
        {
            printf("Server can' t accept a client.\n");
        }
    }

    return EXIT_SUCCESS;
}

void processConnectedClient(int sockfd) {
    ssize_t receive;
    char buffer[200];
    bzero(buffer, sizeof(buffer));

    while ((receive = recv(sockfd, buffer, sizeof(buffer), 0)) != EOF && buffer[0] != '\0') {
        if (receive < 0) {
            perror("Error recv");
            exit(1);
        }

        // getAllUnreadMessagesByName(); TODO: Username meegeven
        char* result = parseMessage(buffer);
        sendMessageToClient(sockfd, result, sizeof(result));

        bzero(buffer, sizeof(buffer));
    }

    close(sockfd);
}

char* parseMessage(char *message)
{
    char *command;

    char* result;
    int offset = substringCharacter(message, &command);

    if (commandEquals(command, "LOGIN"))
    {
        result = handleLoginCommand(message + offset);
    }
    else if (commandEquals(command, "JOIN"))
    {
        result = handleJoinCommand(message + offset);
    }
    else if (commandEquals(command, "PRIVMSG"))
    {
        result = handlePrivateMessageCommand(message + offset);
    }
    else
    {
        result = ERR_UNKNOWNCOMMAND;
    }

    return result;
}

int setupServer(struct sockaddr_in *adres_server, int listenPort, char *server_ip)
{
    int sock;
    adres_server->sin_family = AF_INET; // ip protocol
    adres_server->sin_port = htons(listenPort); // change to network byte order
    adres_server->sin_addr.s_addr = inet_addr(server_ip);


    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror("Socket error...");
        exit(1);
    }

    if(bind(sock, (struct sockaddr *) adres_server, sizeof(*adres_server)) < 0)
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

void sendMessageToClient(int sockfd, char *buffer, int bufferLength)
{
    if (send(sockfd, buffer, bufferLength, 0) < 0) {
        perror("Error send.. ");
        exit(1);
    }
}

void acknowledgeConnection(int sockfd)
{
    char *buffer = RPL_CONNECTED;
    sendMessageToClient(sockfd, buffer, 3);
}

int commandEquals(char* command, char* check)
{
    return strcmp(command, check) == 0;
}

int authenticateUser(char *username, char *password)
{
    // TODO: Check for user in DB
    return 0;
}

int findChannelByName(char *channelName)
{
    // TODO: Find channel by name
    return 0;
}

int authenticateChannel(char *channelName, char *optionalChannelKey)
{
    // TODO: Authenticate on the given channel
    return 0;
}

int joinChannel(char *channelName)
{
    // TODO: Add current user to the channel
    return 0;
}

int createChannel(char *channelName, char *optionalChannelKey)
{
    // TODO: create channel
    return 0;
}

int writeMessageToDB(char *recipient, char *msgToSend)
{
    // TODO: Write message to the file of given user
    return 0;
}

char** getAllUnreadMessagesByName(char *username)
{
    // TODO: Get all unread messages
    char** allUnreadMessages;
    return allUnreadMessages;
}

char* handleJoinCommand(char *message)
{
    char *channelName, *optionalChannelKey = NULL;
    int offset = substringCharacter(message, &channelName);
    if (!(*(message + offset) == '\n' || *(message + offset) == '\0'))
    {
        substringCharacter(message += offset, &optionalChannelKey);
    }

    int channelExists = findChannelByName(channelName);
    if (channelExists && optionalChannelKey != NULL)
    {
        if (authenticateChannel(channelName, optionalChannelKey))
        {
            // TODO: Join existing channel
            joinChannel(channelName);
        }
        else
        {
            // TODO: Not authenticated
            return ERR_BADCHANNELKEY;
        }
    }
    else
    {
        // TODO: Else create channel
        createChannel(channelName, optionalChannelKey);
    }

    return RPL_TOPIC;
}

char* handleLoginCommand(char *message)
{
    char *username, *password, *nickname;
    int offset = substringCharacter(message, &username);
    offset = substringCharacter(message += offset, &password);
    substringCharacter(message += offset, &nickname);

    int userAuthenticated = authenticateUser(username, password);
    if (userAuthenticated)
    {
        // TODO: Set id somewhere to know that this user is authenticated and may communicate with the server
        return RPL_LOGIN;
    }

    return ERR_NOLOGIN;
}

char* handlePrivateMessageCommand(char *message)
{
    char *recipient, *msgToSend;
    int offset = substringCharacter(message, &recipient);
    msgToSend = message += offset;
    writeMessageToDB(recipient, msgToSend);

    return RPL_AWAY;
}