//
// Created by osboxes on 16/06/15.
//

#include "success.h"

void REPLY_JOIN(char *channelName, char *username)
{
    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s %s", RPL_JOIN_CHANNEL, channelName, username);
    sendToAllClientsInChannel(buffer, channelName);
    FREE(buffer);
}

void REPLY_PART(char *channelName, char *username)
{
    char* buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s %s", RPL_PART_CHANNEL, channelName, username);
    sendToClient(getClientIdByUsername(username), buffer);
    sendToAllClientsInChannel(buffer, channelName);
    FREE(buffer);
}