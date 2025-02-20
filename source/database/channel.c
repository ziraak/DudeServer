#include "channel.h"

void _fillChannel(sqlite3_stmt *stmt, channelInfo *channel)
{
    int columnCount = sqlite3_column_count(stmt);
    bzero(channel, sizeof(channelInfo));

    int i;
    for(i = 0; i < columnCount; i++)
    {
        if(strcmp(sqlite3_column_name(stmt, i), "name") == 0) { channel->name = sqlite3_column_string(stmt, i); }
        else if(strcmp(sqlite3_column_name(stmt, i), "password") == 0) { channel->password = sqlite3_column_string(stmt, i); }
        else if(strcmp(sqlite3_column_name(stmt, i), "topic") == 0) { channel->topic = sqlite3_column_string(stmt, i); }

            // flags
        else if(strcmp(sqlite3_column_name(stmt, i), "visible") == 0) { channel->visible = sqlite3_column_int(stmt, i); }
        else if(strcmp(sqlite3_column_name(stmt, i), "inviteOnly") == 0) { channel->inviteOnly = sqlite3_column_int(stmt, i); }
        else if(strcmp(sqlite3_column_name(stmt, i), "topicOperatorOnly") == 0) { channel->topicOperatorOnly = sqlite3_column_int(stmt, i); }
    }
}

int _innerGetChannel(sqlite3_stmt *stmt, channelInfo *channel)
{
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        _fillChannel(stmt, channel);

        STMT_RETURN(BOOL_TRUE, stmt);
    }

    STMT_RETURN(BOOL_FALSE, stmt);
}

channelInfo *_innerGetChannels(sqlite3_stmt *stmt, int *result)
{
    channelInfo *channels = NULL;
    int i = 0;
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        REALLOC(channels, sizeof(channelInfo) * (i + 1));
        channelInfo cs;
        _fillChannel(stmt, &cs);
        channels[i] = cs;
        i++;
    }

    *result = i;
    STMT_RETURN(channels, stmt);
}

channelInfo *getChannels(char* columns, int *result)
{
    sqlite3_stmt *stmt;
    char *sql = getSelectSQL("channels", columns, NULL);

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        FREE(sql);
        return _innerGetChannels(stmt, result);
    }

    FREE(sql);
    *result = BOOL_FALSE;
    STMT_RETURN(NULL, stmt);
}

int getChannelByName(char *name, channelInfo *channel)
{
    sqlite3_stmt *stmt;
    char *sql = getSelectSQL("channels", ALL_COLUMNS, "name=?");

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            FREE(sql);
            return _innerGetChannel(stmt, channel);
        }
    }

    FREE(sql);
    STMT_RETURN(BOOL_FALSE, stmt);
}

channelInfo* getVisibleChannels(char* columns, int *result)
{
    sqlite3_stmt *stmt;
    char *sql = getSelectSQL("channels", columns, "visible=1");

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        FREE(sql);
        return _innerGetChannels(stmt, result);
    }

    FREE(sql);
    *result = BOOL_FALSE;
    STMT_RETURN(NULL, stmt);
}

int checkIfChannelVisible(char* channelName)
{
    channelInfo ci;
    if(getChannelByName(channelName, &ci) == BOOL_TRUE)
    {
        int result = ci.visible;
        channelInfo_free(&ci);
        return (result == BOOL_TRUE) ? BOOL_TRUE : BOOL_FALSE;
    }

    return BOOL_FALSE;
}

int checkChannel(char *channelName)
{
    channelInfo channelInfoStruct;
    if (getChannelByName(channelName, &channelInfoStruct) == BOOL_TRUE)
    {
        channelInfo_free(&channelInfoStruct);
        return BOOL_TRUE;
    }
    return BOOL_FALSE;
}

