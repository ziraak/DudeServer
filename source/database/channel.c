#include "channel.h"

void fillChannel(sqlite3_stmt *stmt, channelInfo *channel)
{
    int columnCount = sqlite3_column_count(stmt);
    bzero(channel, sizeof(channelInfo));

    int i;
    for(i = 0; i < columnCount; i++)
    {
        if(strcmp(sqlite3_column_name(stmt, i), "name") == 0) { channel->name = sqlite3_column_string(stmt, i); continue; }
        if(strcmp(sqlite3_column_name(stmt, i), "password") == 0) { channel->password = sqlite3_column_string(stmt, i); continue; }
//        if(strcmp(sqlite3_column_name(stmt, i), "visible") == 0) { channel-> = sqlite3_column_int(stmt, i); continue; }
    }
}

int getChannel(sqlite3_stmt *stmt, channelInfo *channel)
{
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        fillChannel(stmt, channel);

        STMT_RETURN(DB_RETURN_SUCCES, stmt);
    }

    STMT_RETURN(BOOL_FALSE, stmt);
}

channelInfo * getChannels(char* columns, int *result)
{
    sqlite3_stmt *stmt;
    char *sql = getSelectSQL("channels", columns, NULL);
    channelInfo *channels = NULL;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        free(sql);

        int i = 0;
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            channels = realloc(channels, (i + 1) * sizeof(channelInfo));
            channelInfo cs;
            fillChannel(stmt, &cs);
            channels[i] = cs;
            i++;
        }

        *result = i;
        STMT_RETURN(channels, stmt);
    }

    free(sql);
    *result = BOOL_FALSE;
    STMT_RETURN(channels, stmt);
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
            return getChannel(stmt, channel);
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

char **getChannellist()
{
    char **list;
    return list;
}

char **getVisibleChannels()
{
    char **key;
    return key;
}

int checkIfChannelVisible(char* channelName)
{
    return BOOL_FALSE;
}

int checkChannel(char *channelName)
{
}

int deleteChannelInDB(char *channelName)
{
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

void createNewChannel(char *channelName, char *password, char *topic, int visible)
{
}

void addChannelToList(char *channelName, int visible)
{
}

int checkIfChannelHasPassword(char *channelname)
{
    return BOOL_TRUE;
}

int authenticateChannelPassword(char *channelname, char *password)
{
    return BOOL_FALSE;
}

void newChannelPassword(char *channelName, char *newPass)
{
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
