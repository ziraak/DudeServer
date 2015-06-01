#include "channel.h"

int countMessages(messageInfo *message)
{
}

void writeMessage(messageInfo message, char* channelName)
{
}
void deleteMessage(char* channelname)
{
}

int writeMessageToChannel(char *channelName, messageInfo message)
{
    return DB_RETURN_SUCCES;
}

int getChannel(char *channelName, channelInfo *channel)
{
    return DB_RETURN_SUCCES;
}

channelUser *getUsersFromChannel(char *channelName)
{
    channelUser* users = NULL;
    return users;
}

char* getUserRole(char* channelName, char* username)
{
    return NULL;
}

char **getChannellist()
{
    char **list;
    return list;
}

char **getVisibleChannels()
{
    char **key;
    return key;
}

int checkIfChannelVisible(char* channelName)
{
    return BOOL_FALSE;
}

int checkChannel(char *channelName)
{
}

int deleteChannelInDB(char *channelName)
{
}

int deleteChannelFromList(char *channelName)
{
}

int deleteUserFromChannel(char *channelName, char *username)
{
}

int checkIfChannelEmpty(char *channelName)
{
}

messageInfo *getMessages(char *channelName)
{
    return getMessagesOnTime(channelName, 0);
}

messageInfo *getMessagesOnTime(char *channelName, int timestamp)
{
}

void createNewChannel(char *channelName, char *password, char *topic, int visible)
{
}

void addChannelToList(char *channelName, int visible)
{
}

int checkIfChannelHasPassword(char *channelname)
{
    return BOOL_TRUE;
}

int authenticateChannelPassword(char *channelname, char *password)
{
    return BOOL_FALSE;
}

void newChannelPassword(char *channelName, char *newPass)
{
}

void newChannelTopic(char *channelName, char *newTopic)
{
}

void setChannelVisibility(char* channelname,int visible)
{
}

void setChannelUserRole(char* channelname, char* username, char* newRole)
{
}
