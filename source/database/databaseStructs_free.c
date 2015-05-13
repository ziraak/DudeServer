//
// Created by osboxes on 13/05/15.
//

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
