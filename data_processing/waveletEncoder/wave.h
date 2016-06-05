#ifndef WAVE_H
#define WAVE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>

#define MAXSIZE 520
#define MAXLEVEL 9

extern float* functest;

void readRawiv(const char* fname, int dim[3],float &maxval);
//reads in the stuff and places it into functest[]

void writeRawiv(const char* fname,char* fresultname, float* buffer,int dim[3]);
//void writeRawiv(char* fresultname, double* buffer,int dim[3]);
//void Wavelet(int x, int y, int z, int size);
//void RebuildWavelet(int x, int y, int z, int size);

void writeRawivnew(const char* fname,char* fresultname, float* buffer,int dim[3]);

//float minmaxRawiv(float* buffer,int dim[3],int minmax1,int index1);
//calculate the min/max in a volume data
//buffer dimension is dim[0]*dim[1]*dim[2]
//minmax1==1, caculate the min
//minmax1==2, caculate the max
//index1==1, caculate the x coordinate
//index1==2, caculate the y coordinate
//index1==3, caculate the z coordinate


void thresh(float epsilon, int dim[3],float maxval,float* lambda);
unsigned int findlevel(unsigned int val,unsigned int maxlvl);
void testlevel(int x, int y, int z,int lvl, int size, float *lambda,int dim);

void Wavelet(int x, int y, int z, int dim, int size,int lvl,float eps,float maxval, float *func, float *lambda);
//takes volume and func and computes wavelets ino lambda

void RebuildWavelet(int x, int y, int z, int dim, int size, float *lambda, float *recon);
//takes the wavelets from lambda and rebuilds the volume into recon

#endif





