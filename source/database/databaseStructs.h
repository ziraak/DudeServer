#ifndef DUDESERVER_DATABASESTRUCTS_H
#define DUDESERVER_DATABASESTRUCTS_H

typedef struct userInfo
{
    char *username;
    char *nickname;
    char *password;
    char *role;
} userInfo;


typedef struct channelUser
{
    char* username;
    char* channelName;
    char* role;
}channelUser;

typedef struct messageInfo
{
    char *channel;
    char *user;
    long timestamp;
    char *message;
} messageInfo;

typedef struct channelInfo
{
    char *name;
    char *topic;
    char *password;

    // flags
    int visible;
    int inviteOnly;
    int topicOperatorOnly;
} channelInfo;

#endif //DUDESERVER_DATABASESTRUCTS_H
