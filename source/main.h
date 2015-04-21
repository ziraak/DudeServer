//
// Created by osboxes on 21/04/15.
//

#ifndef DUDESERVER_MAIN_H
#define DUDESERVER_MAIN_H

#endif //DUDESERVER_MAIN_H

void processConnectedClient(int sockfd);
int setupServer(struct sockaddr_in *adres_server, int listenPort, char *server_ip);
void flushStdout();
void sendMessageToClient(int sockfd, char *buffer, int bufferLength);
char* parseMessage(char *message);
void acknowledgeConnection(int sockfd);
int commandEquals(char* command, char* check);
int authenticateUser(char *username, char *password);
int findChannelByName(char *channelName);
int authenticateChannel(char *channelName, char *optionalChannelKey);
int joinChannel(char *channelName);
int createChannel(char *channelName, char *optionalChannelKey);
int writeMessageToDB(char *recipient, char *msgToSend);

char* handleJoinCommand(char *message);
char* handleLoginCommand(char *message);
char* handlePrivateMessageCommand(char *message);

#define RPL_CONNECTED "100" // CONNECT
#define RPL_LOGIN "300" // LOGIN
#define RPL_AWAY "301" // PRIVMSG
#define RPL_TOPIC "301" // JOIN

#define ERR_UNKNOWNCOMMAND "421"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_NOLOGIN "444"
#define ERR_NICKNAMEINUSE "433"
#define ERR_NORECIPIENT "411"
#define ERR_NOTEXTTOSEND "412"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_NOSUCHNICK "401"
#define ERR_BANNEDFROMCHAN "474"
#define ERR_BADCHANNELKEY "475"
#define ERR_CHANNELISFULL "471"
#define ERR_BADCHANMASK "476"
#define ERR_NOSUCHCHANNEL "403"