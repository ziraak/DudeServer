#include "channelMessage.h"

void _fillChannelMessage(sqlite3_stmt *stmt, messageInfo *messageInfoStruct)
{
    int columnCount = sqlite3_column_count(stmt);
    bzero(messageInfoStruct, sizeof(messageInfo));
    int i;
    for(i = 0; i < columnCount; i++)
    {
        if(strcmp(sqlite3_column_name(stmt, i), "user_name") == 0) { messageInfoStruct->user = sqlite3_column_string(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "timestamp") == 0) { messageInfoStruct->timestamp = sqlite3_column_int64(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "message") == 0) { messageInfoStruct->message = sqlite3_column_string(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "channel_name") == 0) { messageInfoStruct->channel = sqlite3_column_string(stmt, i); continue; }
    }
}

messageInfo *_getChannelMessages(sqlite3_stmt *stmt, int *result)
{
    messageInfo *messageInfoStruct = NULL;
    int i = 0;
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        REALLOC(messageInfoStruct, (i + 1) * sizeof(messageInfo));
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
    return getMessagesOnTime(channelName, 0, result, 0);
}

messageInfo *getMessagesOnTime(char *channelName, time_t timestamp, int *result, int amountOfMessages)
{
    sqlite3_stmt *statement;
    messageInfo *messageInfoStruct;
    char *sql = "SELECT * FROM CHANNEL_MESSAGES WHERE message_id IN (SELECT message_id FROM CHANNEL_MESSAGES WHERE channel_name = ? AND timestamp >= ? ORDER BY timestamp DESC LIMIT ?) ORDER BY timestamp ASC;";

    if(sqlite3_prepare_v2(db, sql, -1, &statement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(statement, 1, channelName, -1, SQLITE_STATIC) == SQLITE_OK && sqlite3_bind_int64(statement, 2, timestamp) == SQLITE_OK && sqlite3_bind_int64(statement, 3, amountOfMessages) == SQLITE_OK)
        {
            messageInfoStruct = _getChannelMessages(statement, result);
            return messageInfoStruct;
        }
    }

    STMT_RETURN(BOOL_FALSE, statement);
}

int insertMessage(messageInfo message)
{
    char *statement = sqlite3_mprintf("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, message) VALUES (%Q, %Q, %lu, %Q);", message.user, message.channel, message.timestamp, message.message);
    executeStatement(statement);
    sqlite3_free(statement);

    int result, resultMessageInfos;
    messageInfo *resultMessageInfo = getMessagesOnTime(message.channel, message.timestamp, &resultMessageInfos, 0);
    result = resultMessageInfo != NULL ? BOOL_TRUE : BOOL_FALSE;
    messageInfos_free(resultMessageInfo, resultMessageInfos);
    return result;
}