#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "SNPArffParser.h"
#include "StructArff.h"
#include <iostream>
#include "irelief.h"
#include "random.h"
#include "test.h"




using namespace std;



void folderloop(char* folderbase);
int main(int argc, char** argv)
{
	folderloop("D:\\snp1000");
	

	/*
	char* m = (char*)malloc(sizeof(char) * 1000 * 1000);
	char* label = (char*)malloc(sizeof(char) * 1000);
	random_generate_ver1(m, label);
	int t = 0;
	float* b = (float*)malloc(sizeof(float) * 1000);
	b=irelief(m, label, t);
	printfloat(b);
	cout << rank_x(0, b);*/
	
	getchar();
	return 0;
}

void folderloop(char* folderbase)
{
	char* filepath = new char[100];

	char folder[5];
	char tempfolder[5];
	memset(folder, 0, 5);
	strcat(folder, "/h01");
	folder[4] = 0;

	char file[21];
	char tempfile[21];
	memset(file, 0, 21);
	strcat(file, "/01_EDM-1_01txt.arff");
	file[20] = 0;

	FILE* writefile = fopen("D:\\snp.txt", "w+");

	for (int folderindex = 3; folderindex < 4; folderindex++)
	{
		strcpy(tempfolder, folder);
		tempfolder[3] += folderindex;

		for (int fileindex = 0; fileindex < 1; fileindex++)
		{
			strcpy(tempfile, file);
			tempfile[2] += folderindex;
			tempfile[8] += fileindex / 50;
			tempfile[10] += fileindex % 50 / 10;
			if (fileindex % 50 % 10<9)
			{
				tempfile[11] += fileindex % 50 % 10;
			}
			else
			{
				tempfile[10] += 1;
				tempfile[11] = '0';
			}
			
			memset(filepath, 0, 100);
			strcat(filepath, folderbase);
			strcat(filepath, tempfolder);
			strcat(filepath, tempfile);

			SNPArffParser s;
			StructArff* a=s.ParseSNPArffFile(filepath);
			//s.DisplayArffData(a);
			//s.DisplayArffLabels(a);
			//cout << a->SNPNames[1927] << endl;
			//cout << a->SNPNames[961] << endl;;
			//for (int i = 0; i < a->SampleCount; i++)
			//	cout << static_cast<int>(a->Labels[i]);
			////
			//
			// you can add your code here
			////
			int t = 0;
			cout << a->FeatureCount << " " << a->SampleCount;
			float* b = (float*)malloc(sizeof(float)*(a->FeatureCount));
			b=irelief(a->Matrix,a->Labels,t,a->SampleCount,a->FeatureCount);
			printfloat(b);
			cout << t << endl;
			vector<int> i;
			find_no_zero(b, i);
			cout << i.size();
			for (auto it = i.begin(); it != i.end(); it++)
			{
				cout << *it << " " << a->SNPNames[*it]<<endl;
				fprintf(writefile, "%f %s \n", b[*it], a->SNPNames[*it]);
			}
			//for (int i = 0; i < 1000; i++)
			//{
				//cout << b[i] << " ";
			//}
			//cout << rank_x(998, b)<<" " <<rank_x(999,b);
//			fprintf(writefile, "%d %d \n", rank_x(998, b), rank_x(999, b));
		}	
	}

	fclose(writefile);
}
