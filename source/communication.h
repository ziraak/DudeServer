//
// Created by osboxes on 19/05/15.
//

#ifndef DUDESERVER_COMMUNICATION_H
#define DUDESERVER_COMMUNICATION_H

#include "main.h"

void sendIntegerMessageToClient(int sockfd, int msg);
void sendMessageToClient(int sockfd, char *buffer);

#endif //DUDESERVER_COMMUNICATION_H
