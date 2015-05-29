//
// Created by osboxes on 27/05/15.
//

#ifndef DUDESERVER_MODE_H
#define DUDESERVER_MODE_H

#include "../main.h"

typedef struct flagStruct
{
    int set;
    char flag;
    char *parameter;
} flagStruct;

typedef struct modeStruct
{
    int error;
    char* channelName;
    int flagCount;
    flagStruct* flags;
} modeStruct;

int handleModeCommand(commandStruct cmd);

void modeStruct_free(modeStruct *ms);;

#endif //DUDESERVER_MODE_H
