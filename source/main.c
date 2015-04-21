#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils/utils.h"
#include "main.h"
#include "commands/login.h"
#include "commands/join.h"
#include "commands/privmsg.h"

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

    for(;;)
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

void processConnectedClient(int sockfd)
{
    int authenticated = 0;
    ssize_t receive;
    char buffer[200];
    bzero(buffer, sizeof(buffer));

    // TODO: wachten op login
    while ((receive = recv(sockfd, buffer, sizeof(buffer), 0)) != EOF && buffer[0] != '\0')
    {
        if(!authenticated)
        {
            // er MOET ingelogd zijn
            char *command;
            int offset = substringCharacter(buffer, &command);

            if(commandEquals(command, "LOGIN"))
            {
                int result = handleLoginCommand(buffer + offset);

                if(result == RPL_LOGIN)
                {
                    authenticated = 1;
                }

                authenticated = 1; // DIKKE TODO: zodra de database toegankelijk is moet deze weggehaald worden!!!!!11!!11111!!1!!11!
                sendIntegerMessageToClient(sockfd, result);
            }
            else
            {
                sendIntegerMessageToClient(sockfd, ERR_NOLOGIN);
            }
        }
        else
        {
            // ingelogd!
            if (receive < 0)
            {
                perror("Error recv");
                exit(1);
            }

            // getAllUnreadMessagesByName(); TODO: Username meegeven
            int result = parseMessage(buffer);
            sendIntegerMessageToClient(sockfd, result);

            bzero(buffer, sizeof(buffer));
        }
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

    return ERR_UNKNOWNCOMMAND;
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

void sendIntegerMessageToClient(int sockfd, int msg)
{
    char* dest = malloc(3);
    sprintf(dest, "%i", msg);
    sendMessageToClient(sockfd, dest, sizeof(dest));
    free(dest);
}

void sendMessageToClient(int sockfd, char *buffer, size_t bufferLength)
{
    if (send(sockfd, buffer, bufferLength, 0) < 0) {
        perror("Error send.. ");
        exit(1);
    }
}

void acknowledgeConnection(int sockfd)
{
    int buffer = RPL_CONNECTED;
    sendIntegerMessageToClient(sockfd, buffer);
}

int commandEquals(char* command, char* check)
{
    return strcmp(command, check) == 0;
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