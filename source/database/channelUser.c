//
// Created by osboxes on 02/06/15.
//

#include "channelUser.h"

channelInfo* getUserChannels(char *username, int *result)
{
    sqlite3_stmt *stmt;
    char *sql = sqlite3_mprintf("SELECT c.* FROM CHANNELS c INNER JOIN CHANNEL_USERS cu ON c.name = cu.channel_name WHERE cu.user_name = ?");

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        if(sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            sqlite3_free(sql);
            return _innerGetChannels(stmt, result);
        }
    }

    sqlite3_free(sql);
    *result = 0;
    STMT_RETURN(NULL, stmt);
}

void updateChannelUserRole(char *channelName, char *username, char *newRole)
{
    if(checkChannel(channelName) == BOOL_TRUE)
    {

    }
}

int checkIfChannelEmpty(char *channelName)
{

}
