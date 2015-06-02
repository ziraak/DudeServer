#include "channelMessage.h"

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

messageInfo *getMessages(char *channelName, int *result)
{
    return getMessagesOnTime(channelName, 0, result);
}

messageInfo *getMessagesOnTime(char *channelName, int timestamp, int *result)
{
    sqlite3_stmt *statement;
    messageInfo *messageInfoStruct;
    char *sql = getSelectSQL("CHANNEL_MESSAGES", ALL_COLUMNS, "channel_name=? AND timestamp >= ?");

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

int countMessages(char *channelName)
{

}

int insertMessage(messageInfo message, char *channelName)
{
    char*statement = sqlite3_mprintf("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('%s', '%s', %i, '%s');", message.writer, channelName, message.timestamp, message.body);
    executeStatement(statement);
    sqlite3_free(statement);

    int result, resultMessageInfos;
    messageInfo *resultMessageInfo = getMessagesOnTime(channelName, atoi(message.timestamp), &resultMessageInfos);
    result = resultMessageInfo->writer != NULL ? BOOL_TRUE : BOOL_FALSE;
    messageInfos_free(resultMessageInfo, resultMessageInfos);
    return result;
}

void deleteMessage(char *channelName)
{

}