#include "../db/include/db_connection.h"


#define MAX_HOMEPAGE_LEN 256
char* get_url_util(db_env_t *env, char* username, char* homepage);
void get_url(db_connection_t *dbc, char* query, char* username, char* homepage);
