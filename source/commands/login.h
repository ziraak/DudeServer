#ifndef DUDESERVER_LOGIN_H
#define DUDESERVER_LOGIN_H
#include "../main.h"
#include "../utils/utils.h"
#include "../medium.h"

int handleLoginCommand(char *message);

int authenticateUser(char *username, char *password);
#endif
