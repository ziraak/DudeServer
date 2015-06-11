#include "database.h"

void executeStatement(char* stmt)
{
    char* err;

    if(sqlite3_exec(db, stmt, NULL, NULL, &err) != SQLITE_OK)
    {
        printf("ERROR: %s\nIN STATEMENT: %s\n", err, stmt);
        sqlite3_free(err);
    }
}

void firstTimeSetup()
{
    printf("-- PERFORMING FIRST TIME SETUP --\n\n");

    executeStatement("CREATE TABLE CHANNELS (name TEXT PRIMARY KEY, password TEXT,topic TEXT, visible INT NOT NULL, inviteOnly INT NOT NULL, topicOperatorOnly INT NOT NULL);");
    char *passwordBatcave = passwordEncrypt("open sesame");
    char *insertBatcaveStatement = malloc(200);
    sprintf(insertBatcaveStatement, "INSERT INTO CHANNELS (name, password, topic, visible, inviteOnly, topicOperatorOnly) VALUES ('batcave', '%s','because im awesome', 1, 0, 0);", passwordBatcave);
    executeStatement(insertBatcaveStatement);
    free(insertBatcaveStatement);
    executeStatement("INSERT INTO CHANNELS (name, password, topic, visible, inviteOnly, topicOperatorOnly) VALUES ('eigendunk', NULL,'erg bekend bij ...', 0, 0, 1);");
    executeStatement("INSERT INTO CHANNELS (name, password, topic, visible, inviteOnly, topicOperatorOnly) VALUES ('testChan', NULL,'Test channel', 0, 0, 1);");

    executeStatement("CREATE TABLE USERS (name TEXT PRIMARY KEY, password TEXT NOT NULL, nickname TEXT);");
    createNewUser("fatih","nub");
    createNewUser("desmond","nub");
    createNewUser("sjuul","nub");
    createNewUser("harmen","nub");
    createNewUser("ferdi","nub");
    createNewUser("testUser","wachtwoord");


    executeStatement("CREATE TABLE CHANNEL_USERS (user_name TEXT NOT NULL, channel_name TEXT NOT NULL, user_privileges TEXT NOT NULL, PRIMARY KEY(user_name, channel_name), FOREIGN KEY(user_name) REFERENCES USERS(name), FOREIGN KEY(channel_name) REFERENCES CHANNELS(name));");
    executeStatement("INSERT INTO CHANNEL_USERS (user_name, channel_name, user_privileges) VALUES ('fatih',  'batcave', 'u');");
    executeStatement("INSERT INTO CHANNEL_USERS (user_name, channel_name, user_privileges) VALUES ('desmond','batcave', 'u');");
    executeStatement("INSERT INTO CHANNEL_USERS (user_name, channel_name, user_privileges) VALUES ('sjuul',  'batcave', 'u');");
    executeStatement("INSERT INTO CHANNEL_USERS (user_name, channel_name, user_privileges) VALUES ('harmen', 'batcave', 'o');");
    executeStatement("INSERT INTO CHANNEL_USERS (user_name, channel_name, user_privileges) VALUES ('ferdi',  'batcave', 'u');");
    executeStatement("INSERT INTO CHANNEL_USERS (user_name, channel_name, user_privileges) VALUES ('fatih',  'eigendunk','u');");
    executeStatement("INSERT INTO CHANNEL_USERS (user_name, channel_name, user_privileges) VALUES ('sjuul',  'eigendunk','o');");
    executeStatement("INSERT INTO CHANNEL_USERS (user_name, channel_name, user_privileges) VALUES ('testUser',  'testChan','o');");

    executeStatement("CREATE TABLE CHANNEL_MESSAGES (message_id INTEGER PRIMARY KEY, user_name TEXT NOT NULL, channel_name TEXT NOT NULL, timestamp INTEGER NOT NULL, body TEXT, FOREIGN KEY(channel_name) REFERENCES CHANNELS(name));");
    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('fatih', 'batcave', 1433360988, 'im batman1');");
    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('fatih', 'batcave', 1433360989, 'im batman2');");
    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('fatih', 'batcave', 1433360990, 'im batman3');");
    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('fatih', 'batcave', 1433360991, 'im batman4');");
    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('fatih', 'batcave', 1433360992, 'im batman5');");

    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('sjuul', 'eigendunk', 1433360988, 'im batman1');");
    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('sjuul', 'eigendunk', 1433360989, 'im batman2');");
    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('sjuul', 'eigendunk', 1433360990, 'im batman3');");
    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('sjuul', 'eigendunk', 1433360991, 'im batman4');");
    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('sjuul', 'eigendunk', 1433360992, 'im batman5');");
    executeStatement("INSERT INTO CHANNEL_MESSAGES (user_name, channel_name, timestamp, body) VALUES ('testUser', 'testChan', 1433360992, 'Dit is een test channel!');");

    printf("\n-- FIRST TIME SETUP COMPLETED --\n");
}

