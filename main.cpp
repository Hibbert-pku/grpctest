#include<iostream>
#include<stdlib.h>

using namespace std;

int main()
{
	int* a;
	int* b;
	a = (int *)malloc(sizeof(int) * 10);
	b = (int *)malloc(sizeof(int) * 10);
	for(int i=0;i<10;i++) {
		a[i]=1;
		b[i]=i;
	}
	test(a, b);
	for(int i=0;i<10;i++)
		printf("%d ",a[i]);
	printf("\n");
	return 0;
}
