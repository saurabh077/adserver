#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include <string.h>
//#include "../db/include/db_connection.h"
#include "../include/fetch_url.h"
#define MAX_COLS 1
#define MAX_COL_NAME_LEN 256
#define MAX_USERNAME_LEN 256



//requires input username
void get_url(db_connection_t *dbc, char* query, char* username, char* homepage)
{
	SQLHSTMT stmt;
	SQLSMALLINT columns;
	SQLRETURN retcode;

	SQLCHAR* 	column_name[MAX_COLS];
	SQLSMALLINT column_name_len[MAX_COLS];
	SQLSMALLINT column_data_type[MAX_COLS];
	SQLULEN 	column_data_size[MAX_COLS];
	SQLSMALLINT column_data_digits[MAX_COLS];
	SQLSMALLINT column_data_nullable[MAX_COLS];
	SQLCHAR* 	column_data[MAX_COLS];
	SQLLEN 		column_data_len[MAX_COLS];
	SQLSMALLINT rows_count, cols_count;

	SQLCHAR sql_query[MAX_SQL_QUERY_STR_LEN];
	SQLSMALLINT num_cols;
	
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, dbc->con_handle, &stmt);
	//char *homepage;

//	sprintf(query, "select homepage from user_homepage_map where username = '%s';", username);
	sprintf(sql_query, query, username);
	//printf("\nQUERY :%s",  sql_query);
	retcode = SQLPrepare(stmt, sql_query, strlen(sql_query));
	if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
		printf("\nERROR:Preparing Statement");
		
	 	 if(stmt != 0){
             SQLFreeHandle(SQL_HANDLE_STMT, stmt);
         }
		 return ;
	}
	
	retcode = SQLNumResultCols(stmt, &num_cols);
//	printf("\nNo of cols : %d", num_cols);
	if(num_cols  == 0){
		printf("\nUsername not found in the Database");
		if(stmt != 0)
			SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return ;
	
	}	
	//printf("\nNumber of columns in result:%i", num_cols);
	int i = 0;
	for(int i=0;i<num_cols;i++){
		column_name[i] = (SQLCHAR *) malloc (MAX_COL_NAME_LEN);
		retcode = SQLDescribeCol(
					stmt,
					i+1, 
					column_name[i],
					MAX_COL_NAME_LEN,
					&column_name_len[i],
					&column_data_type[i],
					&column_data_size[i],
					&column_data_digits[i],
					&column_data_nullable[i]);
		
		//printf("\nColumn :%i\n", i+1);
		//printf("Column Name : %s\n Column Name Length :%i\n SQL Data Type :%i\n Data Size : %i\n Decimal Digits : %i\n Nullable %i\n", column_name[i], (int)column_name_len[i], (int)column_data_type[i], (int)column_data_size[i], (int)column_data_digits[i], (int)column_data_nullable[i]);
		
		column_data[i] = (SQLCHAR *) malloc(column_data_size[i] + 1);
		switch(column_data_type[i]){
			case SQL_VARCHAR:
				column_data_type[i] = SQL_C_CHAR;
				break;
			default : 
				column_data_type[i] = SQL_C_CHAR;
		}

		retcode = SQLBindCol (stmt, 
								i+1, 
								column_data_type[i],
								column_data[i],
								column_data_size[i],
								&column_data_len[i]);
	}

	//printf("\n Records \n");
	retcode = SQLExecute(stmt);
	
	for( rows_count = 0 ;rows_count < 1 ; rows_count++){
		retcode = SQLFetch(stmt);
		if(retcode == SQL_NO_DATA)
			break;
		
		//printf("\nRecord %i\n", rows_count+1);
		for(cols_count = 0  ; cols_count < num_cols ; cols_count++){
			//printf("Column %s:", column_name[cols_count]);
			//considering data type is varchar
			printf("\nFor Username:%s \nHomepage is:%s\n", username, column_data[cols_count]);
			strncpy(homepage, column_data[cols_count], MAX_HOMEPAGE_LEN);
			//homepage = column_data[cols_count];
		}
	}

	//Free all the handles allocated
	for(int i=0;i<num_cols;i++)
	{
		free(column_data[i]);
		free(column_name[i]);
	}
	
	if(stmt != SQL_NULL_HSTMT)
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	/*
	if(dbc != SQL_NULL_HDBC){
		SQLDisconnect(dbc);
		SQLFreeHandle(SQL_HANDLE_DBC, dbc);
	}

	if(env != SQL_NULL_HENV)
		SQLFreeHandle(SQL_HANDLE_ENV, env);*/
	return ;
}


char* get_url_util(db_env_t *env, char* username, char* homepage)
{
	SQLRETURN ret_val = SQL_SUCCESS;
	db_connection_t dbc;
	ret_val = get_connection(env, &dbc, "Assign", NULL, NULL);
	if(ret_val != SQL_SUCCESS){
		printf("\nERROR: DB Connection Failed");
		return NULL;
	}
	char query[] = "select homepage from user_homepage_map where username = '%s';";
	
	get_url(&dbc, query, username,homepage);
	release_connection(&dbc);
	if(homepage == NULL){
		printf("\nERRORR: Could not get URL");
		return NULL;
	}
	return homepage;
}

/*
int main(int argc, char* argv[])
{
	//printf("\nSTART");
	get_url(argc, argv);
	return 0;
}*/
