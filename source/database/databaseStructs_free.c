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

    if(ci->name != NULL)
    {
        free(ci->name);
    }

    if(ci->users != NULL)
    {
        free(ci->users);
    }
}
