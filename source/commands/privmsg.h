#ifndef DUDESERVER_PRIVMSG_H
#define DUDESERVER_PRIVMSG_H

#include <time.h>
#include "../main.h"
#include "../utils/utils.h"

int handlePrivateMessageCommand(commandStruct cmd);

int writeMessageToDB(char *msgToSend, char *channel, char *username);

#endif
