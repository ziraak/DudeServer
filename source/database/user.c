#include "user.h"


void fillUser(sqlite3_stmt *statement, userInfo *user)
{
    int columnCount = sqlite3_column_count(statement);
    bzero(user, sizeof(userInfo));

    int i;
    for(i = 0; i < columnCount; i++)
    {
        if(strcmp(sqlite3_column_name(statement, i), "name") == 0) { user->username = sqlite3_column_string(statement, i); continue; }
        if(strcmp(sqlite3_column_name(statement, i), "password") == 0) { user->password = sqlite3_column_string(statement, i); continue; }
        if(strcmp(sqlite3_column_name(statement, i), "nickname") == 0) { user->nickname = sqlite3_column_string(statement, i); continue; }
    }
}

int getUserinfoWithSql(sqlite3_stmt *statement, userInfo *user)
{
    while(sqlite3_step(statement) == SQLITE_ROW)
    {
        fillUser(statement, user);

        STMT_RETURN(BOOL_TRUE, statement);
    }

    STMT_RETURN(BOOL_FALSE, statement);
}

int getUserByName(char *name, userInfo *user)
{
    sqlite3_stmt *statement;
    char *sql = getSelectSQL("users", ALL_COLUMNS, "name=?");
    if(sqlite3_prepare_v2(db, sql, -1, &statement, NULL) == SQLITE_OK)
    {
        free(sql);
        if(sqlite3_bind_text(statement, 1, name, -1, SQLITE_STATIC) == SQLITE_OK)
        {
            return getUserinfoWithSql(statement, user);
        }
    }
    free(sql);
    STMT_RETURN(BOOL_FALSE, statement);
}

void user_free(userInfo *user)
{
    if(user != NULL)
    {
        free(user->username);
        free(user->nickname);
        free(user->password);
    }
}

void users_free(userInfo *users, int amount)
{
    if(users != NULL)
    {
        int i;
        for(i = 0; i < amount; i++)
        {
            user_free(&users[i]);
        }

        free(users);
    }
}

int getUser(char *username, userInfo *result)
{
    getUserByName(username,result);
    return DB_RETURN_SUCCES;
}

int isUserInChannel(char* channelname, char* username)
{
    return BOOL_FALSE;
}


char* getUserNickname(char* username)
{
    char* result = "";
    userInfo info;
    getUserByName(username,&info);
    strcpy(result,info.nickname);
    user_free(&info);
    return result;
}
int checkIfUserExists(char *username)
{
    userInfo info;

    sqlite3_stmt *statement;
    char *sql = getSelectSQL("users", ALL_COLUMNS, "name=?");
    if(sqlite3_prepare_v2(db, sql, -1, &statement, NULL) == SQLITE_OK)
    {
        free(sql);
        if(sqlite3_bind_text(statement, 1, username, -1, SQLITE_STATIC) == SQLITE_OK)
        {

        }

    }
    free(sql);

    if(info.username !=NULL)
    {
        user_free(&info);
        return BOOL_TRUE;
    }
//    user_free(&info);
    return BOOL_FALSE;
}

int userJoinChannel(char *username, char *channelName, char *userRole)
{
    return DB_RETURN_SUCCES;
}

int userLeaveChannel(char* username, char *channelname)
{
    return DB_RETURN_SUCCES;
}


int deleteUser(char *username)
{
    return DB_RETURN_SUCCES;
}

int changeNickname(char *username, char *newNickname)
{
}

int changePassword(char *username, char *newPassword)
{
}

int createNewUser(char *username, char *password)
{
    return DB_RETURN_SUCCES;
}
