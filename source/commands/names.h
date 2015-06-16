#ifndef DUDESERVER_NAMES_H
#define DUDESERVER_NAMES_H

#include "../main.h"
#include "../utils/utils.h"
#include "../database/database.h"

//int handleNamesCommand(commandStruct cmd);

void handleChannelNames(char* channelName, int client);
void handleNames(int client);

#endif //DUDESERVER_NAMES_H
