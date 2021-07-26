#include <fcgi_stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/fetch_url.h"
#include "../include/parse_query.h"

#define MAX_LENGTH_USERNAME 256
#define MAX_LENGTH_URL 128 

int main()
{
	db_env_t env;
	int log_fd = -1;
	char log_file[128];
	int retval;
	get_log_file_name(log_file);

	log_fd = open(log_file, (O_CREAT|O_WRONLY|O_APPEND), (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH));
	if(log_fd < 0){
		fprintf(stderr, "ERROR:/INFO: Failed to open Log File\n");
		return 0;
	}
	fprintf(stderr, "Writing Logs to %s\n", log_file);

	int tmp_fd = dup2(log_fd, 2);
	if(tmp_fd < 0){
		fprintf(stderr, "ERROR:/INFO: Failed to dup log file descriptor %s\n", log_file);
		return 0;
	}

	retval = initialize_db_env(&env);
	if(retval != 0){
		fprintf(stderr, "ERROR:/INFO: DB Initialization Failed\n");
		return 0;
	}
	while(FCGI_Accept() >= 0){
		char* query_string = getenv("QUERY_STRING");
		char* username = parse_query_string(query_string);
	
		long long int random_number;
		char url[MAX_LENGTH_URL + 1];
	
		char homepage[MAX_HOMEPAGE_LEN];
		get_url_util(&env, username, homepage);
		
		if(homepage == NULL){
			fprintf(stderr, "ERROR:/INFO: Homepage not found for given Username : %s\n", username);
			return 0;
		}
	
		printf("HTTP/1.1 301 Moved Permanently\r\n"
		"Location: %s \r\n"
		"Content-Type:text/html;charset=us-ascii\r\n"
		"Set-Cookie: UID=%s; Max-age=86400\r\n"
			"\r\n", homepage, username);
	}
	release_db_env(&env);
		return 0;
}
