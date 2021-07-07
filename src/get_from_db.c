#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

void get_url()
{
	SQLHENV env;
	SQLHDBC dbc;
	SQLHSTMT stmt;
	SQLSMALLINT columns;
	SQLRETURN retcode;

	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
		printf("\nERROR:Failed to allocate Environment Handle");
		return ;
	}

	retcode = SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0);
	if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
		printf("\nERROR:Failed to set environment attributes");
	}

	retcode = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
	if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
		printf("\nERROR:Failed to allocate Connection Handle");
	}

	retcode = SQLDriverConnect(dbc, NULL, "DSN=Assign", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
	if((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)){
		printf("\nERROR:Failed to connect to DSN");
		return ;
	}
	
	printf("\nSuccessfully connected to a DSN");
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
	

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


int main()
{
	printf("\nSTART");
	get_url();
	return 0;
}
