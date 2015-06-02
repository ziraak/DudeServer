//
// Created by osboxes on 13/05/15.
//

#ifndef DUDESERVER_DATABASESTRUCTS_FREE_H
#define DUDESERVER_DATABASESTRUCTS_FREE_H

#include "../main.h"

void userInfo_free(userInfo *ui);
void channelUser_free(channelUser *cu);
void messageInfo_free(messageInfo *mi);
void channelInfo_free(channelInfo *ci);
void channelInfos_free(channelInfo *channels, int amount);
void messageInfos_free(messageInfo *messageInfoStruct, int amount);

#endif //DUDESERVER_DATABASESTRUCTS_FREE_H
