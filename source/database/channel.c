#include "channel.h"

void _fillChannel(sqlite3_stmt *stmt, channelInfo *channel)
{
    int columnCount = sqlite3_column_count(stmt);
    bzero(channel, sizeof(channelInfo));

    int i;
    for(i = 0; i < columnCount; i++)
    {
        if(strcmp(sqlite3_column_name(stmt, i), "name") == 0) { channel->name = sqlite3_column_string(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "password") == 0) { channel->password = sqlite3_column_string(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "topic") == 0) { channel->topic = sqlite3_column_string(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "visible") == 0) { channel->visible = sqlite3_column_int(stmt, i); continue; }
    }
}

int _getChannel(sqlite3_stmt *stmt, channelInfo *channel)
{
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        _fillChannel(stmt, channel);

        STMT_RETURN(BOOL_TRUE, stmt);
    }

    STMT_RETURN(BOOL_FALSE, stmt);
}

channelInfo *_getChannels(sqlite3_stmt *stmt, int *result)
{
    channelInfo *channels = NULL;
    int i = 0;
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        channels = realloc(channels, (i + 1) * sizeof(channelInfo));
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
        free(sql);
        return _getChannels(stmt, result);
    }

    free(sql);
    *result = BOOL_FALSE;
    STMT_RETURN(NULL, stmt);
}

int getChannelByName(char *name, channelInfo *channel)
{
    sqlite3_stmt *stmt;
    char *sql = getSelectSQL("channels", ALL_COLUMNS, "name=?");

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        free(sql);
        if(sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            return _getChannel(stmt, channel);
        }
    }

    free(sql);
    STMT_RETURN(BOOL_FALSE, stmt);
}

int countMessages(messageInfo *message)
{
}

void writeMessage(messageInfo message, char* channelName)
{
}
void deleteMessage(char* channelname)
{
}

int writeMessageToChannel(char *channelName, messageInfo message)
{
    return DB_RETURN_SUCCES;
}

channelUser *getUsersFromChannel(char *channelName)
{
    channelUser* users = NULL;
    return users;
}

char* getUserRole(char* channelName, char* username)
{
    return NULL;
}

channelInfo* getVisibleChannels(char* columns, int *result)
{
    sqlite3_stmt *stmt;
    char *sql = getSelectSQL("channels", columns, "visible=1");

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        free(sql);
        return _getChannels(stmt, result);
    }

    free(sql);
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
    char *channelMessagesDelete = sqlite3_mprintf("DELETE FROM CHANNEL_MESSAGES WHERE channel_name = '%s';", channelName);
    executeStatement(channelMessagesDelete);
    sqlite3_free(channelMessagesDelete);

    char *channelUsersDelete = sqlite3_mprintf("DELETE FROM CHANNEL_USERS WHERE channel_name = '%s';", channelName);
    executeStatement(channelUsersDelete);
    sqlite3_free(channelUsersDelete);

    char *channelDelete = sqlite3_mprintf("DELETE FROM CHANNELS WHERE name = '%s';", channelName);
    executeStatement(channelDelete);
    sqlite3_free(channelDelete);

    return checkChannel(channelName) == BOOL_FALSE;
}

int deleteChannelFromList(char *channelName)
{

}

int deleteUserFromChannel(char *channelName, char *username)
{
}

int checkIfChannelEmpty(char *channelName)
{
}

messageInfo *getMessages(char *channelName)
{
    return getMessagesOnTime(channelName, 0);
}

messageInfo *getMessagesOnTime(char *channelName, int timestamp)
{

}

void insertChannel(char *channelName, char *password, char *topic, int visible)
{
    char* stmt = sqlite3_mprintf("INSERT INTO CHANNELS (name, password, topic, visible) VALUES ('%s', '%s', '%s', '%i');", channelName, password, topic, visible);
    executeStatement(stmt);
    sqlite3_free(stmt);
}

int checkIfChannelHasPassword(char *channelname)
{
    channelInfo ci;
    if(getChannelByName(channelname, &ci) == BOOL_TRUE)
    {
        int result = ci.password != NULL ? BOOL_TRUE : BOOL_FALSE;
        channelInfo_free(&ci);
        return result;
    }

    return BOOL_FALSE;
}

int authenticateChannelPassword(char *channelname, char *password)
{
    channelInfo ci;
    if(getChannelByName(channelname, &ci) == BOOL_TRUE)
    {
        int result = (strcmp(password, ci.password) == 0);
        channelInfo_free(&ci);
        return result;
    }

    return BOOL_FALSE;
}

void updateChannelPassword(char *channelName, char *newPass)
{
    char* stmt = sqlite3_mprintf("UPDATE CHANNELS SET password = '%s' WHERE name = '%s';", newPass, channelName);
    executeStatement(stmt);
    sqlite3_free(stmt);
}

void updateChannelTopic(char *channelName, char *newTopic)
{
    char* stmt = sqlite3_mprintf("UPDATE CHANNELS SET topic = '%s' WHERE name = '%s';", newTopic, channelName);
    executeStatement(stmt);
    sqlite3_free(stmt);
}

void updateChannelVisibility(char *channelName, int visible)
{
    char* stmt = sqlite3_mprintf("UPDATE CHANNELS SET visible = %i WHERE name = '%s';", visible, channelName);
    executeStatement(stmt);
    sqlite3_free(stmt);
}

void updateChannelUserRole(char *channelName, char *username, char *newRole)
{
    if(checkChannel(channelName) == BOOL_TRUE)
    {

    }
}
