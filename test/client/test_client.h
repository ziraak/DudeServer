#ifndef DUDESERVER_TEST_CLIENT_H
#define DUDESERVER_TEST_CLIENT_H

#include "test_client_main.h"

void clientBusinessSend();

void clientBusinessReceive();

int getServerSocket(int port, char *ip);

int input(size_t s, char **result);

int prompt(size_t s, char **result, char *message);

#endif //DUDESERVER_TEST_CLIENT_H
