#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils/utils.h"
#include "commands/login.h"
#include "commands/join.h"
#include "commands/privmsg.h"

void processConnectedClient(int sockfd);

int setupServer();

void flushStdout();

void sendIntegerMessageToClient(int sockfd, int msg);

void sendMessageToClient(int sockfd, char *buffer, size_t bufferLength);

int parseMessage(char *message);

void acknowledgeConnection(int sockfd);

int commandEquals(char *command, char *check);

int writeMessageToDB(char *recipient, char *msgToSend);

#define RPL_CONNECTED 100 // CONNECT
#define RPL_LOGIN 300 // LOGIN
#define RPL_AWAY 301 // PRIVMSG
#define RPL_TOPIC 303 // JOIN

#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NICKNAMEINUSE 433
#define ERR_NOLOGIN 444
#define ERR_NEEDMOREPARAMS 461
#define ERR_CHANNELISFULL 471
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANMASK 476

#define BOOL_TRUE 1
#define BOOL_FALSE 0

#endif