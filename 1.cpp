#include<cstdio>
#include<vector>
#include<time.h>
using namespace std;

int main() {
 vector<int> a,b,c;
 time_t t1,t2;
 int n;
 int sum=0;
 scanf("%d",&n);
 time(&t1);
 a.resize(n);
 b.resize(n);
 c.resize(n);
 for (int i=0; i<n; i++) {
 b[i] = i;
 a[i] = i;
 }
 for (int i=0; i<n; i++) {
 c[i] = a[i] + b[i];
 sum = sum+c[i];
 }
 sum=0;
 time(&t2);
 printf("\n%f",difftime(t2,t1));
 time(&t1);
 int *a1 = (int*)malloc(sizeof(int)*n);
 int *b1 = (int*)malloc(sizeof(int)*n);
 int *c1 = (int*)malloc(sizeof(int)*n);
 for (int i=0; i<n; i++) {
 b1[i] = i;
 a1[i] = i;
 }
 for (int i=0; i<n; i++) {
 c1[i] = a1[i] + b1[i];
 sum = sum+c[i];
 }
 sum=0;
 time(&t2);
 printf("\n%f",difftime(t2,t1));
 free(a1);
 free(b1);
 free(c1);
 time(&t1);
 int *a2 = (int*)calloc(sizeof(int),n);
 int *b2 = (int*)calloc(sizeof(int),n);
 int *c2 = (int*)calloc(sizeof(int),n);
 for (int i=0; i<n; i++) {
 b2[i] = i;
 a2[i] = i;
 }
 for (int i=0; i<n; i++) {
 c2[i] = a2[i] + b2[i];
 sum = sum+c2[i];
 }
 time(&t2);
 printf("\n%f",difftime(t2,t1));
 free(a2);
 free(b2);
 free(c2);
 return 0;
} 
 
