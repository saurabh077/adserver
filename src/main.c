#include <fcgi_stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/fetch_url.h"
#include "../include/parse_query.h"

#define MAX_LENGTH_USERNAME 256
#define MAX_LENGTH_URL 128 

int main()
{
	db_env_t env;
	
	int retval;
	while(FCGI_Accept() >= 0){
		char* query_string = getenv("QUERY_STRING");
		char* username = parse_query_string(query_string);
	
		long long int random_number;
		char url[MAX_LENGTH_URL + 1];
	
		retval = initialize_db_env(&env);
		if(retval != 0)
		{
		//	printf("\nERROR: DB Initialization Failed");
			return retval;
		}
		char homepage[MAX_HOMEPAGE_LEN];
		get_url_util(&env, username, homepage);
		
		if(homepage == NULL){
		//	printf("\nERROR: Enable to find URL");
			return 0;
		}
		//printf("\nURL:%s\n", homepage);
		release_db_env(&env);
	
		printf("HTTP/1.1 301 Moved Permanently\r\n"
		"Location: %s \r\n"
		"Content-Type:text/html;charset=us-ascii\r\n"
		"Set-Cookie: UID=%s; Max-age=86400\r\n"
			"\r\n", homepage, username);
	}
		return 0;
}
