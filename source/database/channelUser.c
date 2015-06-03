//
// Created by osboxes on 02/06/15.
//

#include "channelUser.h"

void _fillChannelUser(sqlite3_stmt *stmt, channelUser *cu)
{
    int columnCount = sqlite3_column_count(stmt);
    bzero(cu, sizeof(channelUser));

    int i;
    for(i = 0; i < columnCount; i++)
    {
        if(strcmp(sqlite3_column_name(stmt, i), "user_name") == 0) { cu->username = sqlite3_column_string(stmt, i); }
        else if(strcmp(sqlite3_column_name(stmt, i), "channel_name") == 0) { cu->channelName = sqlite3_column_string(stmt, i); }
        else if(strcmp(sqlite3_column_name(stmt, i), "user_privileges") == 0) { cu->role = sqlite3_column_string(stmt, i); }
    }
}

int _innerGetChannelUser(sqlite3_stmt *stmt, channelUser *cu)
{
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        _fillChannelUser(stmt, cu);

        STMT_RETURN(BOOL_TRUE, stmt);
    }

    STMT_RETURN(BOOL_FALSE, stmt);
}

channelInfo* getUserChannels(char *username, int *result)
{
    sqlite3_stmt *stmt;
    char *sql = "SELECT c.* FROM CHANNELS c INNER JOIN CHANNEL_USERS cu ON c.name = cu.channel_name WHERE cu.user_name = ?;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            return _innerGetChannels(stmt, result);
        }
    }

    *result = 0;
    STMT_RETURN(NULL, stmt);
}

userInfo* getChannelUsers(char *channelName, int *result)
{
    sqlite3_stmt *stmt;
    char *sql = "SELECT u.*, cu.user_privileges FROM USERS u INNER JOIN CHANNEL_USERS cu ON u.name = cu.user_name WHERE cu.channel_name = ?;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(stmt, 1, channelName, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            return _innerGetUsers(stmt, result);
        }
    }

    *result = 0;
    STMT_RETURN(NULL, stmt);
}

int getChannelUser(char *channelName, char *username, channelUser *cu)
{
    sqlite3_stmt *stmt;
    char *sql = getSelectSQL("CHANNEL_USERS", ALL_COLUMNS, "channel_name = ? AND user_name = ?");

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(stmt, 1, channelName, -1, SQLITE_STATIC) == SQLITE_OK && sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            free(sql);
            return _innerGetChannelUser(stmt, cu);
        }
    }

    free(sql);
    STMT_RETURN(BOOL_FALSE, stmt);
}

int isUserInChannel(char *channelName, char *username)
{
    channelUser cu;
    if(getChannelUser(channelName, username, &cu) == BOOL_TRUE)
    {
        channelUser_free(&cu);
        return BOOL_TRUE;
    }

    return BOOL_FALSE;
}

int updateChannelUserRole(char *channelName, char *username, char *newRole)
{
    if(isUserInChannel(channelName, username) == BOOL_TRUE)
    {
        char *sql = sqlite3_mprintf("UPDATE CHANNEL_USERS SET user_privileges = '%s' WHERE channel_name = '%s' AND user_name = '%s';", newRole, channelName, username);
        executeStatement(sql);
        sqlite3_free(sql);

        char* role = getChannelUserRole(channelName, username);
        int result = (strcmp(role, newRole) == 0) ? BOOL_TRUE : BOOL_FALSE;
        free(role);
        return result;
    }

    return BOOL_FALSE;
}

char *getChannelUserRole(char *channelName, char *username)
{
    char* result = NULL;
    channelUser cu;
    if(getChannelUser(channelName, username, &cu) == BOOL_TRUE)
    {
        size_t length = strlen(cu.role) + 1;
        result = calloc(1, length);
        result = strncpy(result, cu.role, length);
        channelUser_free(&cu);
    }

    return result;
}

int userIsOperatorInChannel(char *channelName, char *username)
{
    char *resultUserRole = getChannelUserRole(channelName, username);
    int resultUserIsOperator = strcmp(resultUserRole, "o") == 0 ? BOOL_TRUE : BOOL_FALSE;
    FREE(resultUserRole);
    return resultUserIsOperator;
}

int checkIfChannelEmpty(char *channelName)
{
    sqlite3_stmt *stmt;
    char *sql = "SELECT COUNT(*) as usercount FROM CHANNEL_USERS WHERE channel_name = ?;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(stmt, 1, channelName, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            if(sqlite3_step(stmt) == SQLITE_ROW)
            {
                int result = (sqlite3_column_int(stmt, 0) > 0) ? BOOL_FALSE : BOOL_TRUE;
                STMT_RETURN(result, stmt);
            }

            STMT_RETURN(BOOL_TRUE, stmt);
        }
    }

    STMT_RETURN(BOOL_TRUE, stmt);
}
