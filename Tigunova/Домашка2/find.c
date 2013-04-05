#include<stdio.h>
#include<dirent.h>
#include<string.h>


int boundcheck(char* a, char* b) {

	int l=0;

	char lb,ub;

	while (*a!=']') {
		lb=*(a+1);
		ub=*(a+3);

		if (l==0 && (int)*b>=(int)(lb) && (int)*b<=(int)(ub))
		l=1;
		a+=4;
	}

	return l;
}


int compar(char* b, char* a) {

	int l=1;

	while (*a != NULL && *b!=NULL) {
		l=0;
		if (*a=='[') {
			l=boundcheck(a,b);

			while (*a!=']') a++;
		}

		if (*a=='*') {
			char* c;

			int ret=-1;
			a++;

			if (*a != NULL && *a!='.') {

			if (*a=='[') {

			while (*b!=NULL && ret==-1) {

			while (*b != NULL && boundcheck(a,b) != 1) b++;
			c=a;

			while (*c!=']') c++;
			if (c+1!=NULL) ret=compar(c+1, b+1);
			b++;
			}
			a=c;
			b--;
			}

			else {

			while (*b!=NULL && ret==-1) {

			while (*b != NULL && *a!=*b) b++;
			if (a+1!=NULL) ret=compar(a+1, b+1);
			b++;
			}
			b--;
			}

			if (*b != NULL) l=1;
		}

		else {

			if (*a=='.') {

				while (*b!='.') b++;

				if (*b != NULL) l=1;
			}

			else {
			return 0;}
		}
		}

		if (*a=='?' && *b != NULL) {
		l=1;
		}

		if (l==0) {

		if (*a==*b) l=1;
		}
		a++;
		b++;


		if (l==0) return -1;
	}

	if (*a==NULL && *b==NULL) return 0;
}



int main(int argc, char** argv) {
	struct dirent *f;
	DIR *d;
	if (argv[1][0]!='-') {
		d=opendir(argv[1]);
	}
	else
		d=opendir(".");
	if (d==NULL) {
			printf("d is null");
			return 1;
		}
	f=readdir(d);
	if (f==NULL) {
		printf("f is null");
		return 1;
	}
	if (strcmp(argv[2],"-name")==0) {
				while (f!=NULL) {
				   if (compar(f->d_name,argv[3])==0)
					   printf("%s\n", f->d_name);
				   f=readdir(d);
				}
			}
	if (strcmp(argv[1],"-name")==0) {
			while (f!=NULL) {
			   if (compar(f->d_name,argv[2])==0)
				   printf("%s\n", f->d_name);
			   f=readdir(d);
			}
		}
	return 0;
}
