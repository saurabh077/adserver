#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(argc < 4 || argc > 4)
	{
		printf("Too few or too many arguments passed.\nThis should be called as: ./test username random_number page_url");
		return 0;
	}

	return 0;
}
