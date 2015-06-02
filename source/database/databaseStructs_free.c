#include "databaseStructs_free.h"

void userInfo_free(userInfo *ui)
{
    if(ui == NULL)
    {
        return;
    }

    if(ui->username != NULL)
    {
        free(ui->username);
    }

    if(ui->password != NULL)
    {
        free(ui->password);
    }

    if(ui->channels != NULL)
    {
        free(ui->channels);
    }

    if(ui->nickname != NULL)
    {
        free(ui->nickname);
    }
}

void channelUser_free(channelUser *cu)
{
    if(cu == NULL)
    {
        return;
    }

    if(cu->username != NULL)
    {
        free(cu->username);
    }

    if(cu->role != NULL)
    {
        free(cu->role);
    }

    if(cu->nickname != NULL)
    {
        free(cu->nickname);
    }
}

void messageInfo_free(messageInfo *mi)
{
    if(mi == NULL)
    {
        return;
    }

    if(mi->writer != NULL)
    {
        free(mi->writer);
    }

    if(mi->body != NULL)
    {
        free(mi->body);
    }

    if(mi->timestamp != NULL)
    {
        free(mi->timestamp);
    }
}

void channelInfo_free(channelInfo *ci)
{
    if(ci == NULL)
    {
        return;
    }

    if(ci->messages != NULL)
    {
        int i = 0;
        for(i; i < DB_MAXMESSAGES; i++)
        {
            if(ci->messages[i].body == NULL || ci->messages[i].writer == NULL)
            {
                break;
            }

            free(ci->messages[i].body);
            free(ci->messages[i].timestamp);
            free(ci->messages[i].writer);
        }
        free(ci->messages);
    }

    free(ci->name);
    free(ci->users);
    free(ci->password);
    free(ci->topic);
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
