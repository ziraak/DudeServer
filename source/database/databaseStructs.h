#ifndef DUDESERVER_DATABASESTRUCTS_H
#define DUDESERVER_DATABASESTRUCTS_H

typedef struct userInfo
{
    char *username;
    char *nickname;
    char *password;
    char **channels;
    char *loginToken;
} userInfo;


typedef struct channelUser
{
    char* username;
    char* role;
    char* nickname;
}channelUser;

typedef struct messageInfo
{
    char *writer;
    char *timestamp;
    char *body;
} messageInfo;

typedef struct channelInfo
{
    char *name;
    char *topic;
    char *password;
    char** users;
    int visible;
    messageInfo *messages;
} channelInfo;


#endif //DUDESERVER_DATABASESTRUCTS_H
