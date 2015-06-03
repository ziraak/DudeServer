//
// Created by osboxes on 02/06/15.
//

#include "channelUser.h"

void _fillChannelUser(sqlite3_stmt *stmt, channelUser *cu)
{
    timeStart;
    int columnCount = sqlite3_column_count(stmt);
    bzero(cu, sizeof(channelUser));

    int i;
    for(i = 0; i < columnCount; i++)
    {
        if(strcmp(sqlite3_column_name(stmt, i), "user_name") == 0) { cu->username = sqlite3_column_string(stmt, i); }
        else if(strcmp(sqlite3_column_name(stmt, i), "channel_name") == 0) { cu->channelName = sqlite3_column_string(stmt, i); }
        else if(strcmp(sqlite3_column_name(stmt, i), "user_privileges") == 0) { cu->role = sqlite3_column_string(stmt, i); }
    }
    timeEnd("fillChannelUser");
}

int _innerGetChannelUser(sqlite3_stmt *stmt, channelUser *cu)
{
    timeStart;
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        _fillChannelUser(stmt, cu);
        timeEnd("innerGetChannelUser true");
        STMT_RETURN(BOOL_TRUE, stmt);
    }
    timeEnd("innerGetChannelUser false");
    STMT_RETURN(BOOL_FALSE, stmt);
}

channelInfo* getUserChannels(char *username, int *result)
{
    timeStart;
    sqlite3_stmt *stmt;
    char *sql = "SELECT c.* FROM CHANNELS c INNER JOIN CHANNEL_USERS cu ON c.name = cu.channel_name WHERE cu.user_name = ?;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            timeEnd("getUserChannels");
            return _innerGetChannels(stmt, result);
        }
    }

    *result = 0;
    timeEnd("getUserChannels null");
    STMT_RETURN(NULL, stmt);
}

userInfo* getChannelUsers(char *channelName, int *result)
{
    timeStart;
    sqlite3_stmt *stmt;
    char *sql = "SELECT u.*, cu.user_privileges FROM USERS u INNER JOIN CHANNEL_USERS cu ON u.name = cu.user_name WHERE cu.channel_name = ?;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(stmt, 1, channelName, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            timeEnd("getChannelUsers");
            return _innerGetUsers(stmt, result);
        }
    }

    *result = 0;
    timeEnd("getChannelUsers null");
    STMT_RETURN(NULL, stmt);
}

int getChannelUser(char *channelName, char *username, channelUser *cu)
{
    timeStart;
    sqlite3_stmt *stmt;
    char *sql = getSelectSQL("CHANNEL_USERS", ALL_COLUMNS, "channel_name = ? AND user_name = ?");

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(stmt, 1, channelName, -1, SQLITE_STATIC) == SQLITE_OK && sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            free(sql);
            timeEnd("getChannelUser");
            return _innerGetChannelUser(stmt, cu);
        }
    }

    free(sql);
    timeEnd("getChannelUser NULL");
    STMT_RETURN(BOOL_FALSE, stmt);
}

int isUserInChannel(char *channelName, char *username)
{
    timeStart;
    channelUser cu;
    if(getChannelUser(channelName, username, &cu) == BOOL_TRUE)
    {
        channelUser_free(&cu);
        timeEnd("isUserInChannel true");
        return BOOL_TRUE;
    }

    timeEnd("isUserInChannel false");
    return BOOL_FALSE;
}

int updateChannelUserRole(char *channelName, char *username, char *newRole)
{
    timeStart;
    if(isUserInChannel(channelName, username) == BOOL_TRUE)
    {
        char *sql = sqlite3_mprintf("UPDATE CHANNEL_USERS SET user_privileges = '%s' WHERE channel_name = '%s' AND user_name = '%s';", newRole, channelName, username);
        executeStatement(sql);
        sqlite3_free(sql);

        char* role = getChannelUserRole(channelName, username);
        int result = (strcmp(role, newRole) == 0) ? BOOL_TRUE : BOOL_FALSE;
        free(role);
        timeEnd("updateChannelUserRole");
        return result;
    }
    timeEnd("updateChannelUserRole false");
    return BOOL_FALSE;
}

char *getChannelUserRole(char *channelName, char *username)
{
    timeStart;
    char* result = NULL;
    channelUser cu;
    if(getChannelUser(channelName, username, &cu) == BOOL_TRUE)
    {
        size_t length = strlen(cu.role) + 1;
        result = calloc(1, length);
        result = strncpy(result, cu.role, length);
        channelUser_free(&cu);
    }

    timeEnd("getChannelUserRole");
    return result;
}

int checkIfChannelEmpty(char *channelName)
{
    timeStart;
    sqlite3_stmt *stmt;
    char *sql = "SELECT COUNT(*) as usercount FROM CHANNEL_USERS WHERE channel_name = ?;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(stmt, 1, channelName, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            if(sqlite3_step(stmt) == SQLITE_ROW)
            {
                timeEnd("checkIfChannelEmpty");
                int result = (sqlite3_column_int(stmt, 0) > 0) ? BOOL_FALSE : BOOL_TRUE;
                STMT_RETURN(result, stmt);
            }
            timeEnd("checkIfChannelEmpty TRUE");
            STMT_RETURN(BOOL_TRUE, stmt);
        }
    }
    timeEnd("checkIfChannelEmpty TRUE 2");
    STMT_RETURN(BOOL_TRUE, stmt);
}
