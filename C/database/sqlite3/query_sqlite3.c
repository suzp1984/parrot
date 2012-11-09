#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ENG_DB "./engtest.db"
#define ENG_TEST_TABLE "test"
#define ENG_SQL_LEN 128
#define ALL_VALUE "*"

typedef struct {
	char* name;
	int result;
} str2int_res;

static int eng_str2int_callback(void* param, int argc, char** argv, char** cname)
{
	int i;
	str2int_res* result = (str2int_res*)param;

	for (i = 0; i < argc; i++) {
		printf("\t %s = %s\n", cname[i], argv[i]);
		if (strcmp("value", cname[i]) == 0) {
			result->result = atoi(argv[i]);
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{
	sqlite3* db = NULL;
	char sql_createtable[ENG_SQL_LEN];
	char* errmsg = NULL;
	int rc = 0;
	int ret = 0;

	str2int_res str_res;
	memset(&str_res, 0, sizeof(str2int_res));

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
	if (argc < 2) {
		sprintf(sql_createtable, "SELECT * FROM %s", ENG_TEST_TABLE);
	} else {
		sprintf(sql_createtable, "SELECT * FROM %s WHERE name='%s'", ENG_TEST_TABLE, argv[1]);
	}

	rc = sqlite3_exec(db, sql_createtable, &eng_str2int_callback, &str_res, &errmsg);

	if (rc == 1) {
		printf("%s: %s already exists.\n", __func__, ENG_DB);
	} else if (rc != 0) {
		printf("%s: query reate table fail, errmsg = %s [%d:%s]\n", __func__, errmsg, 
				sqlite3_errcode(db),  sqlite3_errmsg(db));
		ret = -1;

		goto out;
	} else {
		printf("%s: query table %s success\n", __func__, ENG_DB);
	}
out:
	sqlite3_close(db);
	return ret;
}
