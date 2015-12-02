#include "random.h"

#define MAX 1000

static int samplecount = MAX;
static int featurecount = MAX;

char locate(char* m, int x, int y)
{
	return m[x*featurecount + y];
}
void random_generate_ver1(char* m, char* label)
{
	for (int i = 0; i < 1000 * 1000; i++)
	{
		m[i] = static_cast<char>(rand() % 3);
	}
	for (int i = 0; i < 1000; i++)
	{
		label[i] = (locate(m, i, 0) + 1) / 2;
	}
	return;
}