#ifndef DUDESERVER_DATABASE_H
#define DUDESERVER_DATABASE_H

#include <sqlite3.h>
#include "databaseStructs.h"
#include "channel.h"
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <malloc.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include "../main.h"
#include "dbDefines.h"


sqlite3 *db;


#define SETUP_ERROR_RETURN(val, err) printf("ERROR: %s\n", err); sqlite3_close(db); return val

#define STMT_RETURN(val, stmt) sqlite3_finalize(stmt); return val

void executeStatement(char* stmt);

int setupDatabaseConnection();
int tableExists(char* name);

char* sqlite3_column_string(sqlite3_stmt *stmt, int id);
char* getSelectSQL(char* table, char* columns, char* where);
char* getInsertSQL(char* table,char* valueNames ,char* values);
char* getDeleteSQL(char* table, char* where);
char* getUpdateSQL(char* table, char* where, char* valueName, char* newValue);

void stopDatabase();

#endif
