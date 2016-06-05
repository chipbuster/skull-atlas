#ifndef ERROR_H
#define ERROR_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>

double squareerror(double epsilon1,int dim1[3],float* func,float* recon);

double infiniteerror(double epsilon1,int dim1[3],float maxval,float *func,float *recon);

/*
void errorcalculation(float epsilon1,int* dim1,float MAXVAL1,int nverts1,float* functest1,float* lambdatest1,float* recontest1,char* argv1,char* argv2,char* argv3);
*/

void errorcalculation(float epsilon1,int* dim1,float MAXVAL1,int nverts1,float* functest1,float* lambdatest1,float* recontest1,char* argv1,char* argv2,char* argv3,char* argv4);

void Juliooutput(float epsilon1,float MAXVAL1,float PSNR,float zero_compression_ratio,float nonzero_compression_ratio,float realcompressionratio,char* argv1);

void textfileoutput(char *fresultname,char *buffer1);

float whbfile_compressionratio(float epsilon1,char *fresultname);
#endif





