#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void processConnectedClient(int sockfd);
int setupServer(struct sockaddr_in *adres_server, int listenPort, char *server_ip);
void flushStdout();
void sendMessageToClient(int sockfd, char *buffer, int bufferLength);
void parseMessage(char *message);
void acknowledgeConnection(int sockfd);
#define RPL_CONNECTED "100"

int main(int argc, char **argv)
{
    flushStdout();
    char *server_ip = "127.0.0.1";
    struct sockaddr_in adres_server, adres_client;
    int sockfd;
    uint16_t listenPort = 9098;
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
            printf("Server can' t accept one client.\n");
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

        // parse message
        parseMessage(buffer);

        bzero(buffer, sizeof(buffer));
    }

    close(sockfd);
}

void parseMessage(char *message)
{

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