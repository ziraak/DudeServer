#ifndef DUDESERVER_LOGIN_H
#define DUDESERVER_LOGIN_H
#include "../main.h"
#include "../utils/utils.h"
#include "../database/database.h"

int handleLoginCommand(commandStruct cmd);

int authenticateUser(char *username, char *password, userInfo *result);
#endif
