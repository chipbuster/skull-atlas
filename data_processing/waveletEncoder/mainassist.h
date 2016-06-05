#ifndef MAINASSIST_H
#define MAINASSIST_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int maximum(int dim1[3]);

void mainzeroblocktable(float epsilon,float maxval,int newsize1,float* func,float* lambda,float* recon,char* argv1,char* argv2,char* argv3);

void integer2string(int input1,char* output1,int* outputlen1);

void double2string(double input1,char* output1,int* outputlen1,int precision);
#endif
