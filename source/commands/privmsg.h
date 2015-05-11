#ifndef DUDESERVER_PRIVMSG_H
#define DUDESERVER_PRIVMSG_H

#include "../main.h"
#include "../utils/utils.h"

int handlePrivateMessageCommand(char *message);

int writeMessageToDB(char *msgToSend, char *channel);

#endif
