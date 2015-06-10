#ifndef DUDESERVER_DATABASE_H
#define DUDESERVER_DATABASE_H

#include <sqlite3.h>
#include "../main.h"

#include "databaseStructs.h"
#include "dbDefines.h"
#include "user.h"
#include "channel.h"
#include "channelUser.h"
#include "channelMessage.h"
#include "../ssl/communication.h"

sqlite3 *db;

#define ALL_COLUMNS "*"
#define SETUP_ERROR_RETURN(val, err) printf("ERROR: %s\n", err); sqlite3_close(db); return val
#define STMT_RETURN(val, stmt) sqlite3_finalize(stmt); return val

int setupDatabaseConnection();
int tableExists(char* name);

char* sqlite3_column_string(sqlite3_stmt *stmt, int id);
char* getSelectSQL(char* table, char* columns, char* where);
char* getInsertSQL(char* table,char* valueNames ,char* values);
char* getDeleteSQL(char* table, char* where);
char* getUpdateSQL(char* table, char* where, char* valueName, char* newValue);

void executeStatement(char* stmt);

void stopDatabase();

#endif
