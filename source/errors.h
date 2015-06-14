//
// Created by osboxes on 14/06/15.
//

#ifndef DUDESERVER_ERRORS_H
#define DUDESERVER_ERRORS_H

#include "main.h"

#define ERROR_NEED_MORE_PARAMETERS(msg, count, client) _errorNeedMoreParameter(msg, count, client); return ERR_NEEDMOREPARAMS
#define ERROR_CHANNEL_PRIVILEGES_NEEDED(channel, client) _errorChannelPrivilegesNeeded(channel, client); return ERR_CHANOPPRIVSNEEDED
#define ERROR_PASSWORD_TOO_SHORT(client) _errorPasswordTooShort(client); return ERR_PASSWORDTOOSHORT
#define ERROR_USERNAME_NOT_KNOWN(username, client) _errorUsernameNotKnown(username, client); return ERR_USERNAME_NOT_KNOWN

void _errorNeedMoreParameter(char *msg, int count, int client);
void _errorChannelPrivilegesNeeded(char* channel, int client);
void _errorPasswordTooShort(int client);
void _errorUsernameNotKnown(char* username, int client);

#endif //DUDESERVER_ERRORS_H
