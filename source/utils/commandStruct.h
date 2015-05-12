//
// Created by osboxes on 12/05/15.
//

#ifndef DUDESERVER_COMMANDSTRUCT_H
#define DUDESERVER_COMMANDSTRUCT_H

typedef struct commandStruct
{
    char *command;
    char *trailing;
    char **parameters;
    int parameterCount;
} commandStruct;

#endif //DUDESERVER_COMMANDSTRUCT_H
