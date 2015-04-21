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
void parseMessage(char *message);
void acknowledgeConnection(int sockfd);
int commandEquals(char* command, char* check);
int authenticateUser(char *username, char *password);
int findChannelByName(char *channelName);
int authenticateChannel(char *channelName, char *optionalChannelKey);
int joinChannel(char *channelName);
int createChannel(char *channelName, char *optionalChannelKey);
int writeMessageToDB(char *recipient, char *msgToSend);
#define RPL_CONNECTED "100"