//
// Created by osboxes on 16/06/15.
//

#ifndef DUDESERVER_SUCCESS_H
#define DUDESERVER_SUCCESS_H

#include "server.h"

void REPLY_JOIN(char *channelName, char *username);
void REPLY_PART(char *channelName, char *username);
void REPLY_PRIVATE_MESSAGE(messageInfo mi);
void REPLY_SUCCESS(commandStruct cmd);

#endif //DUDESERVER_SUCCESS_H
