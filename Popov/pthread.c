#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define kStep 1000


void* threadFunc(void* ar)
{
	long long* arg = (long long*)ar;
	while(1)//(arg[1]<arg[2])
	{
		long long t = arg[1]++,i,sum=0;
		for(i=t*kStep;i<(t+1)*kStep && i<=arg[3];++i)
		{
			sum+=i;
		}
		//printf("%lld\n",sum);
		arg[0]+=sum;
		if(i >= arg[3]) return NULL;
	}
	return NULL;
}

int main(int argc, char** argv)
{
	if(argc<3)
	{
		printf("not enough arguments\n");
		return -1;
	}
	long long N = atoi(argv[1]);
	long long K = atoi(argv[2]);
	pthread_t* tred = malloc(K*sizeof(pthread_t));
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	long long arg[4];//1 - summ, 2 - curr index, 3 - limit, 4 - N
	arg[0] = 0;
	arg[1] = 0;
	arg[2] = N/kStep;
	arg[3] = N;
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int i;
	for(i=0;i<K;++i) pthread_create(&tred[i],&attr,threadFunc,arg);
	for(i=0;i<K;++i)
	{
		int rc = pthread_join(tred[i], NULL);
	}
	printf("%lld\n",arg[0]);
	return 0;
}
