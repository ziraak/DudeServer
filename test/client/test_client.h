#ifndef DUDESERVER_TEST_CLIENT_H
#define DUDESERVER_TEST_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../source/utils/utils.h"

#define CMD_SIZE 256

void clientBusinessSend(int sock);
void clientBusinessReceive(int sock);

int getServerSocket(int port, char *ip);

int input(size_t s, char **result);

int prompt(size_t s, char **result, char *message);

#endif //DUDESERVER_TEST_CLIENT_H
