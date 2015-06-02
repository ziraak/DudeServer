#include "channelMessage.h"

int countMessages(char *channelName)
{

}

int insertMessage(messageInfo message, char *channelName)
{
    char*statement = sqlite3_mprintf("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('%s', '%s', %i, '%s');", message.writer, channelName, message.timestamp, message.body);
    executeStatement(statement);
    sqlite3_free(statement);

    int result;
    messageInfo *resultMessageInfo = getMessagesOnTime(channelName, atoi(message.timestamp), &result);
    result = resultMessageInfo->writer != NULL ? BOOL_TRUE : BOOL_FALSE;
    messageInfo_free(resultMessageInfo);
    return result;
}

void deleteMessage(char *channelName)
{

}