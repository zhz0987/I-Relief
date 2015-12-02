#include "test.h"
#define MAX 1000

void test()
{
	char a[MAX];
	char b[MAX];
	float weight[1000];
	for (int i = 0; i < MAX; i++)
	{
		a[i] = 2;
		b[i] = 0;
		weight[i] = 0.001;
	}
	float ans;
	ans=norm_2(weight);
	cout << ans;
}