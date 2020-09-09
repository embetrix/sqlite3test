#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 


void update_callback(void *stuff, int op, const char *db, const char *table, sqlite_int64 rowid)
{
	 printf("%s table %d row was updated\n",table, (int)rowid );
}

static int callback(void *, int argc, char **argv, char **colname)
{
   int i;

   for (i = 0; i<argc; i++) {

      printf("%s = %s\n", colname[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

int main(int argc, char* argv[]) 
{
	sqlite3 *db;
	char *ErrMsg = 0;
	int rc;
	const char *sql;


	/* Open database */
	rc = sqlite3_open("test.db", &db);
	
	sqlite3_update_hook(db, update_callback, NULL);

	if ( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	} 

	else {
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Insert into Database */
	sql =   "insert into param_tab values(NULL, 1,  0,  10,   5, \"KHZ\", \"frequency\", 1);"  \
		"insert into param_tab values(NULL, 2,  2,  25,  10, \"V\",   \"voltage\",   1);"  \
		"insert into param_tab values(NULL, 3,  2,  30,  10, \"V\",   \"voltage\",   1);"  \
		"insert into param_tab values(NULL, 4,  0, 100,  30, \"W\",   \"power\",     1);"  \
		"insert into param_tab values(NULL, 8,  0, 120,  30, \"W\",   \"power\",     1);"  \
		"insert into param_tab values(NULL, 9,  0,  10,   1, \"mA\",  \"current\",   1);"  \
		"insert into param_tab values(NULL, 10, 1,  10,   1, \"mA\",  \"current\",   1);";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, NULL, 0, &ErrMsg);
	
	if ( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", ErrMsg);
		sqlite3_free(ErrMsg);
	} 
	else {
		fprintf(stdout, "Records created successfully\n");
	}


	/* select from Database */
	sql =   "select * from param_tab where unit = \"KHZ\" or unit = \"mA\";"; 

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, NULL, &ErrMsg);
	
	if ( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", ErrMsg);
		sqlite3_free(ErrMsg);
	} 
	else {
		fprintf(stdout, "Records created successfully\n");
	}

	sqlite3_close(db);

	return 0;
}
