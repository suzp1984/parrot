#ifndef DB_OPEN_HELPER_H
#define DB_OPEN_HELPER_H

#include "typedef.h"
#include "sqlit_db.h"

struct _DbOpenHelper;
typedef struct _DbOpenHelper DbOpenHelper;

typedef SqliteDb* (*DbOpenHelperGetDb)(DbOpenHelper* thiz);
typedef Ret (*DbOpenHelperCreate)(DbOpenHelper* thiz);
typedef Ret (*DbOpenHelperGetDbName)(DbOpenHelper* thiz, char** name);
typedef Ret (*DbOpenHelperUpgrade)(DbOpenHelper* thiz);
typedef Ret (*DbOpenHelperClose)(DbOpenHelper* thiz);
typedef void (*DbOpenHelperDestroy)(DbOpenHelper* thiz);

typedef _DbOpenHelper {
	DbOpenHelperGetDb get_db;
	DbOpenHelperCreate create;
	DbOpenHelperGetDbName get_name;
	DbOpenHelperUpgrade upgrade;
	DbOpenHelperClose close;
	DbOpenHelperDestroy destroy;


	char priv[1];
};

static inline Ret db_open_helper_get_db(DbOpenHelper* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->get_db < 0, RET_OK);

	return thiz->get_db(thiz);
}


/*
DbOpenHelper* db_open_helper_create();
SqliteDb* db_open_helper_get_db(DbOpenHelper* thiz);
int db_open_helper_create_db(DbOpenHelper* thiz);
int db_open_helper_on_upgrade(DbOpenHelper* thiz);
int db_open_helper_close(DbOpenHelper* thiz);
void db_open_helper_destroy(DbOpenHelper* thiz);
*/

#endif // DB_OPEN_HELPER_H
