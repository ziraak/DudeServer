//
// Created by osboxes on 13/06/15.
//

#ifndef DUDESERVER_CLIENT_H
#define DUDESERVER_CLIENT_H

#define CLIENT_MKFIFO_LOCATION "clients/client%i"

#include "main.h"

void handleClientProcess(int writeSocket, int number);

#endif //DUDESERVER_CLIENT_H
