//
// Created by osboxes on 12/05/15.
//

#ifndef DUDESERVER_COMMANDSTRUCT_H
#define DUDESERVER_COMMANDSTRUCT_H

typedef struct commandStruct
{
    int client;
    char *command;
    char *trailing;
    char *message;
    char **parameters;
    int parameterCount;
} commandStruct;

#endif //DUDESERVER_COMMANDSTRUCT_H
