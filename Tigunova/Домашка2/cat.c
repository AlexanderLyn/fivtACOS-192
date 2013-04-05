#include<stdio.h>

int main(int argc, char** argv) {
	if (argv[1] != NULL) {
		FILE* f=fopen(argv[1], "r");
		if (f != NULL) {
			char str;
			int res = fscanf(f,"%c",&str);
			while (res != -1) {
				printf("%c", str);
				res = fscanf(f,"%c",&str);
			}
		}
		else printf("file is null");
	}
	return 0;
}
