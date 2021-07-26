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
	db_connection_t dbc;

	int retval;
	while(FCGI_Accept() >= 0){
		char* query_string = getenv("QUERY_STRING");
		char* username = parse_query_string(query_string);

	

		//if(argc < 3 || argc > 3)
		//{
		//	printf("Too few or too many arguments passed.\nThis should be called as: ./main username random_number\n");
		//	return 0;
		//}
	
		//char username[MAX_LENGTH_USERNAME + 1];
		long long int random_number;
		char url[MAX_LENGTH_URL + 1];
	
	//	strncpy(username, argv[1], MAX_LENGTH_USERNAME);
		//random_number = atio_for_long_long(argv[2]);
		//strncpy(url, argv[1], MAX_LENGTH_URL);

//		printf("The Username : %s and corresponding url: %s\n", username, url);
	
		retval = initialize_db_env(&env);
		if(retval != 0)
		{
		//	printf("\nERROR: DB Initialization Failed");
			return retval;
		}
		char homepage[MAX_HOMEPAGE_LEN];
		get_url_util(&env, username, homepage);
		//get_db_connection(&dbc);
		if(homepage == NULL){
		//	printf("\nERROR: Enable to find URL");
			return 0;
		}
		//printf("\nURL:%s\n", homepage);
		release_db_env(&env);
		//	free(homepage);
		printf("HTTP/1.1 301 Moved Permanently\r\n"
		"Location: %s \r\n"
		"Content-Type:text/html;charset=us-ascii\r\n"
		"Set-Cookie: UID=%s; Max-age=86400\r\n"
			"\r\n", homepage, username);
	}
		return 0;
}
