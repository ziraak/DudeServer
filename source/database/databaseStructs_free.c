#include "databaseStructs_free.h"

void userInfo_free(userInfo *ui)
{
    if(ui != NULL)
    {
        FREE(ui->username);
        FREE(ui->password);
        FREE(ui->nickname);
        FREE(ui->role);
    }
}

void channelUser_free(channelUser *cu)
{
    if(cu != NULL)
    {
        FREE(cu->username);
        FREE(cu->channelName);
        FREE(cu->role);
    }
}

void messageInfo_free(messageInfo *mi)
{
    if(mi != NULL)
    {
        FREE(mi->channel);
        FREE(mi->user);
        FREE(mi->message);
    }
}

void channelInfo_free(channelInfo *ci)
{
    if(ci != NULL)
    {
        FREE(ci->name);
        FREE(ci->password);
        FREE(ci->topic);
    }
}

void channelInfos_free(channelInfo *channels, int amount)
{
    if(channels != NULL)
    {
        int i;
        for(i = 0; i < amount; i++)
        {
            channelInfo_free(&channels[i]);
        }

        FREE(channels);
    }
}

void messageInfos_free(messageInfo *messageInfoStruct, int amount)
{
    if (messageInfoStruct != NULL)
    {
        int i;
        for (i = 0; i < amount; i++)
        {
            messageInfo_free(&messageInfoStruct[i]);
        }
        FREE(messageInfoStruct);
    }
}

void userInfos_free(userInfo *userInfoStruct, int amount)
{
    if (userInfoStruct != NULL)
    {
        int i;
        for (i = 0; i < amount; i++)
        {
            userInfo_free(&userInfoStruct[i]);
        }
        FREE(userInfoStruct);
    }
}
