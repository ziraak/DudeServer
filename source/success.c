#include "success.h"

void REPLY_JOIN(char *channelName, char *username)
{
    char *buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s %s", RPL_JOIN_CHANNEL, channelName, username);
    sendToAllClientsInChannel(buffer, channelName);
    FREE(buffer);
}

void REPLY_PART(char *channelName, char *username)
{
    char *buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s %s", RPL_PART_CHANNEL, channelName, username);
    sendToClient(getClientIdByUsername(username), buffer);
    sendToAllClientsInChannel(buffer, channelName);
    FREE(buffer);
}

void REPLY_PRIVATE_MESSAGE(messageInfo mi)
{
    char *buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s %s %ld :%s", RPL_PRIV_MSG, mi.channel, mi.user, mi.timestamp, mi.message);
    sendToAllClientsInChannel(buffer, mi.channel);
    FREE(buffer);
}

void REPLY_SUCCESS(commandStruct cmd)
{
    char *buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s", RPL_SUCCESS, cmd.command);
    sendToClient(cmd.client, buffer);
    FREE(buffer);
}

void REPLY_TOPIC(char *channelName, char *topic, int client)
{
    char *buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s :%s", RPL_TOPIC, channelName, topic);
    sendToClient(client, buffer);
    FREE(buffer);
}

void REPLY_TOPIC_ALL(char *channelName, char *topic)
{
    char *buffer = MALLOC(INNER_BUFFER_LENGTH);
    sprintf(buffer, "%i %s :%s", RPL_TOPIC, channelName, topic);
    sendToAllClientsInChannel(buffer, channelName);
    FREE(buffer);
}