int deleteChannel(char *channelName)
{
    char *channelMessagesDelete = sqlite3_mprintf("DELETE FROM CHANNEL_MESSAGES WHERE channel_name = %Q;", channelName);
    executeStatement(channelMessagesDelete);
    sqlite3_free(channelMessagesDelete);

    char *channelUsersDelete = sqlite3_mprintf("DELETE FROM CHANNEL_USERS WHERE channel_name = %Q;", channelName);
    executeStatement(channelUsersDelete);
    sqlite3_free(channelUsersDelete);

    char *channelDelete = sqlite3_mprintf("DELETE FROM CHANNELS WHERE name = %Q;", channelName);
    executeStatement(channelDelete);
    sqlite3_free(channelDelete);

    return checkChannel(channelName) == BOOL_FALSE;
}

int insertChannel(char *channelName, char *password, char *topic, int visible, int inviteOnly, int topicOperatorOnly)
{
    if(checkChannel(channelName) == BOOL_TRUE)
    {
        return BOOL_FALSE;
    }

    char* stmt = sqlite3_mprintf("INSERT INTO CHANNELS (name, password, topic, visible, inviteOnly, topicOperatorOnly) VALUES (%Q, %Q, %Q, %i, %i, %i);", channelName, password, topic, visible, inviteOnly, topicOperatorOnly);
    executeStatement(stmt);
    sqlite3_free(stmt);

    return checkChannel(channelName);
}

int checkIfChannelHasPassword(char *channelName)
{
    channelInfo ci;
    if(getChannelByName(channelName, &ci) == BOOL_TRUE)
    {
        int result = ci.password != NULL ? BOOL_TRUE : BOOL_FALSE;
        channelInfo_free(&ci);
        return result;
    }

    return BOOL_FALSE;
}

int checkIfChannelIsInviteOnly(char *channelName)
{
    channelInfo ci;
    if(getChannelByName(channelName, &ci) == BOOL_TRUE)
    {
        int result = ci.inviteOnly == BOOL_TRUE ? BOOL_TRUE : BOOL_FALSE;
        channelInfo_free(&ci);
        return result;
    }

    return BOOL_FALSE;
}

int checkIfChannelTopicOperatorOnly(char *channelName)
{
    channelInfo ci;
    if(getChannelByName(channelName, &ci) == BOOL_TRUE)
    {
        int result = ci.topicOperatorOnly == BOOL_TRUE ? BOOL_TRUE : BOOL_FALSE;
        channelInfo_free(&ci);
        return result;
    }

    return BOOL_FALSE;
}

int authenticateChannelPassword(char *channelName, char *password)
{
    channelInfo ci;
    if(getChannelByName(channelName, &ci) == BOOL_TRUE)
    {
        int result = (strcmp(password, ci.password) == 0);
        channelInfo_free(&ci);
        return result;
    }

    return BOOL_FALSE;
}

void updateChannelPassword(char *channelName, char *newPass)
{
    char* stmt = sqlite3_mprintf("UPDATE CHANNELS SET password = %Q WHERE name = %Q;", newPass, channelName);
    executeStatement(stmt);
    sqlite3_free(stmt);
}

void updateChannelTopic(char *channelName, char *newTopic)
{
    char* stmt = sqlite3_mprintf("UPDATE CHANNELS SET topic = %Q WHERE name = %Q;", newTopic, channelName);
    executeStatement(stmt);
    sqlite3_free(stmt);
}

void updateChannelVisibility(char *channelName, int visible)
{
    char* stmt = sqlite3_mprintf("UPDATE CHANNELS SET visible = %i WHERE name = %Q;", visible, channelName);
    executeStatement(stmt);
    sqlite3_free(stmt);
}

void updateChannelInviteOnly(char *channelName, int inviteOnly)
{
    char* stmt = sqlite3_mprintf("UPDATE CHANNELS SET inviteOnly = %i WHERE name = %Q;", inviteOnly, channelName);
    executeStatement(stmt);
    sqlite3_free(stmt);
}

void updateChannelTopicOperatorOnly(char *channelName, int topicOperatorOnly)
{
    char* stmt = sqlite3_mprintf("UPDATE CHANNELS SET topicOperatorOnly = %i WHERE name = %Q;", topicOperatorOnly, channelName);
    executeStatement(stmt);
    sqlite3_free(stmt);
}
