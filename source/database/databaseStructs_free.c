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

    if(ci->messages != NULL)
    {
        int i = 0;
        for(i; i < DB_MAXMESSAGES; i++)
        {
            if(ci->messages[i].body == NULL || ci->messages[i].writer == NULL)
            {
                break;
            }

            messageInfo_free(&ci->messages[i]);
        }
        free(ci->messages);

        ci->messages = NULL;
    }

    free(ci->name);
    free(ci->users);
    free(ci->password);
    free(ci->topic);

    ci->name = NULL;
    ci->users = NULL;
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
