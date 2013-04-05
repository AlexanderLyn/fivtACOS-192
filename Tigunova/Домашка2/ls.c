#include<stdio.h>
#include<dirent.h>

int main(int argc, char** argv) {
	struct dirent *f;
	DIR *d;
	if (argc>1) {
		d=opendir(argv[1]);
	}
	else
		d=opendir(".");
	f=readdir(d);
	while (f!=NULL) {
	   printf("%s\n", f->d_name);
	   f=readdir(d);
	}
	getchar();
	return 0;
}