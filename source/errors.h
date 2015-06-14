//
// Created by osboxes on 14/06/15.
//

#ifndef DUDESERVER_ERRORS_H
#define DUDESERVER_ERRORS_H

#include "main.h"

#define ERROR_NOT_ON_CHANNEL(channel, username, client) _errorNotOnChannel(channel, username, client); return ERR_NOTONCHANNEL
#define ERROR_NO_SUCH_CHANNEL(channel, client) _errorNoSuchChannel(channel, client); return ERR_NOSUCHCHANNEL
#define ERROR_NEED_MORE_PARAMETERS(msg, count, client) _errorNeedMoreParameter(msg, count, client); return ERR_NEEDMOREPARAMS
#define ERROR_CHANNEL_PRIVILEGES_NEEDED(channel, client) _errorChannelPrivilegesNeeded(channel, client); return ERR_CHANOPPRIVSNEEDED
#define ERROR_PASSWORD_TOO_SHORT(client) _errorPasswordTooShort(client); return ERR_PASSWORDTOOSHORT
#define ERROR_USERNAME_NOT_KNOWN(username, client) _errorUsernameNotKnown(username, client); return ERR_USERNAME_NOT_KNOWN

void _errorNotOnChannel(char *channel, char *username, int client);
void _errorNoSuchChannel(char *channel, int client);
void _errorNeedMoreParameter(char *msg, int count, int client);
void _errorChannelPrivilegesNeeded(char* channel, int client);
void _errorPasswordTooShort(int client);
void _errorUsernameNotKnown(char* username, int client);

#endif //DUDESERVER_ERRORS_H
