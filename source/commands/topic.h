#ifndef DUDESERVER_TOPIC_H
#define DUDESERVER_TOPIC_H

#include "../main.h"
#include "../utils/utils.h"
#include "../database/database.h"

int handleTopicCommand(commandStruct cmd);
void handleTopic(int client);
void handleChannelTopic(char* channelName, int client);

#endif //DUDESERVER_TOPIC_H
