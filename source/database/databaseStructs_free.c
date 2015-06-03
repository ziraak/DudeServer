#include "databaseStructs_free.h"

void userInfo_free(userInfo *ui)
{
    if(ui != NULL)
    {
        free(ui->username);
        free(ui->password);
        free(ui->nickname);
        free(ui->role);

        ui->username = NULL;
        ui->password = NULL;
        ui->nickname = NULL;
        ui->role = NULL;
    }
}

void channelUser_free(channelUser *cu)
{
    if(cu == NULL)
    {
        return;
    }

    free(cu->username);
    free(cu->channelName);
    free(cu->role);

    cu->username = NULL;
    cu->channelName = NULL;
    cu->role = NULL;
}

void messageInfo_free(messageInfo *mi)
{
    if(mi != NULL)
    {
        free(mi->writer);
        free(mi->body);
        free(mi->timestamp);

        mi->writer = NULL;
        mi->body = NULL;
        mi->timestamp = NULL;
    }
}

void channelInfo_free(channelInfo *ci)
{
    if(ci == NULL)
    {
        return;
    }

    free(ci->name);
    free(ci->password);
    free(ci->topic);

    ci->name = NULL;
    ci->password = NULL;
    ci->topic = NULL;
}

void channelInfos_free(channelInfo *channels, int amount)
{
    if(channels == NULL)
    {
        return;
    }

    int i;
    for(i = 0; i < amount; i++)
    {
        channelInfo_free(&channels[i]);
    }

    free(channels);
}

void messageInfos_free(messageInfo *messageInfoStruct, int amount)
{
    if (messageInfoStruct == NULL)
    {
        return;
    }

    int i;
    for (i = 0; i < amount; i++)
    {
        messageInfo_free(&messageInfoStruct[i]);
    }
    free(messageInfoStruct);
}

void userInfos_free(userInfo *userInfoStruct, int amount)
{
    if (userInfoStruct == NULL)
    {
        return;
    }

    int i;
    for (i = 0; i < amount; i++)
    {
        userInfo_free(&userInfoStruct[i]);
    }
    free(userInfoStruct);
}
