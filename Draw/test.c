#include <stdio.h>
#include <string.h>
int main()
{
	char buffer[6] = "hello";
	char* buffer2 = "hello";

	if(strcmp(buffer, buffer2))
	{
		printf("Works\n");
	}
	else
	{
		printf("Failed\n");
	}
	return 0;
}
