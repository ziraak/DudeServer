#include "channelMessage.h"

void _fillChannelMessage(sqlite3_stmt *stmt, messageInfo *messageInfoStruct)
{
    timeStart;
    int columnCount = sqlite3_column_count(stmt);
    bzero(messageInfoStruct, sizeof(messageInfo));
    int i;
    for(i = 0; i < columnCount; i++)
    {
        if(strcmp(sqlite3_column_name(stmt, i), "user_name") == 0) { messageInfoStruct->writer = sqlite3_column_string(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "timestamp") == 0) { messageInfoStruct->timestamp = sqlite3_column_string(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "body") == 0) { messageInfoStruct->body = sqlite3_column_string(stmt, i); continue; }
    }
    timeEnd("fillChannelMessage");
}

messageInfo *_getChannelMessages(sqlite3_stmt *stmt, int *result)
{
    timeStart;
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

    timeEnd("getChannelMessages");
    STMT_RETURN(messageInfoStruct, stmt);
}

messageInfo *getMessages(char *channelName, int *result)
{
    return getMessagesOnTime(channelName, 0, result, 0);
}

messageInfo *getMessagesOnTime(char *channelName, int timestamp, int *result, int amountOfMessages)
{
    timeStart;
    sqlite3_stmt *statement;
    messageInfo *messageInfoStruct;
    char *sql = "SELECT * FROM CHANNEL_MESSAGES WHERE message_id IN (SELECT message_id FROM CHANNEL_MESSAGES WHERE channel_name = ? AND timestamp >= ? ORDER BY timestamp DESC LIMIT ?) ORDER BY timestamp ASC;";

    if(sqlite3_prepare_v2(db, sql, -1, &statement, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(statement, 1, channelName, -1, SQLITE_STATIC) == SQLITE_OK && sqlite3_bind_int(statement, 2, timestamp) == SQLITE_OK && sqlite3_bind_int(statement, 3, amountOfMessages) == SQLITE_OK)
        {
            messageInfoStruct = _getChannelMessages(statement, result);
            timeEnd("getMessagesOnTime");
            return messageInfoStruct;
        }
    }
    timeEnd("getMessagesOnTime false");
    STMT_RETURN(BOOL_FALSE, statement);
}

int insertMessage(messageInfo message, char *channelName)
{
    timeStart;
    int timestamp = (int)time(NULL);
    char*statement = sqlite3_mprintf("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES (%Q, %Q, %i, %Q);", message.writer, channelName, timestamp, message.body);

    executeStatement(statement);
    sqlite3_free(statement);

    int result, resultMessageInfos;
    messageInfo *resultMessageInfo = getMessagesOnTime(channelName, timestamp, &resultMessageInfos, 0);
    result = resultMessageInfo != NULL ? BOOL_TRUE : BOOL_FALSE;
    messageInfos_free(resultMessageInfo, resultMessageInfos);
    timeEnd("insterMessage");
    return result;


}