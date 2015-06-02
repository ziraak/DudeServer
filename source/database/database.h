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

sqlite3 *db;

#define ALL_COLUMNS "*"
#define SETUP_ERROR_RETURN(val, err) printf("ERROR: %s\n", err); sqlite3_close(db); return val
#define STMT_RETURN(val, stmt) sqlite3_finalize(stmt); return val

int setupDatabaseConnection();
int tableExists(char* name);

char* sqlite3_column_string(sqlite3_stmt *stmt, int id);
char* getSelectSQL(char* table, char* columns, char* where);

void executeStatement(char* stmt);

void stopDatabase();

#endif
