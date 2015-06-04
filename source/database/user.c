#include "user.h"


void fillUser(sqlite3_stmt *statement, userInfo *user)
{
    int columnCount = sqlite3_column_count(statement);
    bzero(user, sizeof(userInfo));

    int i;
    for(i = 0; i < columnCount; i++)
    {
        if(strcmp(sqlite3_column_name(statement, i), "name") == 0) { user->username = sqlite3_column_string(statement, i); }
        else if(strcmp(sqlite3_column_name(statement, i), "password") == 0) { user->password = sqlite3_column_string(statement, i); }
        else if(strcmp(sqlite3_column_name(statement, i), "nickname") == 0) { user->nickname = sqlite3_column_string(statement, i); }
        else if(strcmp(sqlite3_column_name(statement, i), "user_privileges") == 0) { user->role = sqlite3_column_string(statement, i); }
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

userInfo *_innerGetUsers(sqlite3_stmt *stmt, int *result)
{
    userInfo *users = NULL;
    int i = 0;
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        REALLOC(users, (i + 1) * sizeof(userInfo));
        userInfo ui;
        fillUser(stmt, &ui);
        users[i] = ui;
        i++;
    }

    *result = i;
    STMT_RETURN(users, stmt);
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
        free(user->role);
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
    return getUserByName(username,result);
}

char* getUserNickname(char* username)
{
    userInfo info;
    getUserByName(username,&info);
    char* result = malloc(sizeof(info.nickname));
    strcpy(result,info.nickname);
    user_free(&info);
    return result;
}
int checkIfUserExists(char *username)
{
    if(username == NULL || !strcmp(username,"")) return DB_RETURN_NULLPOINTER;
    userInfo user;
    bzero(&user, sizeof(userInfo));
    if(getUser(username, &user)== BOOL_TRUE)
    {
        user_free(&user);
        return BOOL_TRUE;
    }
    user_free(&user);
    return BOOL_FALSE;
}

int userJoinChannel(char *username, char *channelName, char *userRole)
{
    if(username == NULL || channelName == NULL || userRole == NULL) return DB_RETURN_NULLPOINTER;

    char*values = malloc(strlen(username)+ strlen(channelName) + strlen(userRole)+ 10);
    sprintf(values,"'%s','%s','%s'",username,channelName,userRole);

    char* statement = getInsertSQL("CHANNEL_USERS","user_name, channel_name, user_privileges",values);
    free(values);

    executeStatement(statement);
    free(statement);
    return DB_RETURN_SUCCES;
}

int userLeaveChannel(char* username, char *channelname)
{
    if(username == NULL || channelname == NULL) return DB_RETURN_NULLPOINTER;

    char* where = malloc(strlen(username) + strlen(channelname)+ 40);
    sprintf(where,"user_name = '%s' AND channel_name = '%s'",username,channelname);

    char*statement = getDeleteSQL("CHANNEL_USERS",where);
    free(where);

    executeStatement(statement);
    free(statement);

    return DB_RETURN_SUCCES;
}


int deleteUser(char *username)
{
    if(username == NULL) return DB_RETURN_NULLPOINTER;

    char* where = malloc(strlen(username) + 20);
    sprintf(where,"user_name = '%s'",username);

    char*statement = getDeleteSQL("CHANNEL_USERS",where);
    executeStatement(statement);
    free(statement);

    sprintf(where,"name = '%s'",username);
    char* statement2 = getDeleteSQL("USERS",where);
    free(where);

    executeStatement(statement2);
    free(statement2);
    return DB_RETURN_SUCCES;
}

int changeNickname(char *username, char *newNickname)
{
    if(username == NULL || newNickname == NULL) return DB_RETURN_NULLPOINTER;
    char* where = malloc(strlen(username) + 10);
    sprintf(where,"name = '%s'",username);

    char*statement = getUpdateSQL("USERS",where,"nickname",newNickname);
    free(where);
    executeStatement(statement);
    free(statement);
    return DB_RETURN_SUCCES;
}

int changePassword(char *username, char *newPassword)
{
    if(username == NULL || newPassword == NULL) return DB_RETURN_NULLPOINTER;
    char* where = malloc(strlen(username) + 10);
    sprintf(where,"name = '%s'",username);

    char*statement = getUpdateSQL("USERS",where,"password",newPassword);
    free(where);
    executeStatement(statement);
    free(statement);
    return DB_RETURN_SUCCES;
}

int createNewUser(char *username, char *password)
{
    if(username == NULL || password == NULL) return DB_RETURN_NULLPOINTER;

    char*values = malloc(strlen(username)+ strlen(password) + strlen(username)+10);
    sprintf(values,"'%s','%s','%s'",username,password,username);

    char* statement = getInsertSQL("USERS","name, password, nickname",values);

    free(values);
    executeStatement(statement);

    free(statement);
    return DB_RETURN_SUCCES;
}
