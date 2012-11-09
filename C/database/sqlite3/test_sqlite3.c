#include <sqlite3.h>
#include <string.h>
#include <stdio.h>

#define ENG_DB "./engtest.db"
#define ENG_TEST_TABLE "test"
#define ENG_SQL_LEN 128

int main(int argc, char* argv[])
{
	sqlite3* db = NULL;
	char sql_createtable[ENG_SQL_LEN];
	char* errmsg = NULL;
	int rc = 0;
	int ret = 0;

	rc = sqlite3_open(ENG_DB, &db);

	if (rc != 0) {
		printf("%s: open %s fail [%d:%s]\n", __func__, ENG_DB, 
				sqlite3_errcode(db), sqlite3_errmsg(db));
		ret = -1;
		goto out;
	} else {
		printf("%s: open %s success\n", __func__, ENG_DB);
	}

	memset(sql_createtable, 0, ENG_SQL_LEN);
	sprintf(sql_createtable, "CREATE TABLE %s(name VARCHAR(32) PRIMARY KEY,value INTEGER);", ENG_TEST_TABLE);

	rc = sqlite3_exec(db, sql_createtable, NULL, NULL, &errmsg);

	if (rc == 1) {
		printf("%s: %s already exists.\n", __func__, ENG_DB);
	} else if (rc != 0) {
		printf("%s: create table fail, errmsg = %s [%d:%s]\n", __func__, errmsg, 
				sqlite3_errcode(db),  sqlite3_errmsg(db));
		ret = -1;

		goto out;
	} else {
		printf("%s: create table %s success\n", __func__, ENG_DB);
	}

out:
	sqlite3_close(db);
	return ret;
}

