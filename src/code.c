#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/fetch_url.h"
//#include "../db/include/db_connection.h"

#define MAX_LENGTH_USERNAME 256
#define MAX_LENGTH_URL 128 

int main(int argc, char* argv[])
{
	db_env_t env;
	db_connection_t dbc;

	int retval;

	if(argc < 3 || argc > 3)
	{
		printf("Too few or too many arguments passed.\nThis should be called as: ./test username random_number\n");
		return 0;
	}
	
	char username[MAX_LENGTH_USERNAME + 1];
	long long int random_number;
	char url[MAX_LENGTH_URL + 1];
	
	strncpy(username, argv[1], MAX_LENGTH_USERNAME);
	//random_number = atio_for_long_long(argv[2]);
	//strncpy(url, argv[1], MAX_LENGTH_URL);

//	printf("The Username : %s and corresponding url: %s\n", username, url);
	
	retval = initialize_db_env(&env);
	if(retval != 0)
	{
		printf("\nERROR: DB Initialization Failed");
		return retval;
	}
	char homepage[MAX_HOMEPAGE_LEN];
	get_url_util(&env, username, homepage);
	//get_db_connection(&dbc);
	if(homepage == NULL){
		printf("\nERROR: Enable to find URL");
		return 0;
	}
	printf("\nURL:%s\n", homepage);
	release_db_env(&env);
//	free(homepage);
	return 0;
}
