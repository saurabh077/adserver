#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#define MAX_SQL_QUERY_STR_LEN 8192
#define DB_ERROR 1
#define DB_SUCCESS 0

typedef struct db_env{
	SQLHANDLE env_handle;
} db_env_t;

typedef struct db_connection{
	SQLHANDLE con_handle;
}db_connection_t;

int initialize_db_env(db_env_t *env);

int get_connection(db_env_t *env, db_connection_t *dbc, char* dsnname, char* username, char* password);

void release_connection(db_connection_t *dbc);

void release_db_env(db_env_t *env);

//int get_url(db_connection_t *dbc, char* query, char* username);