int setupDatabaseConnection()
{
    int rc = sqlite3_open("dude@chat.db", &db);

    if(rc != SQLITE_OK)
    {
        SETUP_ERROR_RETURN(-1, sqlite3_errmsg(db));
    }

    executeStatement("PRAGMA foreign_keys = ON;");

    if(tableExists("CHANNELS") == BOOL_FALSE)
    {
        firstTimeSetup();
    }



    return DB_RETURN_SUCCES;
}
void stopDatabase()
{
    sqlite3_close(db);
}

int tableExists(char* name)
{
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, "SELECT name FROM sqlite_master WHERE type='table' AND name=?", -1, &stmt, NULL);
    if(rc == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            STMT_RETURN(BOOL_TRUE, stmt);
        }

        STMT_RETURN(BOOL_FALSE, stmt);
    }

    STMT_RETURN(BOOL_FALSE, stmt);
}

char* sqlite3_column_string(sqlite3_stmt *stmt, int id)
{
    char *stmt_res = (char*)sqlite3_column_text(stmt, id);

    if(stmt_res == NULL)
    {
        return NULL;
    }

    size_t l = strlen(stmt_res);
    char *func_return = MALLOC(l + 1);
    strncpy(func_return, stmt_res, l);
    return func_return;
}

char* getSelectSQL(char* table, char* columns, char* where)
{
    if(columns == NULL || strlen(columns) == 0)
    {
        columns = ALL_COLUMNS;
    }

    if(where == NULL || strlen(where) == 0)
    {
        char* result = MALLOC(strlen(columns) + strlen(table) + 15);
        sprintf(result, "SELECT %s FROM %s;", columns, table);
        return result;
    }
    else
    {
        char* result = MALLOC(strlen(columns) + strlen(where) + strlen(table) + 22);
        sprintf(result, "SELECT %s FROM %s WHERE %s;", columns, table, where);
        return result;
    }
}

char* getInsertSQL(char* table,char* valueNames ,char* values)
{
    if(table == NULL||valueNames == NULL || values == NULL) return NULL;

    char* statement = MALLOC(strlen(table)+ strlen(valueNames) + strlen(values)+ 27);
    sprintf(statement,"INSERT INTO %s (%s) VALUES (%s);",table,valueNames,values);
    return statement;
}

char* getDeleteSQL(char* table, char* where)
{
    if(table == NULL || where == NULL)
    {
        return NULL;
    }

    char* statement = MALLOC(strlen(table)+ strlen(where) + 21);
    sprintf(statement,"DELETE FROM %s WHERE %s;",table,where);
    return statement;
}

char* getUpdateSQL(char* table, char* where, char* valueName, char* newValue)
{
    if(table == NULL || where== NULL || valueName== NULL || newValue == NULL)
    {
        return NULL;
    }
    char* statement = MALLOC(strlen(table)+ strlen(where)+ strlen(valueName)+strlen(newValue) + 26);
    sprintf(statement,"UPDATE %s SET %s = '%s' WHERE %s;",table,valueName,newValue, where);
    return statement;
}
