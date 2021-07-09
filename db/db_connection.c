#include <stdio.h>
#include "include/db_connection.h"
#define MAX_DSN_STRING_LEN 256

int initialize_db_env(db_env_t *env)
{
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &(env->env_handle));

	if(env->env_handle == 0){
		return DB_ERROR;
	}
	
	SQLSetEnvAttr(env->env_handle, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0);
	return DB_SUCCESS;
}
	
int get_connection(db_env_t *env, db_connection_t *dbc, char* dsnname, char* username, char* password)
{
	SQLRETURN retval = SQL_SUCCESS;
	SQLAllocHandle(SQL_HANDLE_DBC, env->env_handle, &(dbc->con_handle));

	if(dbc->con_handle == 0){
		return DB_ERROR;
	}

	char dsn_string[MAX_DSN_STRING_LEN + 1];
	sprintf(dsn_string, "DSN=%s", dsnname);
	//printf("  %s  ", dsn_string);
	
	retval = SQLDriverConnect(dbc->con_handle, NULL, dsn_string, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
	if((dbc->con_handle) != 0 && (retval == SQL_SUCCESS)){
		return DB_SUCCESS;
	}

	if(dbc->con_handle != 0){
		SQLFreeHandle(SQL_HANDLE_DBC, dbc->con_handle);
	}
	
	return DB_ERROR;
}

void release_db_env(db_env_t *env)
{
	if(env->env_handle != 0){
		SQLFreeHandle(SQL_HANDLE_ENV, env->env_handle);
	}
}

void release_connection(db_connection_t *dbc)
{
	if(dbc->con_handle != 0){
		SQLDisconnect(dbc->con_handle);
	}

	if(dbc->con_handle != 0){
		SQLFreeHandle(SQL_HANDLE_DBC, dbc->con_handle);
	}
}
/*
int main()
{
	db_env_t env;
	int retval = initialize_db_env(&env);
	printf("DONE");
	db_connection_t dbc;
	retval = get_connection(&env, &dbc, "Assign", "root", "pubmatic");
	char query[] = "select homepage from user_homepage_map where username = '%s';";
//	int val = get_url(&dbc, query, "abc_77");
	return 0;
}*/
