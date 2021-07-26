#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "../include/parse_query.h"

char* parse_query_string(char* str)
{
	//char str[] = "username=Anil&ran=0.97584378943&pageURL=http://xyz.com";
//	int init_size = strlen(str);
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

void get_log_file_name(char* log_file)
{
	time_t now;
	struct tm *tm_now;
	struct timeval tv;
	char* strptr = NULL;
	int year = 0, month = 0, day = 0, nob = 0;
	int START_YEAR = 1900, START_MONTH = 1;
	char* log_dir = "/home/test/Desktop/Assignment/logs/";
	log_file[0] = '\0';

	now = time(NULL);
	tm_now = localtime( &now );
	if(tm_now == NULL){
		return; 
	}

	year = tm_now->tm_year + START_YEAR;
	month = tm_now->tm_mon + START_MONTH;
	day = tm_now->tm_mday;
	strptr = log_file;

	nob = sprintf(strptr, "%s", log_dir);
	strptr += nob;

	nob = sprintf(strptr, "%d-", year);
	strptr += nob;

	if(tm_now->tm_mon+START_MONTH < 10)
		nob = sprintf(strptr, "0%d-", month);
	else	
		nob = sprintf(strptr, "%d-", month);
	strptr += nob;
	
	if(tm_now->tm_mday < 10)
		nob = sprintf(strptr, "0%d-", day);
	else
		nob = sprintf(strptr, "%d", day);
	strptr += nob;

	gettimeofday(&tv, NULL);
	sprintf(strptr, "-%ld.log", tv.tv_sec);
}
