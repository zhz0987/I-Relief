#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;
const int MAX=1000;


int calculateDistance(int i,int j,const int data[MAX][MAX]);
void findNear( int distance[MAX][MAX],int* classes,
 int* nearMiss, int* nearHit);
void relief( int* classes,int* nearMiss
	, int* nearHit,int* weight,const int data[MAX][MAX]);
void print_vector(const int v[MAX]);

int main2()
{
	srand(time(0));
	auto data = new int[MAX][MAX];
	auto classes = new int[MAX];
	int random;
	cout<<"0 for given data, positive number for random seed"<<endl;
	cin>>random;
	srand(random);
	vector<string> name;
	if(random<=0)
	{
	string temp;
	ifstream file1;
	file1.open("D:\\Documents\\c++\\03_EDM-1_50txt.arff");
	if (!file1.is_open())
	{
		cout << "unknown error" << endl;
	}
	while (getline(file1, temp))
	{
		stringstream ss(temp);
		ss >> temp;
		cout << temp << endl;
		if (temp == "@ATTRIBUTE")
		{
			ss >> temp;
			name.push_back(temp);
			cout << temp << endl;
		}
		else if (temp == "@DATA")
		{
			break;
		}
	}
	int data_count = 0;
	while (getline(file1, temp))
	{
		int count = 0;
		for (int i = 0; i < temp.length(); i+=2)
		{
			
			if (temp[i] == 'f')
			{
				classes[data_count++] = 0;
				break;
			}
			else if (temp[i] == 't')
			{
				classes[data_count++] = 1;
				break;
			}
			data[data_count][count++] = static_cast<int>(temp[i] - 48);
				
		}					
	}
	}
	else
	{
	for(int i=0;i<MAX;i++)
	{
		for(int j=0;j<MAX;j++)
		{
			data[i][j]=(rand() % 3);
		}
		classes[i]=(data[i][0] / 2);
	}

	}
	auto nearHit=new int[MAX];
	auto nearMiss=new int[MAX];
	auto distance=new int[MAX][MAX];
	for(int i=0;i<MAX;i++)
	{
		for(int j=i;j<MAX;j++)
		{
			int temp=calculateDistance(i,j,data);
			distance[i][j]=temp;
			distance[j][i]=temp;
		}
	}
	findNear(distance,classes,nearMiss,nearHit);
	print_vector(nearMiss);
	print_vector(nearHit);
	int weight[MAX];
	relief(classes,nearMiss,nearHit,weight,data);
	print_vector(weight);
	delete[] data;
	delete[] classes;
	return 0;
}

int calculateDistance(int i,int j,const int data[MAX][MAX])
{
	int result=0;
	for(int k=0;k<MAX;k++)
	{
		result+=abs(data[i][k]-data[j][k]);
	}
	
	return result;
}

void findNear( int distance[MAX][MAX],int* classes,
 int* nearMiss, int* nearHit)
{
	int minHit;
	int minMiss;
	int missPos;
	int hitPos;
	for(int i=0;i<MAX;i++)
	{
		int temp=classes[i];
		minHit=MAX*10;
		minMiss=MAX*10;
		missPos=hitPos=0;
		for(int j=0;j<MAX;j++)
		{
			if(i==j)
				continue;
			if(classes[j]==temp)
			{
				if(minHit>distance[i][j])
				{
					minHit=distance[i][j];
					hitPos=j;
				}
			}
			else if(classes[j]!=temp)
			{
				if(minMiss>distance[i][j])
				{
					minMiss=distance[i][j];
					missPos=j;
				}
			}
		}
		nearMiss[i]=missPos;
		nearHit[i]=hitPos;
	}
}

void relief(int* classes,int* nearMiss
	, int* nearHit,int* weight,const int data[MAX][MAX])
{
	for(int i=0;i<MAX;i++)
	{
		weight[i]=0;
	}
	for(int i=0;i<MAX;i++)
	{
		for(int j=0;j<MAX;j++)
		{
			if(data[i][j]==data[nearMiss[i]][j])
				weight[j]--;
			else
				weight[j]++;
			if(data[i][j]==data[nearHit[i]][j])
				weight[j]++;
			else
				weight[j]--;
		}

	}

}

void print_vector(const int* v)
{
	for(int i=0;i<MAX;i++)
	{
		cout<<v[i]<<" ";
	}
	cout<<endl;
}

