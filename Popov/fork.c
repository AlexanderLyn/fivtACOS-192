#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if(argc<2) return 0;
    long long n = atoi(argv[1]);
    int t = fork();
    int start, end;
    switch (t)
    {
        case 0:
        {
            long long i;
            long long sum = 0;
            for(i=1;i<n/2;++i) sum+=i;
            FILE* f = fopen("1.txt","w");
            fprintf(f,"%lld",sum);
            fclose(f);
            break;
        }
        case -1:
        {

            break;
        }
        default:
        {
            long long i,sum = 0;
            for(i=n/2;i<=n;++i) sum+=i;
            int d = wait(0);
            FILE* f = fopen("1.txt","r");
            fscanf(f,"%lld",&i);
            fclose(f);
            printf("%lld\n",sum+i);
            break;
        }
    }

    return 0;
}
