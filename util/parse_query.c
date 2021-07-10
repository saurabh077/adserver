#include <stdio.h>
#include <string.h>
#include "../include/parse_query.h"

char* parse_query_string(char* str)
{
	//char str[] = "username=Anil&ran=0.97584378943&pageURL=http://xyz.com";
	int init_size = strlen(str);
	char delim[] = "&";

	char *ptr = strtok(str, delim);

	char *get_user_ptr = strtok(ptr, "=");

	if(!strcmp(get_user_ptr, "username")){
		get_user_ptr = strtok(NULL, delim);
		//printf("%s", get_user_ptr);
		return get_user_ptr;
	}
	return NULL;
}
