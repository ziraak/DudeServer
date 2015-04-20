#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>

// extra includes
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>


int main()
{
    int sock, connectCheck;
    char *server_ip = "127.0.0.1";
    struct sockaddr_in adres_server, adres_client;
    int sockfd;
    int listenport = atoi(8080); // char array to int
    unsigned int clientlen;


    adres_server.sin_family = AF_INET; // ip protocol
    adres_server.sin_port = htons(listenport); // change to network byte order
    adres_server.sin_addr.s_addr = inet_addr(server_ip);


    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("Socket error...");
        exit(1);
    }

    if((connectCheck = bind(sock, (struct sockaddr *) &adres_server, sizeof(adres_server))) < 0) {
        perror("Bind error...");
        exit(1);
    }

    listen(sock, 200);


    return 0;
}
