#include <stdio.h>
#include <dirent.h>

int main(int argc, char** argv)
{
	DIR* f;
	if(argc<2)
	{
		f = opendir("./");
	}
	else if(argc>2)
	{
		printf("Too many arguments\n");
		return 0;
	}
	else f = opendir(argv[1]);
	if (f == NULL)
	{
		printf("ERROR: no such directory\n");
		return 0;
	}
	struct dirent* temp = readdir(f);
	while(temp)
	{
		printf("%s\n",temp->d_name); 
		temp = readdir(f);
	} 
	closedir(f);
	return 0;
}
