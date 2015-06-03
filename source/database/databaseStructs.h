#ifndef DUDESERVER_DATABASESTRUCTS_H
#define DUDESERVER_DATABASESTRUCTS_H

typedef struct userInfo
{
    char *username;
    char *nickname;
    char *password;
    char *role;
    char **channels;
} userInfo;


typedef struct channelUser
{
    char* username;
    char* channelName;
    char* role;
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
} channelInfo;


#endif //DUDESERVER_DATABASESTRUCTS_H
