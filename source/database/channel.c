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

void _fillChannelMessage(sqlite3_stmt *stmt, messageInfo *messageInfoStruct)
{
    int columnCount = sqlite3_column_count(stmt);
    bzero(messageInfoStruct, sizeof(messageInfo));
    int i;
    for(i = 0; i < columnCount; i++)
    {
        if(strcmp(sqlite3_column_name(stmt, i), "user_name") == 0) { messageInfoStruct->writer = sqlite3_column_string(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "timestamp") == 0) { messageInfoStruct->timestamp = sqlite3_column_string(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "body") == 0) { messageInfoStruct->body = sqlite3_column_string(stmt, i); continue; }
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

messageInfo *_getChannelMessages(sqlite3_stmt *stmt, int *result)
{
    messageInfo *messageInfoStruct = NULL;
    int i = 0;
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        messageInfoStruct = realloc(messageInfoStruct, (i + 1) * sizeof(messageInfo));
        messageInfo ms;
        _fillChannelMessage(stmt, &ms);
        messageInfoStruct[i] = ms;
        i++;
    }
    *result = i;

    STMT_RETURN(messageInfoStruct, stmt);
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

char **getVisibleChannels()
{
    char **key;
    return key;
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

int deleteChannelInDB(char *channelName)
{
    char *statementChannelMessages = malloc(255);
    sprintf(statementChannelMessages, "DELETE FROM CHANNEL_MESSAGES WHERE channel_name = '%s';", channelName);
    executeStatement(statementChannelMessages);
    free(statementChannelMessages);

    char *statementChannelUsers = malloc(255);
    sprintf(statementChannelUsers, "DELETE FROM CHANNEL_USERS WHERE channel_name = '%s';", channelName);
    executeStatement(statementChannelUsers);
    free(statementChannelUsers);

    char *statement = malloc(255);
    sprintf(statement, "DELETE FROM CHANNELS WHERE name = '%s';", channelName);
    executeStatement(statement);
    free(statement);
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

messageInfo *getMessages(char *channelName, int *result)
{
    return getMessagesOnTime(channelName, 0, result);
}

messageInfo *getMessagesOnTime(char *channelName, int timestamp, int *result)
{
    sqlite3_stmt *statement;
    messageInfo *messageInfoStruct;
    char *sql = getSelectSQL("CHANNEL_MESSAGES", ALL_COLUMNS, "channel_name=? AND timestamp=?");

    if(sqlite3_prepare_v2(db, sql, -1, &statement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(statement, 1, channelName, -1, SQLITE_STATIC) == SQLITE_OK && sqlite3_bind_int(statement, 2, timestamp) == SQLITE_OK)
        {
            free(sql);
            messageInfoStruct = _getChannelMessages(statement, result);
            return messageInfoStruct;
        }
    }

    free(sql);
    STMT_RETURN(BOOL_FALSE, statement);
}

void createNewChannel(char *channelName, char *password, char *topic, int visible)
{

}

void addChannelToList(char *channelName, int visible)
{

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

void newChannelPassword(char *channelName, char *newPass)
{
    if(checkChannel(channelName) == BOOL_TRUE)
    {

    }
}

void newChannelTopic(char *channelName, char *newTopic)
{
}

void setChannelVisibility(char* channelname,int visible)
{
}

void setChannelUserRole(char* channelname, char* username, char* newRole)
{
}
