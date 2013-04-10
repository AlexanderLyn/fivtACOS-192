#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("Too few arguments\n");
		return 0;
	}
	if(argc>2)
	{
		printf("Too many arguments\n");
		return 0;
	}
	FILE* f = fopen(argv[1], "r");
	char temp;
	while(!feof(f))
	{
		size_t x = fread(&temp,sizeof(char),1,f);
		printf("%c",temp); 
	} 
	fclose(f);
	return 0;
}
