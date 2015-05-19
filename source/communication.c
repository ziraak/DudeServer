//
// Created by osboxes on 19/05/15.
//

#include "communication.h"

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

    if (sendResult < 0)
    {
        perror("Error while sending a message to the client.");
        exit(EXIT_FAILURE);
    }
}
