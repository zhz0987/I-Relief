#ifndef _IRELIEF_H
#define _IRELIEF_H
#include "SNPArffParser.h"
#include "StructArff.h"
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
void getLine(char* matrix, char* line, int x);
//char* getLine(char* matrix, int x);
float norm(char* line, float* weight);
float norm_2(char* line);
float norm_2(float* line);
char abs(char a, char b);
float abs(float a, float b);
void sub(char* a, char* b, char* c);
void sub(float* a, float* b, float* c);
void sum(float* a, float* b, float* temp);
int mul(char* a, char* b);
float cal_dis(char* a, char*b, float* weight);
void cal(char* label, char** miss, char** hit);
void irelief(StructArff* parser);
float kernel(float d, float sigma);
void dist_kernel(float** distance);
float** distance(char* matrix, float* weight);
float compute_prob(float** distance, int x, int n, char* indices);
float compute_gn(char* matrix, float** distance, int n, char* m);
float* compute_weight(char* matrix, float* weight, char** miss, char** hit);
void printfloat(float* a);
void printchar(char* a);
float* irelief(char* matrix, char* label, int& t,int s, int f);
void init(float* a);
int rank_x (int x, float* a);
int find_max(float* w);
void find_no_zero(float* w, vector<int> &pos);
#endif
