#ifndef DUDESERVER_CHANNELMESSAGES_H
#define DUDESERVER_CHANNELMESSAGES_H

#include "database.h"

int countMessages(char *channelName);
int insertMessage(messageInfo message, char *channelName);
void deleteMessage(char *channelName);

#endif //DUDESERVER_CHANNELMESSAGES_H
