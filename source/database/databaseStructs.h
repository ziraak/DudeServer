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

typedef struct messageInfo
{
    char *writer;
    char *timestamp;
    char *body;
} messageInfo;

typedef struct channelInfo
{
    char *name;
    char **users;
    messageInfo *messages;
} channelInfo;

#endif //DUDESERVER_DATABASESTRUCTS_H
