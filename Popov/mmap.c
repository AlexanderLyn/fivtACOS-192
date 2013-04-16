#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	long long n = 1000000000;
	if(argc<2)
	{
		printf("not enough arguments\n");
		return -1;
	}
	FILE* fd = fopen(argv[1],"r");
	if(fd == NULL)
	{
		printf("error opening file\n");
		return -1;
	}
	FILE* result = fopen("temp.mmap","w+");
	long long ta[2];
	fwrite(ta,sizeof(long long),2,result);
	fclose(result);
	result = fopen("temp.mmap","a+");
	int* a = mmap(NULL, n*sizeof(int),PROT_READ,MAP_SHARED,fileno(fd),0);
	long long* b = mmap(NULL, 2*sizeof(long long),PROT_READ|PROT_WRITE,MAP_SHARED,fileno(result),0);
	int t = fork();
	    switch (t)
	    {
	        case 0:
	        {
	            long long i;
	            long long sum = 0;
	            for(i=1;i<n/2;++i) sum+=a[i-1];
	            b[0]=sum;
	            break;
	        }
	        case -1:
	        {

	            break;
	        }
	        default:
	        {
	            long long i,sum = 0;
	            for(i=n/2;i<=n;++i) sum+=a[i-1];
	            b[1] = sum;
	            int d = wait(0);
	            b[0] += b[1];
	            printf("%lld\n",b[0]);
	            break;
	        }
	    }
	munmap(a,n*sizeof(int));
	munmap(b,2*sizeof(long long));
	return 0;
}
