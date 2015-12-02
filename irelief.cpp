#include "irelief.h"
#define MAX 1000
#include <vector>

static int samplecount = MAX;
static int featurecount = MAX;
static float sigma = 1.0;
static int msize = 0;
//typedef vector<vector<int>> Matrix;

void getLine(char* matrix, char* line, int x)
{
	for (int i = 0; i < featurecount; i++)
	{
		line[i] = matrix[x*featurecount + i];		
	}
	return;
}

float norm(char* line, float* weight)
{
	float temp=0;
	for (int i = 0; i < featurecount; i++)
	{
		temp += static_cast<float>(line[i])*weight[i];
	}
	
	return temp;
}

float norm_2(char* line)
{
	float temp = 0;
	for (int i = 0; i < featurecount; i++)
	{
		temp += pow(static_cast<float>(line[i]), 2);
	}

	return pow(temp,0.5);
}

float norm_2(float* line)
{
	float temp = 0;
	for (int i = 0; i < featurecount; i++)
	{
		temp += pow(line[i], 2);
	}

	return pow(temp,0.5);
}

char abs(char a, char b)
{
	if (a > b)
		return a - b;
	else
		return b - a;
}

float abs(float a, float b)
{
	if (a > b)
		return a - b;
	else
		return b - a;
}

void sub(char* a, char* b,char* c)
{
	for (int i = 0; i < featurecount; ++i)
	{
		if (a[i] == 0 || b[i] == 0)
			c[i] = 0;
		else
		
		c[i] = abs(a[i], b[i]);
	}
}

void sub(float* a, float* b,float* c)
{

	for (int i = 0; i < featurecount; ++i)
	{
			c[i] = abs(a[i], b[i]);
	}
}



float cal_dis(char* a, char*b, float* weight)
{
	char* c = new char[featurecount];
	sub(a, b, c);
	float temp = norm(c, weight);
	delete[] c;
	return temp;
}

void cal(char* label, char** miss, char** hit)
{
	for (int i = 0; i < featurecount; ++i)
	{
		for (int j = 0; j < featurecount; ++j)
		{
			if (i == j)
			{
				miss[i][j] = 2;
				hit[i][j] = 2;
			}
			else if (label[i] == label[j])
			{
				miss[i][j] = 0;
				hit[i][j] = 1;
			}
			else
			{
				miss[i][j] = 1;
				hit[i][j] = 0;
			}
		}
	}
}


float kernel(float d, float sigma)
{
	return exp((-d) / sigma);
}

void dist_kernel(float** distance)
{
	for (int i = 0; i < samplecount; ++i)
	{
		for (int j = 0; j < featurecount; ++j)
		{
			distance[i][j] = kernel(distance[i][j], sigma);
		}
	}
}


float** distance(char* matrix,float* weight)
{
	auto dis = new float*[samplecount];
	for (int i = 0; i < samplecount; i++)
	{
		dis[i] = new float[featurecount];
	}
	for (int i = 0; i < samplecount; ++i)
	{
		for (int j = i; j < featurecount; ++j)
		{
			if (i == j)
			{
				dis[i][j] = 0;
				continue;
			}
			char* a = new char[featurecount];
			char* b = new char[featurecount];
			getLine(matrix, a, i);
			getLine(matrix, b, j);
			float temp =  cal_dis(a, b,weight);
			dis[i][j] = temp;
			dis[j][i] = temp;
			delete[] a;
			delete[] b;
		}
	}
	dist_kernel(dis);
	return dis;
}

void distance(char* matrix, float* weight,float** dis)
{
	for (int i = 0; i < samplecount; ++i)
	{
		for (int j = i; j < samplecount; ++j)
		{
			if (i == j)
			{
				dis[i][j] = 0;
				continue;
			}
			char* a = new char[featurecount];
			char* b = new char[featurecount];
			getLine(matrix, a, i);
			getLine(matrix, b, j);
			float temp = cal_dis(a, b, weight);
			dis[i][j] = temp;
			dis[j][i] = temp;
			delete[] a;
			delete[] b;
		}
	}
	dist_kernel(dis);
	return;
}

float compute_prob(float** distance, int x, int n, char* indices)
{
	float temp=0;
	for (int i = 0; i < featurecount; ++i)
	{
		if (indices[i] == 1)
			temp += distance[n][i];
	}
	return distance[n][x] / temp;
}

float compute_gn(char* matrix, float** distance, int n, char* m)
{
	float num = 0;
	for (int i = 0; i < featurecount; ++i)
	{
		if (m[i] == 1)
			num += distance[n][i];
	}
	float den = 0;
	for (int i = 0; i < featurecount; ++i)
	{
		if (i != n)
			den += distance[n][i];
	}
	return float(1.0) - num / den;
}

