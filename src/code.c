#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH_USERNAME 256
#define MAX_LENGTH_URL 128 

int main(int argc, char* argv[])
{
	if(argc < 4 || argc > 4)
	{
		printf("Too few or too many arguments passed.\nThis should be called as: ./test username random_number page_url\n");
		return 0;
	}
	
	char username[MAX_LENGTH_USERNAME + 1];
	long long int random_number;
	char url[MAX_LENGTH_URL + 1];
	
	strncpy(username, argv[1], MAX_LENGTH_USERNAME);
	//random_number = atio_for_long_long(argv[2]);
	strncpy(url, argv[3], MAX_LENGTH_URL);

	printf("The Username : %s and corresponding url: %s\n", username, url);
	return 0;
}
