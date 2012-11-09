#ifndef SQLITE_DB_H
#define SQLITE_DB_H

struct _SqliteDb;
typedef struct _SqliteDb SqliteDb;

SqliteDb* sqlite_db_create();

void sqlite_db_insert(SqliteDb* thiz);
void sqlite_db_query(SqliteDb* thiz);
void sqlite_db_exec_sql(SqliteDb* thiz, const char* sql);

void sqlite_db_destroy(SqliteDb* thiz);


#endif // SQLITE_DB_H
