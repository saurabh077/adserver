#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>
#include <string.h>

#define MAX_COLS 1
#define MAX_COL_NAME_LEN 256
#define MAX_USERNAME_LEN 256

void get_url(int argc, char* argv[])
{
	SQLHENV env;
	SQLHDBC dbc;
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

	SQLCHAR query[MAX_COL_NAME_LEN];
	SQLSMALLINT num_cols;
	char username[MAX_USERNAME_LEN];
	if(argc < 2){
		printf("\nToo few arguments passed");
		return ;
	}

	strncpy(username, argv[1], MAX_USERNAME_LEN);

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
		printf("\nERROR:Failed to allocate Environment Handle");
		return ;
	}

	retcode = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0);

	retcode = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
	if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
		printf("\nERROR:Failed to allocate Connection Handle");
		SQLFreeHandle(SQL_HANDLE_ENV, env);
		return ;
	}

	retcode = SQLDriverConnect(dbc, NULL, "DSN=Assign", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
	if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
		printf("\nERROR:Failed to connect to DSN");
		if(dbc != 0){
			SQLFreeHandle(SQL_HANDLE_DBC, dbc);
		}	
		if(env != 0){
			SQLFreeHandle(SQL_HANDLE_ENV, env);
		}	
		return ;
	}

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
	if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
		printf("\nERROR:Failed to allocate Statement Handle");
		
	 	 if(dbc != 0){
			SQLDisconnect(dbc);
             SQLFreeHandle(SQL_HANDLE_DBC, dbc);
         }
         if(env != 0){
             SQLFreeHandle(SQL_HANDLE_ENV, env);
         }   
		return ;
	}

	sprintf(query, "select homepage from user_homepage_map where username = '%s';", username);
	//printf("\n query %s\n", query);
	

	retcode = SQLPrepare(stmt, query, strlen(query));
	if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
		printf("\nERROR:Failed to prepare");
		if(dbc != 0){
			SQLDisconnect(dbc);
			SQLFreeHandle(SQL_HANDLE_DBC, dbc);
		}
		if(env != 0){
			SQLFreeHandle(SQL_HANDLE_ENV, env);
		}
		return ;
	}
	
	retcode = SQLNumResultCols(stmt, &num_col);
	if(num_cols  == 0){
		printf("\nUsername not found in the Database");
		if(dbc != 0){
			SQLDisconnect(dbc);
			SQLFreeHandle(SQL_HANDLE_DBC, dbc);
		}
		if(env != 0){
			SQLFreeHandle(SQL_HANDLE_ENV, env);
		}
		return ;
	}	
	printf("\nNumber of columns in result:%i", num_cols);

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
	
	for( rows_count = 0 ; ; rows_count++){
		retcode = SQLFetch(stmt);
		if(retcode == SQL_NO_DATA)
			break;
		
		//printf("\nRecord %i\n", rows_count+1);
		for(cols_count = 0  ; cols_count < num_cols ; cols_count++){
			//printf("Column %s:", column_name[cols_count]);
			//considering data type is varchar
			printf("For Username:%s \nHomepage is:%s\n", username, column_data[cols_count]);
		}
	}

	//Free all the handles allocated
	if(stmt != SQL_NULL_HSTMT)
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	
	if(dbc != SQL_NULL_HDBC){
		SQLDisconnect(dbc);
		SQLFreeHandle(SQL_HANDLE_DBC, dbc);
	}

	if(env != SQL_NULL_HENV)
		SQLFreeHandle(SQL_HANDLE_ENV, env);
	return ;
}


int main(int argc, char* argv[])
{
	//printf("\nSTART");
	get_url(argc, argv);
	return 0;
}
