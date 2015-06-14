//
// Created by osboxes on 14/06/15.
//

#ifndef DUDESERVER_ERRORS_H
#define DUDESERVER_ERRORS_H

#include "main.h"

#define ERROR_NEED_MORE_PARAMETERS(msg, count, client) _errorNeedMoreParameter(msg, count, client); return ERR_NEEDMOREPARAMS
#define ERROR_CHANNEL_PRIVILEGES_NEEDED(channel, client) _errorChannelPrivilegesNeeded(channel, client); return ERR_CHANOPPRIVSNEEDED

void _errorNeedMoreParameter(char *msg, int count, int client);
void _errorChannelPrivilegesNeeded(char* channel, int client);

#endif //DUDESERVER_ERRORS_H
