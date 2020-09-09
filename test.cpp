#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 


void update_callback(void *stuff, int op, const char *db, const char *table, sqlite_int64 rowid)
{
	 printf("%s table %d row was updated\n",table, rowid );
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) 
{
	int i;
	for(i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char* argv[]) 
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;

	/* Open database */
	rc = sqlite3_open("test.db", &db);
	
	sqlite3_update_hook(db, update_callback, NULL);

	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	} 

	else {
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
			"VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
			"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
			"VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "	  \
			"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
			"VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
			"INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
			"VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} 
	else {
		fprintf(stdout, "Records created successfully\n");
	}
	sqlite3_close(db);
	return 0;
}