float* compute_weight(char* matrix, float* weight, char** miss, char** hit)
{
	float* n = new float[featurecount];
	init(n);
	auto dis = new float*[msize];
	for (int i = 0; i < msize; i++)
	{
		dis[i] = new float[msize];
		init(dis[i]);
	}
	distance(matrix, weight,dis);
	for (int i = 0; i < samplecount; ++i)
	{
		float* m_n = new float[featurecount];
		float* h_n = new float[featurecount];
		init(m_n);
		init(h_n);
		for (int j = 0; j < samplecount; ++j)
		{
			if (i == j)
			{
				continue;
			}
			if (miss[i][j] == 1)
			{
				float a = compute_prob(dis, j, i, miss[i]);
				char* x1 = new char[featurecount];
				getLine(matrix,x1,i);
				char* x2 = new char[featurecount];
				getLine(matrix, x2, j);
				char* b = new char[featurecount];
				sub(x1,x2,b);
				for (int k = 0; k < featurecount; k++)
				{
					m_n[k] += a*b[k];
				}
				delete[] b;
				delete[] x1;
				delete[] x2;
			}
			else if(hit[i][j]==1)
			{
				float a = compute_prob(dis, j, i, hit[i]);
				char* x1 = new char[featurecount];
				getLine(matrix, x1, i);
				char* x2 = new char[featurecount];
				getLine(matrix, x2, j);
				char* b = new char[featurecount];
				sub(x1, x2, b);
				for (int k = 0; k < featurecount; k++)
				{
					h_n[k] += a*b[k];
				}
				delete[] b;
				delete[] x1;
				delete[] x2;
			}
		}
		float gn = compute_gn(matrix, dis, i, miss[i]);
		for (int j = 0; j < featurecount; ++j)
		{
			n[j] += gn*(m_n[j] - h_n[j]);
		}
		delete[] m_n;
		delete[] h_n;
	}
	
	for (int i = 0; i < msize; i++)
	{
		delete[] dis[i];
	}
	delete[] dis;
	for (int i = 0; i < featurecount; ++i)
	{
		if (n[i] < 0)
		{
			n[i] = 0;
		}
		else
		{
			n[i] /= 1000;
		}
	}
	float n_norm = norm_2(n);
	for (int i = 0; i < featurecount; ++i)
	{
		n[i] /= n_norm;
	}
	return n;
}

int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

float* irelief(char* matrix, char* label, int& t,int s,int f)
{
	samplecount = s;
	featurecount = f;
	msize = max(s, f);
	float* w_old = new float[featurecount];
	for (int  i = 0; i < featurecount; ++i)
	{
		w_old[i] = float(1.0)/featurecount;
	}
	float n_norm = norm_2(w_old);
	for (int i = 0; i < featurecount; ++i)
	{
		w_old[i] /= n_norm;
	}
	
	char** miss = new char*[msize];
	char** hit = new char*[msize];
	for (int i = 0; i < msize; ++i)
	{
		miss[i] = new char[msize];
		hit[i] = new char[msize];
	}
	cal(label, miss, hit);
	float theta = float(0.001);
	float* w;
	while (true)
	{
		w = compute_weight(matrix, w_old, miss, hit);
		float* temp = new float[featurecount];
		sub(w, w_old, temp);
		float stp = norm_2(temp);
		cout << "stp is "<< stp << endl;
		cout << find_max(w) << endl;
		if (stp < theta || t == 20)
			break;
		//free(w_old);
		w_old = w;
		++t;
		cout << t << endl;
	}

	for (int i = 0; i < samplecount; ++i)
	{
		delete[] hit[i];
		delete[] miss[i];
	}


	//free(miss);
	//free(hit);
	//free(w_old);

	return w;
}



void printfloat(float* a)
{
	for (int i = 0; i < featurecount; i++)
	{
		if (i % 50 == 49)
			cout << endl;
		cout << a[i] << " ";
	}
}

void printchar(char* a)
{
	for (int i = 0; i < featurecount; i++)
	{
		if (i % 50 == 49)
			cout << endl;
		cout << static_cast<int>(a[i]) << " ";
	}
}

void init(float* a)
{
	for (int i = 0; i < featurecount; i++)
	{
		a[i] = float(0);
	}
}

void init(char* a)
{
	for (int i = 0; i < MAX; i++)
	{
		a[i] = char(0);
	}
}

int rank_x(int x, float* a)
{
	int temp = 1;
	for (int i = 0; i < featurecount; i++)
	{
		if (a[i] > a[x])
			temp++;
	}

	return temp;
}

int find_max(float* w)
{
	int c = 0;
	float temp = 0;
	for (int i=0; i < featurecount; i++)
	{
		if (w[i] > temp)
		{
			c = i;
			temp = w[i];
		}
	}
	cout << temp;
	return c;
}

void find_no_zero(float* w, vector<int> &pos)
{
	for (int i = 0; i < featurecount; i++)
	{
		if (w[i] > 0.1)
			pos.push_back(i);
	}

}
